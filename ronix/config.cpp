#include <config.hpp>
#include <ronix.hpp>
#include "utils/json_helper.hpp"

bool Keybind::IsSet()
{
	return this->key != SDL_SCANCODE_UNKNOWN;
}

bool Keybind::IsPressed()
{
	if (!this->IsSet())
		return false;

	int index = this->key;
	if (this->type == MOUSE)
		index += KEYS_MOUSE_INDEX;
	return Ronix::Data::keys[index] == SDL_KEYDOWN;
}

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
	Keybind empty_kb = { Keybind::KEYBOARD, SDL_SCANCODE_UNKNOWN };
	this->data.bunnyhopEnable = false;
	this->data.autoStrafeEnable = false;
	this->data.autoStrafeSilent = false;
	this->data.autoStrafeRage = false;
	this->data.autoStrafeHoldKey = empty_kb;
	this->data.autoStrafeToggleKey = empty_kb;

	this->data.chamsEnable = false;
	this->data.chamsHoldKey = empty_kb;
	this->data.chamsToggleKey = empty_kb;
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
	this->data.espSnaplineHoldKey = empty_kb;
	this->data.espSnaplineToggleKey = empty_kb;
	this->data.espSnaplinePos = ConfigData::ESP_SNAPLINE_POS_BOTTOM;
	this->data.espSnaplineType = ConfigData::ESP_SNAPLINE_TYPE_FOOT;
	this->data.espSnaplineThickness = 2.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espSnaplineTeamVisColor); ++i)
		this->data.espSnaplineTeamVisColor[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espSnaplineEnemyVisColor); ++i)
		this->data.espSnaplineEnemyVisColor[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espSnaplineTeamInvisColor); ++i)
		this->data.espSnaplineTeamInvisColor[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espSnaplineEnemyInvisColor); ++i)
		this->data.espSnaplineEnemyInvisColor[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espSnaplineOutlineColor) - 1; ++i)
		this->data.espSnaplineOutlineColor[i] = 0.0f;
	this->data.espSnaplineOutlineColor[3] = 1.0f;

	this->data.triggerbotEnable = false;
	this->data.triggerbotHoldKey = empty_kb;
	this->data.triggerbotToggleKey = empty_kb;
	this->data.triggerbotDelay = 0.0f;
	
	this->data.espBoxEnable = false;
	this->data.espBoxHoldKey = empty_kb;
	this->data.espBoxToggleKey = empty_kb;
	this->data.espBoxThickness = 2.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espBoxTeamVisColor); ++i)
		this->data.espBoxTeamVisColor[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espBoxEnemyVisColor); ++i)
		this->data.espBoxEnemyVisColor[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espBoxTeamInvisColor); ++i)
		this->data.espBoxTeamInvisColor[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espBoxEnemyInvisColor); ++i)
		this->data.espBoxEnemyInvisColor[i] = 1.0f;
	for (size_t i = 0; i < RONIX_ARRLEN(this->data.espBoxOutlineColor) - 1; ++i)
		this->data.espBoxOutlineColor[i] = 0.0f;
	this->data.espBoxOutlineColor[3] = 1.0f;
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
	json_write(json_obj, "espSnaplineEnable", this->data.espSnaplineEnable);
	json_write(json_obj, "espSnaplineHoldKey", this->data.espSnaplineHoldKey);
	json_write(json_obj, "espSnaplineToggleKey", this->data.espSnaplineToggleKey);
	json_write(json_obj, "espSnaplinePos", this->data.espSnaplinePos);
	json_write(json_obj, "espSnaplineType", this->data.espSnaplineType);
	json_write(json_obj, "espSnaplineThickness", this->data.espSnaplineThickness);
	json_write(json_obj, "espSnaplineTeamVisColor", this->data.espSnaplineTeamVisColor, RONIX_ARRLEN(this->data.espSnaplineTeamVisColor));
	json_write(json_obj, "espSnaplineEnemyVisColor", this->data.espSnaplineEnemyVisColor, RONIX_ARRLEN(this->data.espSnaplineEnemyVisColor));
	json_write(json_obj, "espSnaplineTeamInvisColor", this->data.espSnaplineTeamInvisColor, RONIX_ARRLEN(this->data.espSnaplineTeamInvisColor));
	json_write(json_obj, "espSnaplineEnemyInvisColor", this->data.espSnaplineEnemyInvisColor, RONIX_ARRLEN(this->data.espSnaplineEnemyInvisColor));
	json_write(json_obj, "espSnaplineOutlineColor", this->data.espSnaplineOutlineColor, RONIX_ARRLEN(this->data.espSnaplineOutlineColor));
	json_write(json_obj, "triggerbotEnable", this->data.triggerbotEnable);
	json_write(json_obj, "triggerbotHoldKey", this->data.triggerbotHoldKey);
	json_write(json_obj, "triggerbotToggleKey", this->data.triggerbotToggleKey);
	json_write(json_obj, "triggerbotDelay", this->data.triggerbotDelay);
	json_write(json_obj, "espBoxEnable", this->data.espBoxEnable);
	json_write(json_obj, "espBoxHoldKey", this->data.espBoxHoldKey);
	json_write(json_obj, "espBoxToggleKey", this->data.espBoxToggleKey);
	json_write(json_obj, "espBoxThickness", this->data.espBoxThickness);
	json_write(json_obj, "espBoxTeamVisColor", this->data.espBoxTeamVisColor, RONIX_ARRLEN(this->data.espBoxTeamVisColor));
	json_write(json_obj, "espBoxEnemyVisColor", this->data.espBoxEnemyVisColor, RONIX_ARRLEN(this->data.espBoxEnemyVisColor));
	json_write(json_obj, "espBoxTeamInvisColor", this->data.espBoxTeamInvisColor, RONIX_ARRLEN(this->data.espBoxTeamInvisColor));
	json_write(json_obj, "espBoxEnemyInvisColor", this->data.espBoxEnemyInvisColor, RONIX_ARRLEN(this->data.espBoxEnemyInvisColor));
	json_write(json_obj, "espBoxOutlineColor", this->data.espBoxOutlineColor, RONIX_ARRLEN(this->data.espBoxOutlineColor));

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

	this->Reset();
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
	json_read(json_obj, "espSnaplineEnable", this->data.espSnaplineEnable);
	json_read(json_obj, "espSnaplineHoldKey", this->data.espSnaplineHoldKey);
	json_read(json_obj, "espSnaplineToggleKey", this->data.espSnaplineToggleKey);
	json_read(json_obj, "espSnaplinePos", this->data.espSnaplinePos);
	json_read(json_obj, "espSnaplineType", this->data.espSnaplineType);
	json_read(json_obj, "espSnaplineThickness", this->data.espSnaplineThickness);
	json_read(json_obj, "espSnaplineTeamVisColor", this->data.espSnaplineTeamVisColor, RONIX_ARRLEN(this->data.espSnaplineTeamVisColor));
	json_read(json_obj, "espSnaplineEnemyVisColor", this->data.espSnaplineEnemyVisColor, RONIX_ARRLEN(this->data.espSnaplineEnemyVisColor));
	json_read(json_obj, "espSnaplineTeamInvisColor", this->data.espSnaplineTeamInvisColor, RONIX_ARRLEN(this->data.espSnaplineTeamInvisColor));
	json_read(json_obj, "espSnaplineEnemyInvisColor", this->data.espSnaplineEnemyInvisColor, RONIX_ARRLEN(this->data.espSnaplineEnemyInvisColor));
	json_read(json_obj, "espSnaplineOutlineColor", this->data.espSnaplineOutlineColor, RONIX_ARRLEN(this->data.espSnaplineOutlineColor));
	json_read(json_obj, "triggerbotEnable", this->data.triggerbotEnable);
	json_read(json_obj, "triggerbotHoldKey", this->data.triggerbotHoldKey);
	json_read(json_obj, "triggerbotToggleKey", this->data.triggerbotToggleKey);
	json_read(json_obj, "triggerbotDelay", this->data.triggerbotDelay);
	json_read(json_obj, "espBoxEnable", this->data.espBoxEnable);
	json_read(json_obj, "espBoxHoldKey", this->data.espBoxHoldKey);
	json_read(json_obj, "espBoxToggleKey", this->data.espBoxToggleKey);
	json_read(json_obj, "espBoxThickness", this->data.espBoxThickness);
	json_read(json_obj, "espBoxTeamVisColor", this->data.espBoxTeamVisColor, RONIX_ARRLEN(this->data.espBoxTeamVisColor));
	json_read(json_obj, "espBoxEnemyVisColor", this->data.espBoxEnemyVisColor, RONIX_ARRLEN(this->data.espBoxEnemyVisColor));
	json_read(json_obj, "espBoxTeamInvisColor", this->data.espBoxTeamInvisColor, RONIX_ARRLEN(this->data.espBoxTeamInvisColor));
	json_read(json_obj, "espBoxEnemyInvisColor", this->data.espBoxEnemyInvisColor, RONIX_ARRLEN(this->data.espBoxEnemyInvisColor));
	json_read(json_obj, "espBoxOutlineColor", this->data.espBoxOutlineColor, RONIX_ARRLEN(this->data.espBoxOutlineColor));

	RONIX_LOG("Loaded config: %s\n", abspath.c_str());
}
