
#ifndef _SERIAL_H
#define _SERIAL_H

namespace SERIAL_CONST{
	enum{

	};
}


class Serial{
	private:
		static Serial* ser;
		Serial();

		static int ino_com;
	public:
		static Serial* getInstance();

		int connectIno();
		void closeIno();
		std::string readIno();
		void writeIno(std::string buf);

//		int readIno(char read_buf[], unsigned int buf_len);
//		int writeIno(char write_buf[], unsigned int buf_len);

};

#endif

