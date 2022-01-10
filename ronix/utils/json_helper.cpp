#include "json_helper.hpp"

void json_write(nlohmann::json &json_obj, std::string varname, bool value)
{
	json_obj[varname] = value;
}

void json_write(nlohmann::json &json_obj, std::string varname, int value)
{
	json_obj[varname] = value;
}

void json_write(nlohmann::json &json_obj, std::string varname, float value)
{
	json_obj[varname] = value;
}

void json_write(nlohmann::json &json_obj, std::string varname, float value[], size_t length)
{
	for (size_t i = 0; i < length; ++i)
		json_write(json_obj[varname], std::to_string(i), value[i]);
}

void json_write(nlohmann::json &json_obj, std::string varname, SDL_Scancode value)
{
	json_write(json_obj, varname, static_cast<int>(value));
}

void json_write(nlohmann::json &json_obj, std::string varname, ConfigData::ChamsMat value)
{
	json_write(json_obj, varname, static_cast<int>(value));
}

void json_write(nlohmann::json &json_obj, std::string varname, ConfigData::ChamsData (&chamsData)[ConfigData::CHAMS_TYPE_INVAL][CHAMS_COUNT])
{
	for (size_t i = 0; i < ConfigData::CHAMS_TYPE_INVAL; ++i) {
		for (size_t j = 0; j < CHAMS_COUNT; ++j) {
			json_write(json_obj[varname][i][j], "enable", chamsData[i][j].enable);
			json_write(json_obj[varname][i][j], "wireframe", chamsData[i][j].wireframe);
			json_write(json_obj[varname][i][j], "ignoreZ", chamsData[i][j].ignoreZ);
			json_write(json_obj[varname][i][j], "selfIllum", chamsData[i][j].selfIllum);
			json_write(json_obj[varname][i][j], "mat", chamsData[i][j].mat);
			json_write(json_obj[varname][i][j], "color", chamsData[i][j].color);
		}
	}
}

void json_read(nlohmann::json &json_obj, std::string varname, bool &var)
{
	if (json_obj.contains(varname) && json_obj[varname].is_boolean())
		var = json_obj[varname];
}

void json_read(nlohmann::json &json_obj, std::string varname, int &var)
{
	if (json_obj.contains(varname) && json_obj[varname].is_number_integer())
		var = json_obj[varname];
}

void json_read(nlohmann::json &json_obj, std::string varname, float &var)
{
	if (json_obj.contains(varname) && json_obj[varname].is_number_float())
		var = json_obj[varname];
}

void json_read(nlohmann::json &json_obj, std::string varname, float (&var)[], size_t length)
{
	for (size_t i = 0; i < length; ++i)
		json_read(json_obj[varname], std::to_string(i), var[i]);
}

void json_read(nlohmann::json &json_obj, std::string varname, SDL_Scancode &var)
{
	json_read(json_obj, varname, reinterpret_cast<int &>(var));
}

void json_read(nlohmann::json &json_obj, std::string varname, ConfigData::ChamsMat &var)
{
	json_read(json_obj, varname, reinterpret_cast<int &>(var));
}

void json_read(nlohmann::json &json_obj, std::string varname, ConfigData::ChamsData (&chamsData)[ConfigData::CHAMS_TYPE_INVAL][CHAMS_COUNT])
{
	if (!json_obj.contains(varname))
		return;
	
	for (size_t i = 0; i < ConfigData::CHAMS_TYPE_INVAL; ++i) {
		/*
		if (!json_obj[varname].contains(std::to_string(i)))
			break;
		*/
		for (size_t j = 0; j < CHAMS_COUNT; ++j) {
			/*
			if (!json_obj[varname][i].contains(j))
				break;
			*/
			json_read(json_obj[varname][i][j], "enable", chamsData[i][j].enable);
			json_read(json_obj[varname][i][j], "wireframe", chamsData[i][j].wireframe);
			json_read(json_obj[varname][i][j], "ignoreZ", chamsData[i][j].ignoreZ);
			json_read(json_obj[varname][i][j], "selfIllum", chamsData[i][j].selfIllum);
			json_read(json_obj[varname][i][j], "mat", chamsData[i][j].mat);
			json_read(json_obj[varname][i][j], "color", chamsData[i][j].color, RONIX_ARRLEN(chamsData[i][j].color));
		}
	}
}
