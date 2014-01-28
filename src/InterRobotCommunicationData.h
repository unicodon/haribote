/*******************************************************************************
*
* InterRobotCommunicationData.h
*
* Broadcasted data structure and constants
*
*
*******************************************************************************/

#ifndef _InterRobotCommunicationData_h_DEFINED
#define _InterRobotCommunicationData_h_DEFINED

// data structure version number
// up version number at chnaging data structure
#define ROBOT_COMMUNICATION_STRUCT_VERSION              5

// data structure header
#define ROBOT_COMMUNICATION_STRUCT_HEADER               "TAra"

struct InterRobotCommunicationData
{
	//‚Æ‚è‚ ‚¦‚¸‚±‚ñ‚¾‚¯
	char   header[4];
    uint32 version; 
	uint32 robotID;
	uint32 team_number;
	double x_val;
	double y_val;
	double t_val;
	unsigned char ball_visible;
	double ball_distance;
	double ball_direction;
	unsigned char ball_near;
	unsigned char penalty;
	unsigned char last_approach;
};



#endif
