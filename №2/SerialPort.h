#pragma once
#include "Header.h"

class SerialPort{
private:

	HANDLE hPort;
	OVERLAPPED asynchRead;
	OVERLAPPED asynchWrite;

	void setRate(DWORD);

public:

	SerialPort(string);

	void changeRate();
	void write(char*);
	void read(char*);

	~SerialPort();

};