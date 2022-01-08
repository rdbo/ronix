#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "pch.hpp"

#define CHAMS_COUNT 3

struct ConfigData {
	enum ChamsType : int {
		CHAMS_TYPE_ENEMIES = 0,
		CHAMS_TYPE_ALLIES,
		CHAMS_TYPE_WEAPONS,
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
		bool flat;
		bool wireframe;
		bool ignoreZ;
		ChamsMat mat;
		float color[4];
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
};

class Config {
private:
	std::string path;
	std::string MakePath(std::string name);
public:
	ConfigData data;
	Config();
	~Config();
	void Reset();
	void Save(std::string name);
	void Load(std::string name);
};

#endif
