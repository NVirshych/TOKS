#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

char COLLISION = '-';

char chanel = ' ';

//Resolves collision after 200ms
void collisionResolver() {

	while (1) {
	
		while (chanel != COLLISION);
		
		Sleep(300);

		chanel = ' ';	
	}
}

void transmitter(string data) {

	int tries;
	int k;
	int shift = 0;

	while (data.size() > shift) {
	
		tries = 0;

		while (1) {
			
			//Waiting for free chanel
			while (chanel != ' ');

			//Writing in chanel
			chanel = data[shift];

			//Collision window
			Sleep(200);

			//Collision check
			if (chanel == data[shift]) {
				
				chanel = ' ';
				break;
			}

			//Jam signal
			chanel = COLLISION;

			//Inc tries counter
			tries++;

			//Check tries counter
			if (tries == 10) {
				cout << "Transmitter disconected after 10 tries" << endl;
				return;
			}

			//Calculate and wait the random backoff period
			k = (rand() % (10 - tries)) + tries;
			Sleep(pow(2, k));
		}

		shift++;
	}
}

int main(int argc, char* argv[]) {

	thread resolver(collisionResolver);
	thread one(transmitter, "1234567891234567890");
	thread two(transmitter, "qwertyuioasdfghjklasdfghjkl");
	thread three(transmitter, "יצףךוםדרשחהכמנןאגûגקסלטעüבכדם");

	while (1) {
	
		Sleep(100);
		cout << chanel << endl;

	}

	one.join();
	two.join();
	three.join();

	return 0;
}