#include <config.hpp>
#include <ronix.hpp>
#include "utils/json_helper.hpp"

Config::Config(std::string path)
{
	this->path = path;
	this->Reset();
}

Config::~Config()
{

}

std::string Config::MakePath(std::string name)
{
	return this->path + '/' + name + ".ronix";
}

void Config::Reset()
{
	this->data.bunnyhopEnable = false;
	this->data.autoStrafeEnable = false;
	this->data.autoStrafeSilent = false;
	this->data.autoStrafeRage = false;
	this->data.autoStrafeHoldKey = SDL_SCANCODE_UNKNOWN;
	this->data.autoStrafeToggleKey = SDL_SCANCODE_UNKNOWN;
	this->data.chamsEnable = false;
	this->data.chamsHoldKey = SDL_SCANCODE_UNKNOWN;
	this->data.chamsToggleKey = SDL_SCANCODE_UNKNOWN;
	for (size_t i = 0; i < ConfigData::CHAMS_TYPE_INVAL; ++i) {
		auto chamsData = &this->data.chamsData[i];
		for (size_t j = 0; j < CHAMS_COUNT; ++j) {
			(*chamsData)[j].enable = false;
			(*chamsData)[j].wireframe = false;
			(*chamsData)[j].ignoreZ = false;
			(*chamsData)[j].selfIllum = false;
			(*chamsData)[j].mat = ConfigData::CHAMS_MAT_FLAT;
			for (size_t k = 0; k < sizeof((*chamsData)[j].color) / sizeof((*chamsData)[j].color[0]); ++k)
				(*chamsData)[j].color[k] = 1.0f;
		}
	}
	this->data.espSnaplineEnable = false;
	this->data.espSnaplinePos = ConfigData::ESP_SNAPLINE_BOTTOM;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espSnaplineColorTeam); ++i)
		this->data.espSnaplineColorTeam[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espSnaplineColorEnemy); ++i)
		this->data.espSnaplineColorEnemy[i] = 1.0f;
}

void Config::Save(std::string name)
{
	if (name == "") {
		RONIX_LOG("Empty config name\n");
		return;
	}

	std::string abspath = this->MakePath(name);
	std::ofstream fs = std::ofstream(abspath);
	if (!fs.is_open()) {
		RONIX_LOG("Unable to save config: %s\n", name.c_str());
		return;
	}

	nlohmann::json json_obj = nlohmann::json();

	json_write(json_obj, "bunnyhopEnable", this->data.bunnyhopEnable);
	json_write(json_obj, "autoStrafeEnable", this->data.autoStrafeEnable);
	json_write(json_obj, "autoStrafeSilent", this->data.autoStrafeSilent);
	json_write(json_obj, "autoStrafeRage", this->data.autoStrafeRage);
	json_write(json_obj, "autoStrafeHoldKey", this->data.autoStrafeHoldKey);
	json_write(json_obj, "autoStrafeToggleKey", this->data.autoStrafeToggleKey);
	json_write(json_obj, "chamsEnable", this->data.chamsEnable);
	json_write(json_obj, "chamsHoldKey", this->data.chamsHoldKey);
	json_write(json_obj, "chamsToggleKey", this->data.chamsToggleKey);
	json_write(json_obj, "chamsData", this->data.chamsData);

	fs << json_obj.dump();
	fs.close();
	RONIX_LOG("Saved config: %s\n", abspath.c_str());
}

void Config::Load(std::string name)
{
	if (name == "") {
		RONIX_LOG("Empty config name\n");
		return;
	}

	std::string abspath = this->MakePath(name);
	std::ifstream fs = std::ifstream(abspath);
	if (!fs.is_open()) {
		RONIX_LOG("Unable to load config: %s\n", name.c_str());
		return;
	}
	
	std::stringstream filebuf;
	filebuf << fs.rdbuf();
	fs.close();

	nlohmann::json json_obj = nlohmann::json::parse(filebuf.str());

	json_read(json_obj, "bunnyhopEnable", this->data.bunnyhopEnable);
	json_read(json_obj, "autoStrafeEnable", this->data.autoStrafeEnable);
	json_read(json_obj, "autoStrafeSilent", this->data.autoStrafeSilent);
	json_read(json_obj, "autoStrafeRage", this->data.autoStrafeRage);
	json_read(json_obj, "autoStrafeHoldKey", this->data.autoStrafeHoldKey);
	json_read(json_obj, "autoStrafeToggleKey", this->data.autoStrafeToggleKey);
	json_read(json_obj, "chamsEnable", this->data.chamsEnable);
	json_read(json_obj, "chamsHoldKey", this->data.chamsHoldKey);
	json_read(json_obj, "chamsToggleKey", this->data.chamsToggleKey);
	json_read(json_obj, "chamsData", this->data.chamsData);

	RONIX_LOG("Loaded config: %s\n", abspath.c_str());
}
