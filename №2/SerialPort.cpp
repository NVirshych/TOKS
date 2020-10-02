#include "SerialPort.h"
#include "Package.h"

void SerialPort::setRate(DWORD baudRate) {

	//Настройка порта
	DCB ComDCM;
	memset(&ComDCM, 0, sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(DCB);
	GetCommState(hPort, &ComDCM);

	ComDCM.BaudRate = baudRate;

	//Установка новых параметров
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


	//Настройка порта
	DCB ComDCM;
	memset(&ComDCM, 0, sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(DCB);
	GetCommState(hPort, &ComDCM);

	ComDCM.ByteSize = 8;							//Размер информационного байта
	ComDCM.Parity = NOPARITY;						//Схема контроля чётности
	ComDCM.StopBits = ONESTOPBIT;					//Количество стоповых бит
	ComDCM.fAbortOnError = TRUE;					//Окончание работы при ошибке
	ComDCM.fDtrControl = DTR_CONTROL_DISABLE;		//Режим управления обменом для сигнала DTR
	
	ComDCM.fRtsControl = RTS_CONTROL_TOGGLE;		//Режим управления потоком для сигнала RTS
	ComDCM.fBinary = TRUE;							//Двоичный режим обмена
	ComDCM.fParity = FALSE;							//Режим контроля четности.
	ComDCM.fInX = ComDCM.fOutX = FALSE;				//Использование XON/XOFF управления потоком при приёме/передаче
	ComDCM.XonChar = 0;								
	ComDCM.XoffChar = uint8_t(0xff);
	ComDCM.fErrorChar = FALSE;						//Необходимость замены символов с ошибкой четности
	ComDCM.fNull = FALSE;							//Действие при приёме нулевого байта
	ComDCM.fOutxCtsFlow = FALSE;					//Слежение за сигналом CTS
	ComDCM.fOutxDsrFlow = FALSE;					//Слежение за сигналом DSR
	ComDCM.XonLim = 128;							//Минимальное число символов в приемном буфере перед посылкой символа XON
	ComDCM.XoffLim = 128;							//Максимальное количество байт в приемном буфере перед посылкой символа XOFF

	if (!SetCommState(hPort, &ComDCM)) {
		CloseHandle(hPort);
		cout << "Failed to set DCB!" << endl;
		exit(0);
	}

	//Выбор baudRate
	changeRate();
}

void SerialPort::changeRate() {

	//Выбор частоты
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

void SerialPort::write(string message) {

	int i = 0;
	char buf;

	message = Package::encode(message);
	cout << "Encoded: " << message << endl;
	message = Package::pack(message);
	cout << "Packed: " << message << endl;
	


	//Запись 0 символа - конец работы
	if (!message.size()) {

		buf = 0;
		WriteFile(hPort, &buf, 1, NULL, &asynchWrite);
		return;
	}

	//Асинхронная запись сообщения
	WriteFile(hPort, message.c_str(), message.size(), NULL, &asynchWrite);

	//Запись \n - конец сообщения
	buf = '\n';
	WriteFile(hPort, &buf, 1, NULL, &asynchWrite);
}

string SerialPort::read() {

	char buf;
	string message;
	int i = 0;
	unsigned long state = 0;

	//Чтение сообщения до 0-символа или \n

	if (SetCommMask(hPort, EV_RXCHAR)) {							//Установка маски событий порта

		WaitCommEvent(hPort, &state, &asynchRead);					//Связывание порта и объекта синхронизации асинхронной работы

		do {

			WaitForSingleObject(asynchRead.hEvent, INFINITE);		//Ожидание данных для считывания

			ReadFile(hPort, &buf, 1, NULL, &asynchRead);

			WaitForSingleObject(asynchRead.hEvent, INFINITE);		//Ожидание считывания байта данных

			if (!buf)
				break;

			if (buf == '\n')
				break;
			message += buf;

		} while (1);
	}

	cout << "Received: " << message << endl;
	message = Package::unpack(message);
	cout << "Unpacked: " << message << endl;

	return Package::decode(message);
}

SerialPort::~SerialPort() {
	CloseHandle(hPort);
}