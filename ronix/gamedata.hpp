#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <pch.hpp>
#include "cstrike.hpp"

struct PlayerData {
	enum BoneIndex {
		CORE = 0,
		LPELVIS = 1,
		LKNEE = 2,
		LANKLE = 3,
		LFOOT = 4,
		RPELVIS = 5,
		RKNEE = 6,
		RANKLE = 7,
		RFOOT = 8,
		CHEST = 12,
		NECK = 14,
		LSHOULDER = 16,
		LELBOW = 17,
		LHAND = 18,
		RSHOULDER = 29,
		RELBOW = 30,
		RHAND = 31,
		HEAD = 49,
	};

	bool isValid;
	bool isVisible;
	bool behind;
	int team;
	Vector pos2d;
	Vector pos3d;
	Vector headpos2d;
	Vector headpos3d;
	matrix3x4_t boneMatrix[256];
	Vector boneMatrixPos2d[256];
};

struct GameData {
	std::vector<PlayerData> players;
	uint screenRes[2];
	int localPlayerTeam;
	void Update();
};

#endif
