
#ifndef _I2cPi_H
#define _I2cPi_H

#define INO_ADDR 0x04

namespace I2CPI_CONST{
	enum{
		SET_ROLL = 1, SET_PITCH, SET_YAW, SET_OUTER_P, SET_P, SET_I, SET_D,
		TRIM_ROLL, TRIM_PITCH, TRIM_YAW, READ_BUF_SIZE = 10
	};
}


class I2cPi{
	private:
		static I2cPi* ipi;
		I2cPi();

		static int file;
		static unsigned char read_buf[];

		int connect();
	public:
		static I2cPi* getInstance();

		unsigned char* i2cRead();

		void i2cWrite(const int code, int data);
		void i2cWrite(const int code, float data);
		void i2cWrite(const int code, double data);
};

#endif

