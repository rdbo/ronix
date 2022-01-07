#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "pch.hpp"

struct ConfigData {
	enum KeyType : int {
		KEYTYPE_HOLD,
		KEYTYPE_TOGGLE,
		KEYTYPE_INVAL
	};

	const SDL_Scancode guiKey = SDL_SCANCODE_INSERT;
	bool bunnyhopEnable;
	bool autoStrafeEnable;
	bool autoStrafeSilent;
	SDL_Scancode autoStrafeKey;
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
