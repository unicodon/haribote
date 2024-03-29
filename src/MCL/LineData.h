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
	X,//X一定
	Y//Y一定
};

struct LineStruct
{
	LineDirection direction;
	int constant;//xまたはy
	int start;
	int end;
};

#endif
