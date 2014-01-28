#ifndef BHVGOALIE2006_H
#define BHVGOALIE2006_H

#include "Behavior.h"

enum GOALIE2006_STATE
{
	RETURN_GOALIE_POSITION,
	RETURN_GOAL_MOUTH,
	GOALIE_ADJUST_POSITION,
	GOALIE_WAIT,
	GOALIE_CHASE,
	GOALIE_CLEAR,
	GOALIE_GO_BACK
};



class BhvGoalie2006 : public Behavior
{
public:
	void Execute();
	BhvGoalie2006();
	void Reset();
	void ResetForPenalize();
private:
	bool savingflag;
	int savingmotion;
	void SaveMotion();
	void SetSaveMotion(double y, double t);
	void ReturnGoalMouth();
	void AdjustPosition();
	void Wait();
	void ChaseBall();
	void Clear();
	bool Saving();
	void ReturnToGoaliePosition();
	void GoBack();

	bool CheckBallPosition();
	bool InSpot(int x, int y);
	bool InSpotTheta(int x, int y, int theta);
	int gmcnt;
//	void MoveToPosition();
	GOALIE2006_STATE mode; 
};

#endif
