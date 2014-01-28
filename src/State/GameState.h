#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../config.h"
#include "../RoboCupGameControlData.h"

struct GameState
{
	int team_number;
	int robot_id;
	GAME_STATE game_state;
	HALF_TIME first_half;
	TEAM_COLOR team_color;
	int point_me;
	int point_enemy;
	TEAM_COLOR kikkoff_team;
	PENALTY penalty;
	RoboCupGameControlData* communication_data;
};

#endif //GAME_STATE_H
