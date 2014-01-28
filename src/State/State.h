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

	FieldState field;//�X�y�[�X�Ƃ������h�}�[�N�Ƃ��S�[���Ƃ�

	BallInfo ball;//�{�[�����
	BallInfo virtual_ball;//�{�[�����

	double	goalDirection;//�łׂ�����
	bool	goalVisible;
	
	BallPDF ballpdf;

	bool	last_approach;//�{�[���֍Ō�̐ڋߒ����ǂ���


	double	goalDirectionOfOurs;//�����̃S�[��
	bool	goalVisibleOfOurs;

	bool lineVisible;
	LineInformation lineinfo;

	bool myGoalVisible;
	bool enemyGoalVisible;
	GoalInformation myGoalInfo;
	GoalInformation enemyGoalInfo;


	TeamMate team_mate[5];//�`�[�����[�g���瑗���Ă����f�[�^��4�䕪�p��[0]�͎g��Ȃ�


	static byte* CRT[1];//ColorReliabilityTable

	SoundPlayState soundPlayState;//��
};

extern State state;

#endif
