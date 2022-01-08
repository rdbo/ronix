#include <ronix.hpp>

using namespace Ronix::Data;

typedef void (*DrawModelExecuteFn)(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);

bool Chams(ConfigData::ChamsData &chamsData)
{
	if (!chamsData.enable)
		return false;

	IMaterial *mat = nullptr;
	switch (chamsData.mat) {
	case ConfigData::CHAMS_MAT_FLAT:
		mat = cstrike->MaterialSystem->FindMaterial("debug/debugtranslucentsinglecolor", nullptr);
		break;
	case ConfigData::CHAMS_MAT_PLASTIC:
		mat = cstrike->MaterialSystem->FindMaterial("debug/debugambientcube", nullptr);
		break;
	case ConfigData::CHAMS_MAT_GLASS:
		mat = cstrike->MaterialSystem->FindMaterial("models/gibs/glass/glass", nullptr);
		break;
	default:
		return false;
	}

	if (chamsData.mat == ConfigData::CHAMS_MAT_FLAT || chamsData.mat == ConfigData::CHAMS_MAT_PLASTIC)
		cstrike->RenderView->SetColorModulation(chamsData.color);
	else
		mat->FindVar("$envmaptint", nullptr)->SetVecValue(chamsData.color[0], chamsData.color[1], chamsData.color[2]);
	
	mat->AlphaModulate(chamsData.color[3]);

	mat->SetMaterialVarFlag(MATERIAL_VAR_FLAT, chamsData.flat);
	mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, chamsData.wireframe);
	mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, chamsData.ignoreZ);
	cstrike->ModelRender->ForcedMaterialOverride(mat);
	return true;
}

void Ronix::Hooks::DrawModelExecute(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	static DrawModelExecuteFn fnDrawModelExecute = Ronix::Data::ModelRenderVmt->GetOriginal<DrawModelExecuteFn>(19);

	if (!config->data.chamsEnable || (config->data.chamsHoldKey != SDL_SCANCODE_UNKNOWN && keys[config->data.chamsHoldKey] != SDL_KEYDOWN) || !cstrike->LocalPlayer)
		return fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
	
	const char *model_name = cstrike->ModelInfoClient->GetModelName(pInfo.pModel);
	if (std::strstr(model_name, "models/player") && cstrike->LocalPlayer->GetTeamNumber() > TEAM_SPECTATOR) {
		auto player = reinterpret_cast<CBasePlayer *>(cstrike->EntityList->GetClientEntity(pInfo.entity_index));
		if (player && player->IsPlayer() && !player->IsDormant() && player->IsAlive() && player->GetHealth() > 0) {
			ConfigData::ChamsType chamsType;
			bool chamsDone = false;
			if (player->GetTeamNumber() != cstrike->LocalPlayer->GetTeamNumber())
				chamsType = ConfigData::CHAMS_TYPE_ENEMIES;
			else
				chamsType = ConfigData::CHAMS_TYPE_ALLIES;

			for (size_t i = 0; i < CHAMS_COUNT; ++i) {
				if (Chams(config->data.chamsData[chamsType][i])) {
					chamsDone = true;
					fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);	
				}
			}
			
			if (chamsDone) {
				cstrike->ModelRender->ForcedMaterialOverride(nullptr);
				return;
			}
		}
	} else if (std::strstr(model_name, "models/weapons")) {
		bool chamsDone = false;
		for (size_t i = 0; i < CHAMS_COUNT; ++i) {
			if (Chams(config->data.chamsData[ConfigData::CHAMS_TYPE_WEAPONS][i])) {
				chamsDone = true;
				fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
			}
		}
		
		if (chamsDone) {
			cstrike->ModelRender->ForcedMaterialOverride(nullptr);
			return;
		}
	}

	return fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);	
}
