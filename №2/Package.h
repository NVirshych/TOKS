#pragma once
#include "Header.h"

class Package {

private:

	static const string flag;
	static const string stuffed;

public:

	static string pack(string);
	static string unpack(string);


	static string encode(char*);
	static void decode(string, char*);

};