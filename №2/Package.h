#pragma once
#include "Header.h"

class Package {

private:

	static const string flag;

public:

	static string pack(string);
	static string unpack(string);


	static string encode(string);
	static string decode(string);

};