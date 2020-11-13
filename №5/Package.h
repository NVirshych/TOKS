#pragma once
#include "Header.h"

class Package {
private:

	byte control = 0;
	byte destination = 0;
	byte source = 0;
	byte data = 0;
	byte status = 0;
	byte monitor = 0; 

public:
	Package();
	Package(byte, byte, byte);

	friend ostream& operator<< (ostream& out, const Package& package);

	byte getControl();
	void setControl(byte);
	byte getDestination();
	void setDestination(byte);
	byte getSource();
	void setSource(byte);
	byte getData();
	void setData(byte);
	byte getStatus();
	void setStatus(byte);
	byte getMonitor();
	void setMonitor(byte);
};