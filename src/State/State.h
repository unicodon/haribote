#ifndef STATE_H
#define STATE_H

#include "MyRobot.h"
#include "GameState.h"
#include "FieldState.h"
#include "../Vision/VisionUtil.h"
#include "../InterRobotCommunicationData.h"
#include "TeamMate.h"
#include "../MCL/LineData.h"
#include "../MCL/BallPDF.h"

struct State{
public:
	LocalizationData localizationData[SELECTED_SAMPLE_NUMBER];//for sample_viewer
	MyRobot self;
	GameState game;
	ColorClusterParam color_cluster_param[1];

	FieldState field;//スペースとかランドマークとかゴールとか

	BallInfo ball;//ボール情報
	BallInfo virtual_ball;//ボール情報

	double	goalDirection;//打つべき方向
	bool	goalVisible;
	
	BallPDF ballpdf;

	bool	last_approach;//ボールへ最後の接近中かどうか


	double	goalDirectionOfOurs;//自分のゴール
	bool	goalVisibleOfOurs;

	bool lineVisible;
	LineInformation lineinfo;

	bool myGoalVisible;
	bool enemyGoalVisible;
	GoalInformation myGoalInfo;
	GoalInformation enemyGoalInfo;


	TeamMate team_mate[5];//チームメートから送られてきたデータを4台分用意[0]は使わない


	static byte* CRT[1];//ColorReliabilityTable

	SoundPlayState soundPlayState;//音
};

extern State state;

#endif
