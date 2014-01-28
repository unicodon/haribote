#ifndef BEHAVIORLIST_H
#define BEHAVIORLIST_H

//ここにBehaviorのヘッダファイルを
//列記してください

#include "BhvReady.h"
#include "BhvSet.h"
#include "BhvSample.h"
#include "BhvBallApproach.h"
#include "BhvDPBallApproach.h"
#include "BhvLongShoot.h"
#include "BhvHeadSearchBall.h"
#include "BhvSearchBall.h"
#include "BhvMoveToPosition.h"
#include "BhvMoveToPosDP.h"
//#include "BhvParaWalk.h"

#include "BhvBallAppDirect.h"
#include "BhvGoBackCourt.h"
#include "BhvSupport.h"
#include "BhvGoToMyArea.h"
#include "BhvGoalie.h"
#include "BhvGoalie2006.h"
#include "BhvGoalie2007.h"
#include "BhvShot.h"

#include "BhvPKShotPreparation.h"
#include "BhvPKShot.h"
#include "BhvLeavingField.h"
#include "BhvWalkingHeading.h"

#include "BhvDrible.h"
#include "BhvLastApproach.h"
#include "BhvHoldShot.h"
#include "BhvShotChoose.h"

#include "BhvColorCooperation.h"
#include "BhvUnpenalize.h"

//この下にBehaviorのインスタンスを宣言してください．
//BehaviorList.cppにも同じものを定義してください．

//MakeFileにも記述を忘れずに

extern BhvSet bhvSet;
extern BhvReady bhvReady;
extern BhvSample bhvSample;
extern BhvBallApproach bhvBallApproach;
extern BhvDPBallApproach bhvDPBallApproach;
extern BhvLongShoot bhvLongShoot;
extern BhvHeadSearchBall bhvHeadSearchBall;
extern BhvSearchBall bhvSearchBall;
extern BhvMoveToPosition bhvMoveToPosition;
extern BhvMoveToPosDP bhvMoveToPosDP;
//extern BhvParaWalk bhvParaWalk;

extern BhvBallAppDirect bhvBallAppDirect;
extern BhvGoBackCourt bhvGoBackCourt;
extern BhvSupport bhvSupport;
extern BhvGoToMyArea bhvGoToMyArea;
extern BhvGoalie bhvGoalie;
extern BhvGoalie2006 bhvGoalie2006;
extern BhvGoalie2007 bhvGoalie2007;
extern BhvShot bhvShot;

extern BhvPKShotPreparation bhvPKShotPreparation;
extern BhvPKShot bhvPKShot;
extern BhvLeavingField bhvLeavingField;
extern BhvWalkingHeading bhvWalkingHeading;

extern BhvDrible bhvDrible;
extern BhvLastApproach bhvLastApproach;
extern BhvHoldShot bhvHoldShot;
extern BhvShotChoose bhvShotChoose;

extern BhvColorCooperation bhvColorCooperation;

extern BhvUnpenalize bhvUnpenalize;

#endif
