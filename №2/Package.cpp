#include "Package.h"

const string Package::flag = "01111110";

string Package::encode(string data) {

	string encodedData;
	int i = 0;

	while (i < data.size())
		encodedData += bitset<8>(data[i++]).to_string();

	return encodedData;
}

string Package::decode(string data) {

	string decodedData;
	char buf[9];

	while (data.size()) {
	
		data.copy(buf, 8, 0);
		decodedData += strtol(buf, NULL, 2);
		data.erase(0, 8);
	}

	return decodedData;
}

string Package::pack(string data) {

	int i = 0;
	int parity = 0;
	size_t pos = 0;

	while (1) {
	
		if ((pos = data.find(Package::flag)) != -1) {

			pos += 7;
			data.insert(pos, "1");
		}
		else
			break;
	}

	while (i < data.size())		
		if (data[i++] == '1')
			parity++;


	if (parity % 2)
		data += "1";
	else
		data += "0";

	return Package::flag + data;
}

string Package::unpack(string packedData) {

	string data;
	size_t pos = 0;
	int i = 0;
	int parity = 0;

	if ((pos = packedData.find(Package::flag)) == -1)
		return data;

	packedData.erase(0, pos + 8);

	while (i < packedData.size()) 
		if (packedData[i++] == '1')
			parity++;

	if (parity % 2)
		return data;

	data = packedData.erase(packedData.size()-1, 1);
		
	while (1) {

		if ((pos = data.find("011111110")) != -1) {

			data.erase(pos+7, 1);
		}
		else
			break;
	}

	return data;
}