#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <pch.hpp>
#include "cstrike.hpp"

struct PlayerData {
	bool isValid;
	bool isVisible;
	bool behind;
	int team;
	Vector pos2d;
	Vector pos3d;
};

struct GameData {
	std::vector<PlayerData> players;
	uint screenRes[2];
	int localPlayerTeam;
};

#endif
