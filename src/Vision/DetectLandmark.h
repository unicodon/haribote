#ifndef DETECTLANDMARK_H
#define DETECTLANDMARK_H

#include "../MCL/ObserveLandmark.h"

class DetectLandmark{
public:
	static void Execute();
private:
	static LandmarkInformation ScanLine(double tilt, int intercept);
};

#endif
