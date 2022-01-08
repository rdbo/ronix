#include <ronix.hpp>

void SDLCALL Ronix::Hooks::SDL_GL_SwapWindow(SDL_Window *window)
{
	static PseudoGLContextPtr orig_context = Ronix::Data::cstrike->LauncherMgr->GetMainContext();
	static PseudoGLContextPtr user_context = nullptr;

	if (!user_context) {
		RONIX_LOG("SDL Window: %p\n", reinterpret_cast<void *>(window));
		RONIX_LOG("Original Context: %p\n", reinterpret_cast<void *>(orig_context));
		user_context = Ronix::Data::cstrike->LauncherMgr->CreateExtraContext();
		RONIX_LOG("Context: %p\n", reinterpret_cast<void *>(user_context));
		Ronix::Data::cstrike->LauncherMgr->MakeContextCurrent(user_context);

		// Initialize ImGui
		auto imgui_ctx = ImGui::CreateContext();
		ImGui::SetCurrentContext(imgui_ctx);

		ImGui::GetIO().IniFilename = nullptr;

		ImGui_ImplSDL2_InitForOpenGL(window, user_context);
		ImGui_ImplOpenGL2_Init();
		RONIX_LOG("ImGui Initialized\n");

		// Setup GUI
		Ronix::Data::gui->Setup();
	}

	Ronix::Data::cstrike->LauncherMgr->MakeContextCurrent(user_context);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	// Show cursor when GUI is shown
	ImGui::GetIO().MouseDrawCursor = Ronix::Data::gui->IsVisible();
	ImGui::GetIO().WantCaptureMouse = Ronix::Data::gui->IsVisible();
	ImGui::GetIO().WantCaptureKeyboard = Ronix::Data::gui->IsVisible();

	Ronix::Data::gui->Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	Ronix::Data::cstrike->LauncherMgr->MakeContextCurrent(orig_context);

	if (Ronix::Data::hasShutdown) {
		::SDL_GL_SwapWindow(window);
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		// Ronix::Data::cstrike->LauncherMgr->DeleteContext(user_context);
		return;
	}

	return ::SDL_GL_SwapWindow(window);
}
