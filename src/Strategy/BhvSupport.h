#ifndef BHVSUPPORT_H
#define BHVSUPPORT_H

#include "Behavior.h"

enum SUPPORT_ROLE
{
	ROLE_FW,
	ROLE_MF,
	ROLE_DF
};

class BhvSupport : public Behavior{
private:
	static unsigned char *stt_map;
	static bool load_sttmap;
	SUPPORT_ROLE my_role;

public:
	BhvSupport();
	~BhvSupport();
	
	void Execute();
	bool InSpot(int x, int y, int theta);
	unsigned char get_stt(int pos_x, int pos_y, int pos_t, int pos_r, int pos_p);
	void GetHoldID(int &hold_id, SUPPORT_ROLE &my_role);
	void GetSupportPosition(int ID, SUPPORT_ROLE my_role, int hold_id, int &dest_x, int &dest_y, int &dest_t);
	void GetFWPosition(int &dest_x, int &dest_y, int &dest_t, int ball_x, int ball_y);
	void GetMFPosition(int &dest_x, int &dest_y, int &dest_t, int ball_x, int ball_y);
	void GetDFPosition(int &dest_x, int &dest_y, int &dest_t, int ball_x, int ball_y);
};

#endif
