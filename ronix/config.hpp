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
		ESP_SNAPLINE_POS_TOP = 0,
		ESP_SNAPLINE_POS_CENTER,
		ESP_SNAPLINE_POS_BOTTOM,
		ESP_SNAPLINE_POS_INVAL
	};

	enum EspSnaplineType : int {
		ESP_SNAPLINE_TYPE_HEAD = 0,
		ESP_SNAPLINE_TYPE_FOOT,
		ESP_SNAPLINE_TYPE_INVAL
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
	EspSnaplineType espSnaplineType;
	float espSnaplineThickness;
	float espSnaplineTeamVisColor[4];
	float espSnaplineEnemyVisColor[4];
	float espSnaplineTeamInvisColor[4];
	float espSnaplineEnemyInvisColor[4];
	float espSnaplineOutlineColor[4];
	bool triggerbotEnable;
	SDL_Scancode triggerbotHoldKey;
	SDL_Scancode triggerbotToggleKey;
	float triggerbotDelay;
	bool espBoxEnable;
	SDL_Scancode espBoxHoldKey;
	SDL_Scancode espBoxToggleKey;
	float espBoxThickness;
	float espBoxTeamVisColor[4];
	float espBoxEnemyVisColor[4];
	float espBoxTeamInvisColor[4];
	float espBoxEnemyInvisColor[4];
	float espBoxOutlineColor[4];
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
