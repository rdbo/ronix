#include <ronix.hpp>

using namespace Ronix::Data;

typedef void (*FrameStageNotifyFn)(IBaseClientDLL *thisptr, ClientFrameStage_t curStage);

void Ronix::Hooks::FrameStageNotify(IBaseClientDLL *thisptr, ClientFrameStage_t curStage)
{
	static FrameStageNotifyFn fnFrameStageNotify = BaseClientDllVmt->GetOriginal<FrameStageNotifyFn>(35);

	if (curStage == ClientFrameStage_t::FRAME_START) {
		cstrike->LocalPlayer = (CBasePlayer *)cstrike->EntityList->GetClientEntity(cstrike->EngineClient->GetLocalPlayer());
		if (cstrike->LocalPlayer)
			gameData->Update();
	}

	return fnFrameStageNotify(thisptr, curStage);
}
