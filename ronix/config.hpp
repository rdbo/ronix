#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "pch.hpp"

#define CHAMS_COUNT 5

struct ConfigData {
	enum ChamsType : int {
		CHAMS_TYPE_ENEMIES = 0,
		CHAMS_TYPE_ALLIES,
		CHAMS_TYPE_VIEWMODEL,
		CHAMS_TYPE_WEAPONS,
		CHAMS_TYPE_GRENADES,
		CHAMS_TYPE_C4,
		CHAMS_TYPE_INVAL
	};

	enum ChamsMat : int {
		CHAMS_MAT_FLAT = 0,
		CHAMS_MAT_PLASTIC,
		CHAMS_MAT_GLASS,
		CHAMS_MAT_INVAL
	};

	struct ChamsData {
		bool enable;
		bool wireframe;
		bool ignoreZ;
		bool selfIllum;
		ChamsMat mat;
		float color[4];
	};

	enum EspSnaplinePos : int {
		ESP_SNAPLINE_TOP = 0,
		ESP_SNAPLINE_CENTER,
		ESP_SNAPLINE_BOTTOM,
		ESP_SNAPLINE_INVAL
	};

	const SDL_Scancode guiKey = SDL_SCANCODE_INSERT;
	bool bunnyhopEnable;
	bool autoStrafeEnable;
	bool autoStrafeSilent;
	bool autoStrafeRage;
	SDL_Scancode autoStrafeHoldKey;
	SDL_Scancode autoStrafeToggleKey;
	bool chamsEnable;
	SDL_Scancode chamsHoldKey;
	SDL_Scancode chamsToggleKey;
	ChamsData chamsData[CHAMS_TYPE_INVAL][CHAMS_COUNT];
	bool espSnaplineEnable;
	SDL_Scancode espSnaplineHoldKey;
	SDL_Scancode espSnaplineToggleKey;
	EspSnaplinePos espSnaplinePos;
	float espSnaplineTeamVisColor[4];
	float espSnaplineEnemyVisColor[4];
	float espSnaplineTeamInvisColor[4];
	float espSnaplineEnemyInvisColor[4];
};

class Config {
private:
	std::string path;
	std::string MakePath(std::string name);
public:
	ConfigData data;
	Config(std::string path);
	~Config();
	void Reset();
	void Save(std::string name);
	void Load(std::string name);
};

#endif
