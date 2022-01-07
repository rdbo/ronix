#include <config.hpp>
#include <ronix.hpp>
#include <dirent.h>
#include <sys/stat.h>

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
	this->data.autoStrafeSilent = true;
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
	json_obj["bunnyhopEnable"] = this->data.bunnyhopEnable;
	json_obj["autoStrafeEnable"] = this->data.autoStrafeEnable;
	json_obj["autoStrafeSilent"] = this->data.autoStrafeSilent;

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
	this->data.bunnyhopEnable = json_obj["bunnyhopEnable"];
	this->data.autoStrafeEnable = json_obj["autoStrafeEnable"];
	this->data.autoStrafeSilent = json_obj["autoStrafeSilent"];

	RONIX_LOG("Loaded config: %s\n", abspath.c_str());
}
