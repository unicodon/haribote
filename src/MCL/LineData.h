#ifndef LINEDATA_H
#define LINEDATA_H

enum ScanLineType
{
	VERTICAL,
	HOLIZONTAL
};

struct LineInformation
{
	int 	r;
	int 	theta;
	ScanLineType type;
};

enum LineDirection
{
	X,//Xˆê’è
	Y//Yˆê’è
};

struct LineStruct
{
	LineDirection direction;
	int constant;//x‚Ü‚½‚Íy
	int start;
	int end;
};

#endif
