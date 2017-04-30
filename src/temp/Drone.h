#ifndef _DRONE_H
#define _DRONE_H

namespace DRONE_CONST{
	enum{

	};
}

typedef struct _pid{
	double roll_outer_p, roll_p, roll_i, roll_d;
	double pitch_outer_p, pitch_p, pitch_i, pitch_d;
	double yaw_outer_p, yaw_p, yaw_i, yaw_d;
}PID;

typedef struct _trim{
	double roll, pitch, yaw;
}TRIM;

class Drone{
	private:

	protected:

	public:


#endif

