#include <Windows.h>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

int count(string, char);

string shl(string);
string shr(string);

string delz(string);

string sxor(string, string);
string div(string, string);

	// poly = 1010101
	// - 

	// poly = 1000111
	// -

	// poly = 1100111
	// -

	// poly = 1000101
	// - 

	// poly = 1001101
	// - 

	// poly = 10001001
	// -

int main(int argc, char* argv[]) {

	int error = atoi(argv[2]);
	string data = argv[1];
	string remainder;
	string zero = "";
	string polynome;

	int crcSize = ceil(log2(data.size() + 1 + ceil(log2(data.size() + 1))));

	cout << "Data  : " << data << endl;

	for (int i = 0; i < crcSize; i++) {
		
		data += '0';
		zero += '0';
	}

	switch (crcSize){
		case(5):
			polynome = "100101";
			break;

		case(6):
			polynome = "1001101";
			break;

		default:
			cout << "Only CRC-5 and CRC-6." << endl;
			return 0;
	}

	remainder = div(data, polynome);
	data.erase(data.size() - crcSize, crcSize);
	data += remainder;
	   
	string modifiedData = data;

	if (modifiedData[error] == '0')
		modifiedData[error] = '1';
	else
		modifiedData[error] = '0';

	cout << "Poly  : " << polynome << endl << "Sum   : " << remainder << endl << endl << "Start : " << data << endl << "Error : " << modifiedData << endl;

	for (int i = 0; i < error; i++)
		cout << " ";
	cout << "        -" << endl;

	int i = -1;

	modifiedData = shr(modifiedData);

	while (count(remainder, '1') != 1) {

		modifiedData = shl(modifiedData);
		remainder = div(modifiedData, polynome);
		i++;
		cout << i << " shift : " << remainder << endl;

		if (remainder == zero) {

			cout << "No erorr" << endl;
			return 0;
		}
	}

	string tmp;

	tmp = modifiedData.substr(modifiedData.size() - remainder.size(), remainder.size());
	modifiedData.erase(modifiedData.size() - remainder.size(), remainder.size());

	tmp = sxor(tmp, remainder);

	modifiedData += tmp;

	while (i) {

		modifiedData = shr(modifiedData);
		i--;
	}


	cout << endl << "Start : " <<  data << endl << "Fix   : " << modifiedData << endl;

	for (int i = 0; i < error; i++)
		cout << " ";
	cout << "        -" << endl;
	
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