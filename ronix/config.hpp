#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "pch.hpp"

struct ConfigData {
	bool bhopEnable;
};

class Config {
private:
	std::string path;
	ConfigData data;
	std::string MakePath(std::string name);
public:
	Config();
	~Config();
	ConfigData &GetData();
	void Reset();
	void Save(std::string name);
	void Load(std::string name);
};

#endif
