#include "Package.h"

const string Package::flag = "01111110";
const string Package::stuffed = "011111110";

string Package::encode(char* data) {

	string encodedData;

	for (int i = 0; i < 10; i++)
		encodedData += bitset<8>(data[i]).to_string();

	return encodedData;
}

void Package::decode(string data, char* ptr) {

	string decodedData;
	int i = 0;
	char buf[9];

	while(data.size()) {
	
		data.copy(buf, 8, 0);
		ptr[i] = strtol(buf, NULL, 2);
		data.erase(0, 8);
		i++;
	}
}

string Package::pack(string data) {

	int i = 0;
	int parity = 0;
	int flag = 0;
	size_t pos = 0;

	//Бит-стаффинг
	while (1) 
		if ((pos = data.find("111111", pos)) != -1) {

			pos += 6;
			data.insert(pos, "1");
			flag++;
		}
		else
			break;

	//Контроль чётности
	while (i < data.size())
		if (data[i++] == '1')
			parity++;

	for (int j = 0; j < 7 - flag; j++) {
		data += "0";
	}

	if (parity % 2)
		data += "1";
	else
		data += "0";

	return "01111110" + data;
}

string Package::unpack(string packedData) {

	string data;
	int i = 0;
	int parity = 0;
	size_t pos = 0;

	packedData.erase(0, 8);

	//Контроль чётности
	while (i < packedData.size()) 
		if (packedData[i++] == '1')
			parity++;

	if (parity % 2)
		return data;

	data = packedData.erase(packedData.size()-1, 1);
		
	//Восстановление данных
	i = 0;
	while (1)
		if ((pos = data.find("1111111", pos)) != -1) {
			data.erase(pos, 1);
			pos += 6;
			i++;
		}
		else
			break;

	data.erase(data.size() - (7 - i), 7 - i);

	return data;
}