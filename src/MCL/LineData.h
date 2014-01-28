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
	X,//X���
	Y//Y���
};

struct LineStruct
{
	LineDirection direction;
	int constant;//x�܂���y
	int start;
	int end;
};

#endif
