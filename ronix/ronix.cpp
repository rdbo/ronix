#include "ronix.hpp"
#include "utils/module.hpp"
#include "utils/memory.hpp"

bool Ronix::Data::hasShutdown = false;
std::unique_ptr<Logger> Ronix::Data::logger;
std::unique_ptr<Config> Ronix::Data::config;
std::unique_ptr<cstrike_t> Ronix::Data::cstrike;
std::unique_ptr<VmtMgr> Ronix::Data::BaseClientDllVmt;
std::unique_ptr<VmtMgr> Ronix::Data::ModelRenderVmt;

static uintptr_t *pSDL_GL_SwapWindow;
static uintptr_t oSDL_GL_SwapWindow;
static uintptr_t *pSDL_PollEvent;
static uintptr_t oSDL_PollEvent;

void Ronix::Init()
{
	Ronix::Data::logger = std::unique_ptr<Logger>(new Logger("/tmp/ronix.log"));
	RONIX_LOG("Loaded\n");
	
	Ronix::Data::config = std::unique_ptr<Config>(new Config());
	Ronix::Data::cstrike = std::unique_ptr<cstrike_t>(new cstrike_t());

	CreateInterfaceFn fnCreateInterface; // CreateInterface helper

	// launcher.so
	auto launcher = Module("bin/launcher.so");
	RONIX_LOG("Launcher Name: %s\n", launcher.GetName().c_str());
	RONIX_LOG("Launcher Base: %p\n", launcher.GetBase());
	RONIX_LOG("Launcher Size: %p\n", reinterpret_cast<void *>(launcher.GetSize()));
	RONIX_LOG("Launcher Handle: %p\n", launcher.GetHandle());

	Ronix::Data::cstrike->LauncherMgr = **reinterpret_cast<ILauncherMgr ***>(
		&(Memory::FindPattern<char *>("\x24\x8B\x1D\x00\x00\x00\x00", "xxx????", launcher.GetBase(), launcher.GetSize()))[3]
	);
	RONIX_LOG("LauncherMgr: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->LauncherMgr));

	// call <SDL_GL_SwapWindow>
	pSDL_GL_SwapWindow = reinterpret_cast<uintptr_t *>(
		&(Memory::FindPattern<char *>("\x89\x55\xCC\x89\04\x24\xE8\x00\x00\x00\x00", "xxxxxxx????", launcher.GetBase(), launcher.GetSize()))[7]
	);
	RONIX_LOG("SDL_GL_SwapWindow Call Pointer: %p\n", reinterpret_cast<void *>(pSDL_GL_SwapWindow));

	// call <SDL_PollEvent>
	pSDL_PollEvent = reinterpret_cast<uintptr_t *>(
		&(Memory::FindPattern<char *>("\x8D\xB6\x00\x00\x00\x00\x89\x34\x24\xE8\x00\x00\x00\x00", "xxxxxxxxxx????", launcher.GetBase(), launcher.GetSize()))[10]
	);
	RONIX_LOG("SDL_PollEvent Call Pointer: %p\n", reinterpret_cast<void *>(pSDL_PollEvent));

	RONIX_LOG("====================\n");

	// client.so
	auto client = Module("cstrike/bin/client.so");
	RONIX_LOG("Client Name: %s\n", client.GetName().c_str());
	RONIX_LOG("Client Base: %p\n", client.GetBase());
	RONIX_LOG("Client Size: %p\n", reinterpret_cast<void *>(client.GetSize()));
	RONIX_LOG("Client Handle: %p\n", client.GetHandle());

	fnCreateInterface = client.FindSymbol<CreateInterfaceFn>(CREATEINTERFACE_PROCNAME);
	RONIX_LOG("CreateInterface: %p\n", reinterpret_cast<void *>(fnCreateInterface));

	Ronix::Data::cstrike->EntityList = reinterpret_cast<IClientEntityList *>(fnCreateInterface(VCLIENTENTITYLIST_INTERFACE_VERSION, nullptr));
	RONIX_LOG("EntityList: %p\n", Ronix::Data::cstrike->EntityList);

	Ronix::Data::cstrike->GameMovement = reinterpret_cast<CGameMovement *>(fnCreateInterface(INTERFACENAME_GAMEMOVEMENT, nullptr));
	RONIX_LOG("GameMovement: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->GameMovement));

	Ronix::Data::cstrike->BaseClientDll = reinterpret_cast<IBaseClientDLL *>(fnCreateInterface(CLIENT_DLL_INTERFACE_VERSION, nullptr));
	RONIX_LOG("BaseClientDll: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->BaseClientDll));

	Ronix::Data::BaseClientDllVmt = std::unique_ptr<VmtMgr>(new VmtMgr(*reinterpret_cast<void ***>(Ronix::Data::cstrike->BaseClientDll)));
	RONIX_LOG("BaseClientDll VMT: %p\n", Ronix::Data::BaseClientDllVmt->GetVmt());

	/* Get 'input' from IN_ActivateMouse
	 * IN_ActivateMouse:
	 *   mov 0xed9455fc,%eax ; pointer to 'input'
	 *   ...
	 * Offset: 1
	 */
	Ronix::Data::cstrike->Input = **reinterpret_cast<CInput ***>(
		&(reinterpret_cast<char *>(Ronix::Data::BaseClientDllVmt->GetFunction(14)))[1]
	);
	RONIX_LOG("Input: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->Input));

	/* Get 'gpGlobals' from HudUpdate
	 * HudUpdate:
	 *   push %ebp
	 *   mov %esp,%ebp
	 *   push %ebx
	 *   sub $0x34,%esp
	 *   mov 0xed8fc1d4,%eax ; pointer to 'gpGlobals'
	 * Offset: 8
	 */
	Ronix::Data::cstrike->GlobalVars = **reinterpret_cast<CGlobalVarsBase ***>(
		&(Memory::FindPattern<char *>("\xA1\x00\x00\x00\x00\x0F\xB6\x5D\x0C\xF3\x0F\x10\x40\x10", "x????xxxxxxxxx", client.GetBase(), client.GetSize()))[1]
	);
	RONIX_LOG("GlobalVars: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->GlobalVars));

	RONIX_LOG("====================\n");

	auto engine = Module("bin/engine.so");
	RONIX_LOG("Engine Name: %s\n", engine.GetName().c_str());
	RONIX_LOG("Engine Base: %p\n", engine.GetBase());
	RONIX_LOG("Engine Size: %p\n", reinterpret_cast<void *>(engine.GetSize()));
	RONIX_LOG("Engine Handle: %p\n", engine.GetHandle());

	fnCreateInterface = engine.FindSymbol<CreateInterfaceFn>(CREATEINTERFACE_PROCNAME);
	RONIX_LOG("CreateInterface: %p\n", reinterpret_cast<void *>(fnCreateInterface));

	Ronix::Data::cstrike->EngineClient = reinterpret_cast<IVEngineClient *>(fnCreateInterface(VENGINE_CLIENT_INTERFACE_VERSION, nullptr));
	RONIX_LOG("EngineClient: %p\n", Ronix::Data::cstrike->EngineClient);

	Ronix::Data::cstrike->ModelInfoClient = reinterpret_cast<IVModelInfoClient *>(fnCreateInterface(VMODELINFO_CLIENT_INTERFACE_VERSION, nullptr));
	RONIX_LOG("ModelInfoClient: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->ModelInfoClient));

	Ronix::Data::cstrike->ModelRender = reinterpret_cast<IVModelRender *>(fnCreateInterface(VENGINE_HUDMODEL_INTERFACE_VERSION, nullptr));
	RONIX_LOG("ModelRender: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->ModelRender));

	Ronix::Data::ModelRenderVmt = std::unique_ptr<VmtMgr>(new VmtMgr(*reinterpret_cast<void ***>(Ronix::Data::cstrike->ModelRender)));
	RONIX_LOG("ModelRender VMT: %p\n", reinterpret_cast<void *>(Ronix::Data::ModelRenderVmt->GetVmt()));

	Ronix::Data::cstrike->RenderView = reinterpret_cast<IVRenderView *>(fnCreateInterface(VENGINE_RENDERVIEW_INTERFACE_VERSION, nullptr));
	RONIX_LOG("RenderView: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->RenderView));

	RONIX_LOG("====================\n");

	// materialsystem.so
	auto materialsystem = Module("bin/materialsystem.so");
	RONIX_LOG("MaterialSystem Name: %s\n", materialsystem.GetName().c_str());
	RONIX_LOG("MaterialSystem Base: %p\n", materialsystem.GetBase());
	RONIX_LOG("MaterialSystem Size: %p\n", reinterpret_cast<void *>(materialsystem.GetSize()));
	RONIX_LOG("MaterialSystem Handle: %p\n", materialsystem.GetHandle());

	fnCreateInterface = materialsystem.FindSymbol<CreateInterfaceFn>(CREATEINTERFACE_PROCNAME);
	RONIX_LOG("CreateInterface: %p\n", reinterpret_cast<void *>(fnCreateInterface));

	Ronix::Data::cstrike->Cvar = reinterpret_cast<ICvar *>(fnCreateInterface(CVAR_INTERFACE_VERSION, nullptr));
	RONIX_LOG("Cvar: %p\n", reinterpret_cast<void *>(Ronix::Data::cstrike->Cvar));

	Ronix::Data::cstrike->MaterialSystem = reinterpret_cast<IMaterialSystem *>(fnCreateInterface(MATERIAL_SYSTEM_INTERFACE_VERSION, nullptr));
	RONIX_LOG("MaterialSystem: %p\n", Ronix::Data::cstrike->MaterialSystem);

	RONIX_LOG("====================\n");

	// Hooks
	Memory::Protect(reinterpret_cast<void *>(pSDL_GL_SwapWindow), sizeof(*pSDL_GL_SwapWindow), PROT_EXEC | PROT_READ | PROT_WRITE);
	oSDL_GL_SwapWindow = *pSDL_GL_SwapWindow;
	*pSDL_GL_SwapWindow = reinterpret_cast<uintptr_t>(Ronix::Hooks::SDL_GL_SwapWindow) - reinterpret_cast<uintptr_t>(pSDL_GL_SwapWindow) - 4;

	Memory::Protect(reinterpret_cast<void *>(pSDL_PollEvent), sizeof(*pSDL_PollEvent), PROT_EXEC | PROT_READ | PROT_WRITE);
	oSDL_PollEvent = *pSDL_PollEvent;
	*pSDL_PollEvent = reinterpret_cast<uintptr_t>(Ronix::Hooks::SDL_PollEvent) - reinterpret_cast<uintptr_t>(pSDL_PollEvent) - 4;

	Ronix::Data::BaseClientDllVmt->Hook(21, reinterpret_cast<void *>(Ronix::Hooks::CreateMove));
	Ronix::Data::ModelRenderVmt->Hook(19, reinterpret_cast<void *>(Ronix::Hooks::DrawModelExecute));

	RONIX_LOG("Hooks Initialized\n");
	RONIX_LOG("====================\n");

	// ---
	Color col = Color(255, 0, 0);
	Ronix::Data::cstrike->Cvar->ConsoleColorPrintf(col, "[RONIX] Loaded\n");
}

void Ronix::Shutdown()
{
	if (Ronix::Data::hasShutdown)
		return;
	
	Ronix::Data::ModelRenderVmt->Restore(19);
	Ronix::Data::BaseClientDllVmt->Restore(21);

	*pSDL_PollEvent = oSDL_PollEvent;
	*pSDL_GL_SwapWindow = oSDL_GL_SwapWindow;

	Ronix::Data::hasShutdown = true;
	RONIX_LOG("Unloaded\n");
}

void __attribute__((constructor)) ronix_entry()
{
	Ronix::Init();
}

void __attribute__((destructor)) ronix_exit()
{
	// Ronix::Shutdown();
}
