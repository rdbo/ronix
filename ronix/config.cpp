#include <config.hpp>
#include <ronix.hpp>
#include <dirent.h>
#include <sys/stat.h>

static void write_var(nlohmann::json &json_obj, const char *varname, bool value)
{
	json_obj[varname] = value;
}

static void write_var(nlohmann::json &json_obj, const char *varname, int value)
{
	json_obj[varname] = value;
}

static void write_var(nlohmann::json &json_obj, const char *varname, SDL_Scancode value)
{
	write_var(json_obj, varname, static_cast<int>(value));
}

static void write_var(nlohmann::json &json_obj, const char *varname, ConfigData::ChamsData (&chamsData)[ConfigData::CHAMS_TYPE_INVAL][CHAMS_COUNT])
{
	for (size_t i = 0; i < ConfigData::CHAMS_TYPE_INVAL; ++i) {
		for (size_t j = 0; j < CHAMS_COUNT; ++j) {
			json_obj[varname][i][j]["enable"] = chamsData[i][j].enable;
			json_obj[varname][i][j]["wireframe"] = chamsData[i][j].wireframe;
			json_obj[varname][i][j]["ignoreZ"] = chamsData[i][j].ignoreZ;
			json_obj[varname][i][j]["selfIllum"] = chamsData[i][j].selfIllum;
			json_obj[varname][i][j]["mat"] = static_cast<int>(chamsData[i][j].mat);
			json_obj[varname][i][j]["color"] = chamsData[i][j].color;
		}
	}
}

static void read_var(nlohmann::json &json_obj, const char *varname, bool &var)
{
	if (json_obj[varname].is_boolean())
		var = json_obj[varname];
}

static void read_var(nlohmann::json &json_obj, const char *varname, int &var)
{
	if (json_obj[varname].is_number_integer())
		var = json_obj[varname];
}

static void read_var(nlohmann::json &json_obj, const char *varname, SDL_Scancode &var)
{
	read_var(json_obj, varname, reinterpret_cast<int &>(var));
}

static void read_var(nlohmann::json &json_obj, const char *varname, ConfigData::ChamsData (&chamsData)[ConfigData::CHAMS_TYPE_INVAL][CHAMS_COUNT])
{
	for (size_t i = 0; i < ConfigData::CHAMS_TYPE_INVAL; ++i) {
		for (size_t j = 0; j < CHAMS_COUNT; ++j) {
			chamsData[i][j].enable = json_obj[varname][i][j]["enable"];
			chamsData[i][j].wireframe = json_obj[varname][i][j]["wireframe"];
			chamsData[i][j].ignoreZ = json_obj[varname][i][j]["ignoreZ"];
			chamsData[i][j].selfIllum = json_obj[varname][i][j]["selfIllum"];
			chamsData[i][j].mat = static_cast<ConfigData::ChamsMat>(json_obj[varname][i][j]["mat"]);
			for (size_t k = 0; k < sizeof(chamsData[i][j].color) / sizeof(chamsData[i][j].color[0]); ++k)
				chamsData[i][j].color[k] = json_obj[varname][i][j]["color"][k];
		}
	}
}

Config::Config()
{
	this->path = std::string("/home/") + getlogin() + "/.ronix";
	DIR *dir;
	if (!(dir = opendir(this->path.c_str())))
		mkdir(this->path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	else
		closedir(dir);
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

	write_var(json_obj, "bunnyhopEnable", this->data.bunnyhopEnable);
	write_var(json_obj, "autoStrafeEnable", this->data.autoStrafeEnable);
	write_var(json_obj, "autoStrafeSilent", this->data.autoStrafeSilent);
	write_var(json_obj, "autoStrafeRage", this->data.autoStrafeRage);
	write_var(json_obj, "autoStrafeHoldKey", this->data.autoStrafeHoldKey);
	write_var(json_obj, "autoStrafeToggleKey", this->data.autoStrafeToggleKey);
	write_var(json_obj, "chamsEnable", this->data.chamsEnable);
	write_var(json_obj, "chamsHoldKey", this->data.chamsHoldKey);
	write_var(json_obj, "chamsToggleKey", this->data.chamsToggleKey);
	write_var(json_obj, "chamsData", this->data.chamsData);

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

	read_var(json_obj, "bunnyhopEnable", this->data.bunnyhopEnable);
	read_var(json_obj, "autoStrafeEnable", this->data.autoStrafeEnable);
	read_var(json_obj, "autoStrafeSilent", this->data.autoStrafeSilent);
	read_var(json_obj, "autoStrafeRage", this->data.autoStrafeRage);
	read_var(json_obj, "autoStrafeHoldKey", this->data.autoStrafeHoldKey);
	read_var(json_obj, "autoStrafeToggleKey", this->data.autoStrafeToggleKey);
	read_var(json_obj, "chamsEnable", this->data.chamsEnable);
	read_var(json_obj, "chamsHoldKey", this->data.chamsHoldKey);
	read_var(json_obj, "chamsToggleKey", this->data.chamsToggleKey);
	read_var(json_obj, "chamsData", this->data.chamsData);

	RONIX_LOG("Loaded config: %s\n", abspath.c_str());
}
