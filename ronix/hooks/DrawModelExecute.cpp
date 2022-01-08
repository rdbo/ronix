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

	mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, chamsData.wireframe);
	mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, chamsData.ignoreZ);
	mat->SetMaterialVarFlag(MATERIAL_VAR_SELFILLUM, chamsData.selfIllum);
	cstrike->ModelRender->ForcedMaterialOverride(mat);
	return true;
}

void Ronix::Hooks::DrawModelExecute(IVModelRender *thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	static DrawModelExecuteFn fnDrawModelExecute = Ronix::Data::ModelRenderVmt->GetOriginal<DrawModelExecuteFn>(19);

	if (!config->data.chamsEnable || (config->data.chamsHoldKey != SDL_SCANCODE_UNKNOWN && keys[config->data.chamsHoldKey] != SDL_KEYDOWN) || !cstrike->LocalPlayer)
		return fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
	
	const char *model_name = cstrike->ModelInfoClient->GetModelName(pInfo.pModel);
	ConfigData::ChamsType chams_type = ConfigData::CHAMS_TYPE_INVAL;
	if (std::strstr(model_name, "models/player") && cstrike->LocalPlayer->GetTeamNumber() > TEAM_SPECTATOR) {
		auto player = reinterpret_cast<CBasePlayer *>(cstrike->EntityList->GetClientEntity(pInfo.entity_index));
		if (player && player->IsPlayer() && !player->IsDormant() && player->IsAlive() && player->GetHealth() > 0)
			chams_type = player->GetTeamNumber() != cstrike->LocalPlayer->GetTeamNumber() ? ConfigData::CHAMS_TYPE_ENEMIES : ConfigData::CHAMS_TYPE_ALLIES;
	} else if (std::strstr(model_name, "models/weapons/v_")) {
		chams_type = ConfigData::CHAMS_TYPE_VIEWMODEL;
	} else if (std::strstr(model_name, "models/weapons/w_c4")) {
		chams_type = ConfigData::CHAMS_TYPE_C4;
	} else if (std::strstr(model_name, "models/weapons/w_")) {
		RONIX_LOG("Model %s\n", model_name);
		chams_type = ConfigData::CHAMS_TYPE_WEAPONS;
	} else if (std::strstr(model_name "models/Weapons")) {
		chams_type = ConfigData::CHAMS_TYPE_GRENADES;
	}

	if (chams_type != ConfigData::CHAMS_TYPE_INVAL) {
		bool chams_done = false;
		for (size_t i = 0; i < CHAMS_COUNT; ++i) {
			if (Chams(config->data.chamsData[chams_type][i])) {
				chams_done = true;
				fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
			}
		}
		
		if (chams_done) {
			cstrike->ModelRender->ForcedMaterialOverride(nullptr);
			return;
		}
	}

	return fnDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);	
}
