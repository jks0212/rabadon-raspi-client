#include <iostream>
//#include <cstdlib>
//#include <stdlib.h>
//#include <cstdio>
//#include <string>
#include <cstring>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "SpiPino.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

const char* SpiPino::device = "/dev/spidev0.0";
uint8_t SpiPino::mode;
uint8_t SpiPino::bits = 8;
uint32_t SpiPino::speed = 50000;
uint16_t SpiPino::delay = 0;

using namespace std;
using namespace SPIPINO_CONST;

SpiPino* SpiPino::spi = NULL;

int SpiPino::file = 0;
//unsigned char SpiPino::read_buf[READ_BUF_SIZE];

SpiPino::SpiPino(){
	if(init() < 0) exit(1);
}

SpiPino* SpiPino::getInstance(){
	if(spi == NULL){
		spi = new SpiPino();
	}
	return spi;
}

int SpiPino::init(){
	file = open(device, O_RDWR);
	if(file < 0){
		perror("can't open device");
		return -1;
	}
	// spi mode
	if(ioctl(file, SPI_IOC_WR_MODE, &mode) < 0){
		perror("can't set spi mode");
		return -1;
	}
	if(ioctl(file, SPI_IOC_RD_MODE, &mode) < 0){
		perror("can't get spi mode");
		return -1;
	}
	// bits per word
	if(ioctl(file, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0){
		perror("can't set bits per word");
		return -1;
	}
	if(ioctl(file, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0){
		perror("can't get bits per word");
		return -1;
	}
	// max speed Hz
	if(ioctl(file, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0){
		perror("can't set max speed Hz");
		return -1;
	}
	if(ioctl(file, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0){
		perror("can't get max speed Hz");
		return -1;
	}
}

void SpiPino::transfer(const int code, int data){
	uint8_t tx[TR_INT_BUF];
	tx[0] = code & 255;
	tx[1] = data & 255;
//	if(data > 255 && data < 0){
//		return;
//	} else{
//		tx[1] = data & 255;
//	}
	uint8_t rx[TR_INT_BUF];
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = TR_INT_BUF,
	//	.delay_usecs = delay,
		.speed_hz = speed,
	//	.bits_per_word = bits,
	};

	if(ioctl(file, SPI_IOC_MESSAGE(1), &tr) < 1){
		perror("can't send spi message");
		return;
	}
}

void SpiPino::transfer(const int code, float data){
	uint8_t tx[TR_FLOAT_BUF];
	tx[0] = code & 255;
//	if(data > 255 && data < 0){
//		return;
//	} else{
//		tx[1] = data & 255;
//	}

//	tx[0] = code;
	tx[1] = (unsigned char)(*((unsigned long*)&data) >> 24);
	tx[2] = (unsigned char)(*((unsigned long*)&data) >> 16);
	tx[3] = (unsigned char)(*((unsigned long*)&data) >> 8);
	tx[4] = (unsigned char)(*((unsigned long*)&data));

	unsigned long l = (((unsigned long)tx[1] & 0xFF) << 24);
	l += (((unsigned long)tx[2] & 0xFF) << 16);
	l += (((unsigned long)tx[3] & 0xFF) << 8);
	l += (((unsigned long)tx[4] & 0xFF));

//	write(file, buf, 5);

	uint8_t rx[TR_FLOAT_BUF];
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = TR_FLOAT_BUF,
	//	.delay_usecs = delay,
		.speed_hz = speed,
	//	.bits_per_word = bits,
	};

	if(ioctl(file, SPI_IOC_MESSAGE(1), &tr) < 1){
		perror("can't send spi message");
		return;
	}
}

void SpiPino::transfer(const int code, double data){
	float f = data;
	uint8_t tx[TR_FLOAT_BUF];
	tx[0] = code & 255;
//	if(data > 255 && data < 0){
//		return;
//	} else{
//		tx[1] = data & 255;
//	}

//	tx[0] = code;
	tx[1] = (unsigned char)(*((unsigned long*)&f) >> 24);
	tx[2] = (unsigned char)(*((unsigned long*)&f) >> 16);
	tx[3] = (unsigned char)(*((unsigned long*)&f) >> 8);
	tx[4] = (unsigned char)(*((unsigned long*)&f));

	unsigned long l = (((unsigned long)tx[1] & 0xFF) << 24);
	l += (((unsigned long)tx[2] & 0xFF) << 16);
	l += (((unsigned long)tx[3] & 0xFF) << 8);
	l += (((unsigned long)tx[4] & 0xFF));

//	write(file, buf, 5);

	uint8_t rx[TR_FLOAT_BUF];
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = TR_FLOAT_BUF,
	//	.delay_usecs = delay,
		.speed_hz = speed,
	//	.bits_per_word = bits,
	};

	if(ioctl(file, SPI_IOC_MESSAGE(1), &tr) < 1){
		perror("can't send spi message");
		return;
	}
}


/*


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
*/




