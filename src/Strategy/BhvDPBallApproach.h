#ifndef BHVDPBALLAPPROACH_H
#define BHVDPBALLAPPROACH_H

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "Behavior.h"

extern const int x_elem;
extern const int y_elem;
extern const int theta_elem;
extern const int r_elem;
extern const int phi_elem;
extern const int rtp_elem;
extern const int full_elem;
extern const int x_wid;
extern const int y_wid;
extern const int t_wid;

class BhvDPBallApproach : public Behavior{
private:
	static unsigned char *m_map;
	static bool load_map;

public:
	BhvDPBallApproach();
	~BhvDPBallApproach();

	void Execute();

	unsigned char get_action(int x,int y,int r,int phi,int theta);

	int x_to_id(int x);
	int y_to_id(int y);
	int r_to_id(int r);
	int theta_to_id(int theta);
	int phi_to_id(int phi);
};

#endif
