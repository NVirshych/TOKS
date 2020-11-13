#include "Header.h"
#include "Package.h"
#include "Station.h"

void threadFun(Station* st) {
	st->routine();
}

int main(int argc, char* argv[]) {

	Station zero(0);
	Station one(1);
	Station two(2);

	one.setMaxDest(2);
	two.setMaxDest(2);
	zero.setMaxDest(2);
	zero.setNext(&one);
	one.setNext(&two);
	two.setNext(&zero);

	thread a(threadFun, &one);
	thread b(threadFun, &two);
	thread c(threadFun, &zero);

	one.receivePackage(Package());

	a.join();
	b.join();
	c.join();

	return 0;
}