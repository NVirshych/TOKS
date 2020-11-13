#include "Station.h"

Station::Station(byte id) {
	this->id = id;
}

void Station::setMaxDest(byte max) {
	this->maxDest = max + 1;
}

void Station::setNext(Station* next) {
	this->nextStation = next;
}

byte Station::generateDestination() {
	byte tmp = byte(rand() % maxDest);
	return tmp == id ? (id+1)%maxDest : tmp;
}

byte Station::generateData() {
	return byte( 'a' + rand() % 26 );
}

void Station::sendPackage() {
	cout << "Station " << (int)this->id << " sent " << package << endl;
	nextStation->receivePackage(package);
}

void Station::receivePackage(Package package) {
	this->package = package;
	Sleep(100);
	cout << "Station " << (int)this->id << " received " << package << endl;
	flag = 1;
}

void Station::routine() {

	while (active) {
		
		if(flag) {

			//received mark
			if (package.getControl() == 1) {

				cout << "Received mark" << endl;
				if (rand() % 2) {
					//Skip
					cout << "Passing Mark" << endl;
					sendPackage();
					flag = 0;
					continue;
				}

				//send message
				cout << "Sending message" << endl;
				package = Package(generateDestination(), id, generateData());
				sendPackage();
				flag = 0;

				//wait for package to return
				while (!flag);
				
				//right package
				if (package.getSource() == id) {
				
					if (package.getStatus() == 1) {
						//successfull delivery
						cout << "successfull delivery" << endl;
						flag = 0;
						package = Package();
						cout << "Passing Mark" << endl;
						sendPackage();
						continue;
					}
					else {
						cout << "destination didn't change status" << endl;
					}
				}
				else {
					cout << "wrong package" << endl;
				}

				//send mark further
				package = Package();
				cout << "Passing Mark" << endl;
				sendPackage();
			}
			else {

				if (package.getDestination() == id) {
					//received package
					cout << "It's mine" << endl;
					package.setStatus(1);
				}
				cout << "Passing message" << endl;
				sendPackage();
			}
			flag = 0;
		}
		Sleep(1000);
	}
}

