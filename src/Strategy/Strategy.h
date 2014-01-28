#ifndef STRATEGY_H
#define STRATEGY_H

class Behavior;

struct IsAlreadySet{
	bool isMotionSet;
	bool isWalkSet;
	bool isHeadSet;
	bool isTailSet;
	bool isFaceSet;
};

class Strategy{
private:
	static Behavior* reservedBhv;

	//タッチセンサでゲーム状態遷移
	static void TouchSensorOperation();
	static void OnHeadTouch();
	static void OnHeadTouch3sec();
	static void OnHeadRelease();
	static void OnBackTouch(int i);
	static void OnBackTouch1sec();
	static void IndicateGameStateLED();
public:
	Strategy();
	~Strategy();

	static IsAlreadySet isSet;

	static void SetReservedBehavior(Behavior* bhv);	

	static bool Execute();
};

#endif
