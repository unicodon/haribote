#ifndef DETECTGOAL_H
#define DETECTGOAL_H

class DetectGoal{
public:
	static void Execute();
private:
	static int ScanLineYellow(double tilt, int intercept, int &u, int &left, int &right);
	static int ScanLineSkyblue(double tilt, int intercept, int &u, int &left, int &right);
	static int ScanVerticalLineYellow(double tilt, int x, int y);
	static int ScanVerticalLineSkyblue(double tilt, int x, int y);
};

#endif
