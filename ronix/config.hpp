#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "pch.hpp"

struct ConfigData {
	enum KeyType : int {
		KEYTYPE_HOLD,
		KEYTYPE_TOGGLE
	};

	const SDL_Keycode guiKey = SDLK_INSERT;
	bool bunnyhopEnable;
	bool autoStrafeEnable;
	bool autoStrafeSilent;
	SDL_Keycode autoStrafeKey;
	KeyType autoStrafeKeyType;
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
