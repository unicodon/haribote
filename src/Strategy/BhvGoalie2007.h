#ifndef BHVGOALIE2007_H
#define BHVGOALIE2007_H

#include "Behavior.h"

enum GOALIE2007_STATE{
	GOALIE2007_WAIT,
	GOALIE2007_CHASE_BALL,
	GOALIE2007_CLEAR,
	GOALIE2007_RETURN,
	GOALIE2007_SAVING,
};

class MoveToPosPotential
{
public:
	int		GetWalk();
	double	GetValue(double x, double y, double tt);
	void	GetDest(int walk, double &dest_x, double &dest_y, double &dest_t);
};

class BhvGoalie2007 : public Behavior{
public:
	void Execute();
	BhvGoalie2007();
	void Reset();
	void ResetForPenalize();

private:
	bool savingflag;
	int savingmotion;
	void CheckSaving();
	void SaveMotion();
	void SetSaveMotion(double y, double t);

	bool CheckBallPosition();

	void Wait();
	void ChaseBall();
	void Clear();
	void Return();
	bool Saving();


	double dest_x;
	double dest_y;
	double dest_t;


	GOALIE2007_STATE mode; 
};

#endif
