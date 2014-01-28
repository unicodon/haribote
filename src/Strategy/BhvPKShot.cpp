#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"

BhvPKShot::BhvPKShot()
{
}

void
BhvPKShot::Execute()
{
	switch(shotDirection)
	{
	case PK_SHOT_LEFT:
		SetMotion("HOLD_LEFT90");
		break;
	case PK_SHOT_RIGHT:
		SetMotion("HOLD_RIGHT90");
		break;
	case PK_SHOT_CENTER:
	default:
		SetMotion("SHOOT_FRONT_PK");
		break;
	}
}

void BhvPKShot::SetDirection(ShotDirection input)
{
	shotDirection = input;
}
