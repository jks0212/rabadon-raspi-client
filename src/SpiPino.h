
#ifndef _SPIPINO_H
#define _SPIPINO_H
#include <stdint.h>

namespace SPIPINO_CONST{
	enum{
		SET_ROLL = 1, SET_PITCH, SET_YAW, SET_POWER, 
		SET_ROLL_OUTER_P, SET_ROLL_P, SET_ROLL_I, SET_ROLL_D,
		SET_PITCH_OUTER_P, SET_PITCH_P, SET_PITCH_I, SET_PITCH_D,
		SET_YAW_OUTER_P, SET_YAW_P, SET_YAW_I, SET_YAW_D,
		TRIM_ROLL, TRIM_PITCH, TRIM_YAW, 
		READ_BUF_SIZE = 10, TR_INT_BUF = 2, TR_FLOAT_BUF = 5, TR_DOUBLE_BUF = 5
	};
}


class SpiPino{
	private:
		static SpiPino* spi;
		SpiPino();

		static const char *device;
		static uint8_t mode;
		static uint8_t bits;
		static uint32_t speed;
		static uint16_t delay;
		static int file;
		static unsigned char read_buf[];

		int init();
	public:
		static SpiPino* getInstance();

	//	unsigned char* i2cRead();

		void transfer(const int code, int data);
		void transfer(const int code, float data);
		void transfer(const int code, double data);
	//	void i2cWrite(const int code, int data);
	//	void i2cWrite(const int code, float data);
	//	void i2cWrite(const int code, double data);
};

#endif

