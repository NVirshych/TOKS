#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

int count(string, char);

string shl(string);
string shlz(string);
string shr(string);

string delz(string);

string sxor(string, string);
string div(string, string);

int main() {

	string ost = "";

	// dima 
	// data + sum = 1001010010010010100100101010101
	// poly = 100101
	// Работает в любом месте

	// data = 01101010101011101010101010100100
	// data * x^6 = 01101010101011101010101010100100000000
	// data error = 01101010101011111010101010100100

	// poly = 1010101
	// sum = 011000
	// - 

	// poly = 1000111
	// sum = 010001
	// -

	// poly = 1100111
	// sum = 001011
	// -

	// poly = 1000101
	// sum = 111101
	// - 

	// data * x^7 = 011010101010111010101010101001000000000

	// poly = 10001001
	// sum = 1111000
	// -

	string base = "101010010101101011011011010110111111";
	string code = "1000011";

	string bases = base;

	int i = -1;

	base = shr(base);

	while (count(ost, '1') != 1) {

		base = shl(base);
		ost = div(base, code);
		i++;
		cout << i << " shift: " << ost << endl;

		if (ost == "000000") {

			cout << "No erorr" << endl;
			return 0;
		}
	}

	string tmp;

	tmp = base.substr(base.size() - ost.size(), ost.size());
	base.erase(base.size() - ost.size(), ost.size());

	tmp = sxor(tmp, ost);

	base += tmp;

	while (i) {

		base = shr(base);
		i--;
	}


	cout << "Start : " <<  bases << endl << "Fix   : " << base << endl;
	
	return 0;
}

int count(string str, char sym) {

	int pos = 0;
	int cnt = 0;

	while ((pos = str.find(sym, pos)) != -1) {
	
		cnt++;
		pos++;
	}

	return cnt;
}

string shl(string data) {

	data += data[0];
	data.erase(0, 1);

	return data;
}

string shlz(string data) {

	while (data[0] == '0')
		 data = shl(data);

	return data;
}

string shr(string data) {

	data.insert(0, data, data.size()-1, 1);
	data.erase(data.size() - 1, 1);

	return data;
}

string delz(string data) {

	while (data[0] == '0')
		data.erase(0, 1);

	return data;
}

string sxor(string a, string b) {

	string result;

	while (a.size() && b.size()) {
	
		result += '0' + (a[0] + b[0]) % 2;
		
		a.erase(0, 1);
		b.erase(0, 1);
	}

	return result;
}

string div(string data, string code) {

	string tmp;

	while (data.size() >= code.size()) {

		tmp = data.substr(0, code.size());
		data.erase(0, code.size());
		tmp = sxor(tmp, code);
		data.insert(0, tmp);	
		data = delz(data);
	}

	while (data.size() != code.size() - 1)
		data = "0" + data;

	return data;
}