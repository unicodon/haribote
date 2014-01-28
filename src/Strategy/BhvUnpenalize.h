#ifndef BHVUNPENALIZE_H
#define BHVUNPENALIZE_H

#include "Behavior.h"

class BhvUnpenalize : public Behavior{
public:
	//static bool now_release;
	//static bool now_recover;
	//static bool now_penalize;
	BhvUnpenalize();
	void Execute();
	void Initialize();

	//static unsigned int cnt_penalize;
private:
};

#endif
