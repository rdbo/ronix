#ifndef JSON_HELPER_HPP
#define JSON_HELPER_HPP

#include <ronix.hpp>

void json_write(nlohmann::json &json_obj, std::string varname, bool value);
void json_write(nlohmann::json &json_obj, std::string varname, int value);
void json_write(nlohmann::json &json_obj, std::string varname, float value);
void json_write(nlohmann::json &json_obj, std::string varname, float value[], size_t length);
void json_write(nlohmann::json &json_obj, std::string varname, Keybind value);
void json_write(nlohmann::json &json_obj, std::string varname, ConfigData::ChamsMat value);
void json_write(nlohmann::json &json_obj, std::string varname, ConfigData::ChamsData (&chamsData)[ConfigData::CHAMS_TYPE_INVAL][CHAMS_COUNT]);
void json_write(nlohmann::json &json_obj, std::string varname, ConfigData::EspSnaplinePos value);
void json_write(nlohmann::json &json_obj, std::string varname, ConfigData::EspSnaplineType value);
void json_write(nlohmann::json &json_obj, std::string varname, ConfigData::AimbotType value);

void json_read(nlohmann::json &json_obj, std::string varname, bool &var);
void json_read(nlohmann::json &json_obj, std::string varname, int &var);
void json_read(nlohmann::json &json_obj, std::string varname, float &var);
void json_read(nlohmann::json &json_obj, std::string varname, float var[], size_t length);
void json_read(nlohmann::json &json_obj, std::string varname, Keybind &var);
void json_read(nlohmann::json &json_obj, std::string varname, ConfigData::ChamsMat &var);
void json_read(nlohmann::json &json_obj, std::string varname, ConfigData::ChamsData (&chamsData)[ConfigData::CHAMS_TYPE_INVAL][CHAMS_COUNT]);
void json_read(nlohmann::json &json_obj, std::string varname, ConfigData::EspSnaplinePos &var);
void json_read(nlohmann::json &json_obj, std::string varname, ConfigData::EspSnaplineType &var);
void json_read(nlohmann::json &json_obj, std::string varname, ConfigData::AimbotType &var);

#endif
