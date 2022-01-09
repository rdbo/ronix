#include <ronix.hpp>

using namespace Ronix::Data;

typedef void (*DrawModelExecuteFn)(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);

void Ronix::Hooks::DrawModelExecute(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	static DrawModelExecuteFn fnDrawModelExecute = Ronix::Data::ModelRenderVmt->GetOriginal<DrawModelExecuteFn>(19);

	if (!Ronix::Hacks::Chams(thisptr, state, pInfo, pCustomBoneToWorld))
		return fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);	
}
