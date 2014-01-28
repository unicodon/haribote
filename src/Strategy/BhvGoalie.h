#ifndef BHVGOALIE_H
#define BHVGOALIE_H

#include "Behavior.h"

enum GOALIE_STATE{
	RETURN_GOALAREA,ADJUST_POSITION,WAIT,CHASE,CLEAR,RETURN_GOALMOUTH,GO_BACK
};



class BhvGoalie : public Behavior{
public:
	void Execute();
	BhvGoalie();
	void Reset();
	void ResetForPenalize();
private:
	bool savingflag;
	int savingmotion;
	void SaveMotion();
	void SetSaveMotion(double y, double t);
	void ReturnToGoalArea();
	void AdjustPosition();
	void Wait();
	void ChaseBall();
	void Clear();
	void Saving();
	void ReturnGoalMouth();
	void GoBack();
	int gmcnt;
//	void MoveToPosition();
	GOALIE_STATE mode; 
};

#endif
