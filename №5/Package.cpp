#include "Package.h"

Package::Package() {

	this->control = 1;
}

Package::Package(byte destination, byte source, byte data) {

	this->destination = destination;
	this->source = source;
	this->data = data;
}

ostream& operator<< (ostream& out, const Package& package) {

	out << "C:" << (int)package.control << "|S:" << (int)package.source << "|De:" << (int)package.destination << "|Da:" << package.data << "|St:" << (int)package.status << "|M:" << (int)package.monitor;
	return out;
}

byte Package::getControl() {
	return this->control;
}

void Package::setControl(byte byte) {
	this->control = byte;
}

byte Package::getDestination() {
	return this->destination;
}

void Package::setDestination(byte byte) {
	this->destination = byte;
}

byte Package::getSource() {
	return this->source;
}

void Package::setSource(byte byte) {
	this->source = byte;
}

byte Package::getData() {
	return this->data;
}

void Package::setData(byte byte) {
	this->data = byte;
}

byte Package::getStatus() {
	return this->status;
}

void Package::setStatus(byte byte) {
	this->status = byte;
}

byte Package::getMonitor() {
	return this->monitor;
}

void Package::setMonitor(byte byte) {
	this->monitor = byte;
}
