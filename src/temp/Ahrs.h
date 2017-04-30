#ifndef _AHRS_H
#define _AHRS_H

namespace AHRS_CONST{
	enum{

	};
}

typedef struct _accel_data{
	short org_x, org_y, org_z;
	double amd_x, amd_y, amd_z;
	double lpf_x, lpf_y, lpf_z;
	double x, y, z;
	double roll, pitch, yaw;
}ACCEL_DATA;

typedef struct _gyro_data{
	short org_x, org_y, org_z;
	double amd_x, amd_y, amd_z;
	double hpf_x, hpf_y, hpf_z;
	double x, y, z;
	double roll, pitch, yaw;
}GYRO_DATA;

typedef struct _pid{
	double roll_outer_p, roll_p, roll_i, roll_d;
	double pitch_outer_p, pitch_p, pitch_i, pitch_d;
	double yaw_outer_p, yaw_p, yaw_i, yaw_d;
}PID;

typedef struct _trim{
	double roll, pitch, yaw;
}TRIM;

class Ahrs{
private:
	static double outer_Krp, Krp, Kri, Krd;
	static double outer_Kpp, Kpp, Kpi, Kpd;
	static double outer_Kyp, Kyp, Kyi, Kyd;
	static double rErr, pErr, yErr;
	static double rErr1, pErr1, yErr1;

	static double roll_trim, pitch_trim, yaw_trim;

	static double rTarget_deg;
	static double rCurr_deg;
	static double pTarget_deg;
	static double pCurr_deg;
	static double yTarget_deg;
	static double yCurr_deg;

	static double r_p_control, r_i_control, r_d_control;
	static double p_p_control, p_i_control, p_d_control;
	static double y_p_control, y_i_control, y_d_control;
	static double r_pid_control, p_pid_control, y_pid_control;

	void initTemp(double coef[]);
	double tempAmender(double coef[], short data, double temp);

public:
	static void setKalmanValues(double q_ang, double q_gyro, double r_ang);

	static int target_power;

	void renewPidLoopTime();
	static void applyPidValues(PID _pid);
	static void applyTrimValues(TRIM _trim);
//	static void applyPidValues(std::string pid_vals);
	static void initPidValues(PID* _pid);
//	static void resetPidValues(char* new_deg);
	static void setTargetValues(char* new_deg);
//	static int pidProc();
	int ahrsProc();
};


#endif


















