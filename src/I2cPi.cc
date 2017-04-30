#include <iostream>
//#include <cstdlib>
//#include <stdlib.h>
//#include <cstdio>
//#include <string>
#include <cstring>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "I2cPi.h"

using namespace std;
using namespace I2CPI_CONST;

I2cPi* I2cPi::ipi = NULL;

int I2cPi::file = 0;
unsigned char I2cPi::read_buf[READ_BUF_SIZE];

I2cPi::I2cPi(){
	if(connect() < 0) exit(1);
}

I2cPi* I2cPi::getInstance(){
	if(ipi == NULL){
		ipi = new I2cPi();
	}
	return ipi;
}

int I2cPi::connect(){
	if((file = open("/dev/i2c-1", O_RDWR)) < 0){
		perror("file open: ");
		return -1;
	}

	if(ioctl(file, I2C_SLAVE, INO_ADDR) < 0){
		perror("i2c connect: ");
		return -1;
	}

	return 0;
}

unsigned char* I2cPi::i2cRead(){
	memset(read_buf, 0, READ_BUF_SIZE);
	if(read(file, read_buf, READ_BUF_SIZE) < 0){
		return NULL;
	}
	return read_buf;
}

void I2cPi::i2cWrite(const int code, int data){
	unsigned char buf[2];
	buf[0] = code;
	if(data > 255 && data < 0){
		return;
	} else{
		buf[1] = data;
	}
	write(file, buf, 2);
}

void I2cPi::i2cWrite(const int code, float data){
	unsigned char buf[5];
	buf[0] = code;
	buf[1] = (unsigned char)(*((unsigned long*)&data) >> 24);
	buf[2] = (unsigned char)(*((unsigned long*)&data) >> 16);
	buf[3] = (unsigned char)(*((unsigned long*)&data) >> 8);
	buf[4] = (unsigned char)(*((unsigned long*)&data));

	unsigned long l = (((unsigned long)buf[1] & 0xFF) << 24);
	l += (((unsigned long)buf[2] & 0xFF) << 16);
	l += (((unsigned long)buf[3] & 0xFF) << 8);
	l += (((unsigned long)buf[4] & 0xFF));

	write(file, buf, 5);
}

void I2cPi::i2cWrite(const int code, double data){
	float f = data;
	unsigned char buf[5];
	buf[0] = code;
	buf[1] = (unsigned char)(*((unsigned long*)&f) >> 24);
	buf[2] = (unsigned char)(*((unsigned long*)&f) >> 16);
	buf[3] = (unsigned char)(*((unsigned long*)&f) >> 8);
	buf[4] = (unsigned char)(*((unsigned long*)&f));

	unsigned long l = (((unsigned long)buf[1] & 0xFF) << 24);
	l += (((unsigned long)buf[2] & 0xFF) << 16);
	l += (((unsigned long)buf[3] & 0xFF) << 8);
	l += (((unsigned long)buf[4] & 0xFF));

	write(file, buf, 5);
}

