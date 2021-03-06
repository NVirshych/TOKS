#include "SerialPort.h"
#include "Package.h"

void SerialPort::setRate(DWORD baudRate) {

	//��������� �����
	DCB ComDCM;
	memset(&ComDCM, 0, sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(DCB);
	GetCommState(hPort, &ComDCM);

	ComDCM.BaudRate = baudRate;

	//��������� ����� ����������
	if (!SetCommState(hPort, &ComDCM)) {
		CloseHandle(hPort);
		cout << "Failed to set DCB!" << endl;
		exit(1);
	}
}

/*=========================================                         PUBLIC                         ==============================================*/

SerialPort::SerialPort(string portName) {

	hPort = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (!hPort) {
		cout << "Failed to open port!" << endl;
		exit(0);
	}

	asynchRead = { 0 };
	asynchRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	asynchWrite = { 0 };
	asynchRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	//��������� �����
	DCB ComDCM;
	memset(&ComDCM, 0, sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(DCB);
	GetCommState(hPort, &ComDCM);

	ComDCM.ByteSize = 8;							//������ ��������������� �����
	ComDCM.Parity = NOPARITY;						//����� �������� ��������
	ComDCM.StopBits = ONESTOPBIT;					//���������� �������� ���
	ComDCM.fAbortOnError = TRUE;					//��������� ������ ��� ������
	ComDCM.fDtrControl = DTR_CONTROL_DISABLE;		//����� ���������� ������� ��� ������� DTR
	
	ComDCM.fRtsControl = RTS_CONTROL_TOGGLE;		//����� ���������� ������� ��� ������� RTS
	ComDCM.fBinary = TRUE;							//�������� ����� ������
	ComDCM.fParity = FALSE;							//����� �������� ��������.
	ComDCM.fInX = ComDCM.fOutX = FALSE;				//������������� XON/XOFF ���������� ������� ��� �����/��������
	ComDCM.XonChar = 0;								
	ComDCM.XoffChar = uint8_t(0xff);
	ComDCM.fErrorChar = FALSE;						//������������� ������ �������� � ������� ��������
	ComDCM.fNull = FALSE;							//�������� ��� ����� �������� �����
	ComDCM.fOutxCtsFlow = FALSE;					//�������� �� �������� CTS
	ComDCM.fOutxDsrFlow = FALSE;					//�������� �� �������� DSR
	ComDCM.XonLim = 128;							//����������� ����� �������� � �������� ������ ����� �������� ������� XON
	ComDCM.XoffLim = 128;							//������������ ���������� ���� � �������� ������ ����� �������� ������� XOFF

	if (!SetCommState(hPort, &ComDCM)) {
		CloseHandle(hPort);
		cout << "Failed to set DCB!" << endl;
		exit(0);
	}

	//����� baudRate
	changeRate();
}

void SerialPort::changeRate() {

	//����� �������
	DWORD baudRate;

	cout << "Select baudrate:" << endl << "0. 110" << endl << "1. 9600" << endl << "2. 14400" << endl << "3. 19200" << endl << "4. 38400" << endl << "5. 56000" << endl << "6. 256000" << endl;
	switch (_getch()) {

	case('0'):
		system("cls");
		baudRate = CBR_110;
		break;

	case('1'):
		system("cls");
		baudRate = CBR_9600;
		break;

	case('2'):
		system("cls");
		baudRate = CBR_14400;
		break;

	case('3'):
		system("cls");
		baudRate = CBR_19200;
		break;

	case('4'):
		system("cls");
		baudRate = CBR_38400;
		break;

	case('5'):
		system("cls");
		baudRate = CBR_56000;
		break;

	case('6'):
		system("cls");
		baudRate = CBR_256000;
		break;

	default:
		system("cls");
		baudRate = CBR_19200;
		cout << "Wrong input. Default 19200 baudrate is set" << endl;
		break;

	}

	setRate(baudRate);
}

void SerialPort::write(char* message) {

	char buf;

	//������ 0 ������� - ����� ������
	if (!message) {

		buf = 0;
		WriteFile(hPort, &buf, 1, NULL, &asynchWrite);
		return;
	}

	//����������� ������ ���������
	WriteFile(hPort, message, 9, NULL, &asynchWrite);

}

void SerialPort::read(char* message) {

	unsigned long state = 0;


	if (SetCommMask(hPort, EV_RXCHAR)) {							//��������� ����� ������� �����

		WaitCommEvent(hPort, &state, &asynchRead);					//���������� ����� � ������� ������������� ����������� ������

		do {

			WaitForSingleObject(asynchRead.hEvent, INFINITE);		//�������� ������ ��� ����������

			ReadFile(hPort, message, 1, NULL, &asynchRead);

			WaitForSingleObject(asynchRead.hEvent, INFINITE);		//�������� ���������� ����� ������

			if (!message[0])
				return;

			if (message[0] == 126)
				break;

		} while (1);

		WaitForSingleObject(asynchRead.hEvent, INFINITE);		//�������� ������ ��� ����������

		ReadFile(hPort, message+1, 8, NULL, &asynchRead);

		WaitForSingleObject(asynchRead.hEvent, INFINITE);		//�������� ���������� ����� ������

	}
}

SerialPort::~SerialPort() {
	CloseHandle(hPort);
}