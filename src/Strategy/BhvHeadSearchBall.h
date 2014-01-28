#ifndef BHV_SEARCH_BALL_H
#define BHV_SEARCH_BALL_H

#include "Behavior.h"

/*
頭だけを動かすBehaviorです。
ボールロスト時:ボールを捜す
ボールが見える:トラッキング
ボールを見失う:しばらくトラッキング
*/


class BhvHeadSearchBall : public Behavior{
public:
	void Execute();
};

#endif
