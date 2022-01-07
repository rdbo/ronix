#include <config.hpp>
#include <ronix.hpp>
#include <dirent.h>
#include <sys/stat.h>

static void write_var(nlohmann::json &json_obj, const char *varname, bool value)
{
	json_obj[varname] = value;
}

static void write_var(nlohmann::json &json_obj, const char *varname, SDL_Scancode value)
{
	json_obj[varname] = static_cast<int>(value);
}

static void write_var(nlohmann::json &json_obj, const char *varname, ConfigData::KeyType value)
{
	json_obj[varname] = static_cast<int>(value);
}

static void read_var(nlohmann::json &json_obj, const char *varname, bool &var)
{
	if (json_obj[varname].is_boolean())
		var = json_obj[varname];
}

static void read_var(nlohmann::json &json_obj, const char *varname, SDL_Scancode &var)
{
	if (json_obj[varname].is_number_integer())
		var = static_cast<SDL_Scancode>(json_obj[varname]);
}

static void read_var(nlohmann::json &json_obj, const char *varname, ConfigData::KeyType &var)
{
	if (json_obj[varname].is_number_integer())
		var = static_cast<ConfigData::KeyType>(json_obj[varname]);
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
	this->data.autoStrafeKey = SDL_SCANCODE_UNKNOWN;
	this->data.autoStrafeKeyType = ConfigData::KEYTYPE_HOLD;
}

void Config::Save(std::string name)
{
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
	write_var(json_obj, "autoStrafeKey", this->data.autoStrafeKey);
	write_var(json_obj, "autoStrafeKeyType", this->data.autoStrafeKeyType);

	fs << json_obj.dump();
	fs.close();
	RONIX_LOG("Saved config: %s\n", abspath.c_str());
}

void Config::Load(std::string name)
{
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
	read_var(json_obj, "autoStrafeKey", this->data.autoStrafeKey);
	read_var(json_obj, "autoStrafeKeyType", this->data.autoStrafeKeyType);

	RONIX_LOG("Loaded config: %s\n", abspath.c_str());
}
