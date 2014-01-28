#ifndef VISIONUTIL_H
#define VISIONUTIL_H

#include <Types.h>
#include "../config.h"

enum COLOR_COMPONENT_TAG{
	HVAL,
	SVAL,
	VVAL
};

struct ColorClusterParam{
	double ave[3];//平均
	double cov[3];//共分散
	double coef;
};

//カラーテーブル読み込み
void LoadCDT();

void DetectColor();

//色信頼度テーブル作成
void MakeColorReliabilityTable(byte* table ,const ColorClusterParam param);

//色変換関数
inline void RGB2HSV(byte R,byte G,byte B,int &H,byte &S,byte &V);
inline void YUV2RGB(byte Y,byte U,byte V,byte &R,byte &G,byte &B);
inline byte HSV2Reliability(int H,byte S,byte V,const ColorClusterParam param);
inline byte YUV2Reliability(byte Y,byte U,byte V,const ColorClusterParam param);

//画像補正関数
void ReviseImage(byte* image, longword whitebalance, longword shutterspeed);

//画像保存
int SaveYUVPPMImage(const BYTE* image, const char* name);
int SaveRGBPPMImage(const BYTE* image, const char* name);
int SaveCDTPPMImage(const char* name);

double PixelToDirection(double x, double y);
double PixelToDistance(double x, double y);
void PixelToAngle(double x,double y,double &pan,double &tilt);

void Shrink(BYTE color);
void Expand(BYTE color);

#endif
