#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "pch.hpp"

struct ConfigData {
	bool bunnyhopEnable;
	bool autoStrafeEnable;
	bool autoStrafeSilent;
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
