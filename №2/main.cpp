/*			
---->						README								<----
---->															<----
---->				multibyte character set 					<----
---->		cmd args = "COM_PORT_NAME_1 COM_PORT_NAME_2" 		<----
---->															<----
*/

#include "Header.h"
#include "SerialPort.h"
#include "Package.h"

void chat(SerialPort);

int main(int argc, char* argv[]) {

	if (argc < 3) {

		cout << "Insufficient command line arguments. Please, specify COM port names";
		return 0;
	}

	int master = 0;
	if (strcmp(argv[1], argv[2]))
		master = 1;

	PROCESS_INFORMATION client;																	//Создание структуры PROCESS_INFORMATION для нового процесса

	//Для родительского процесса
	if (master) {

		//Командная строка нового процесса
		string cmd(argv[0]);
		cmd += " ";
		cmd += argv[2];
		cmd += " ";
		cmd += argv[2];

		//Новый процесс
		STARTUPINFO si;																			//Структура определяющая свойства нового процесса	
		ZeroMemory(&si, sizeof(STARTUPINFO));													//Обнуление структуры si
		si.cb = sizeof(STARTUPINFO);															//Инициализация поля cb структуры si размером структры

		ZeroMemory(&client, sizeof(PROCESS_INFORMATION));										//Обнуление структуры pi

		if (!CreateProcess(NULL,																//Создание нового процесса
			const_cast<char*>(cmd.c_str()),
			NULL,
			NULL,
			TRUE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si,
			&client)) {
			cout << "CreateProcess failed\n";
			return 0;
		}
	}

	SerialPort port(argv[1]);

	if(!master){

		char buf[10];
		string str;

		while (1) {

			ZeroMemory(buf, 10);

			port.read(buf);

			if (!buf[0])
				return 0;

			str = Package::encode(buf);

			str.erase(str.size() - 8, 8);

			cout << "Pack: " << str << endl;

			str = Package::unpack(str);

			cout << "Unpc: " << str << endl;

			Package::decode(str, buf);

			ZeroMemory(buf + 7, 3);

			cout << "Message: " << buf << endl;

			str = buf;

			if (str.find("\n") != -1)
				break;

		}
	}

	chat(port);

	if (master) {

		//Закрытие handl'ов
		CloseHandle(client.hProcess);
		CloseHandle(client.hThread);
	}

	return 0;
}

void chat(SerialPort port) {

	string str;
	string enc;
	char buf[10];

	while (1) {
		str.clear();

		//Ввод строки для передачи
		cout << "Input message or press 'Enter' to exit: ";
		getline(cin, str);

		//Запись 0 символа - конец работы
		if (!str.size()) {
			port.write(NULL);
			return;
		}

		str += '\n';

		while (str.size()) {

			ZeroMemory(buf, 10);
			str.copy(buf, 7, 0);

			enc = Package::encode(buf);

			enc.erase(enc.size() - 24, 24);

			cout << "Encd: " << enc << endl;

			enc = Package::pack(enc);

			cout << "Pack: " << enc << endl;

			Package::decode(enc, buf);

			port.write(buf);	
			
			str.erase(0, 7);
		}

			
		//Чтение из порта
		while (1) {

			ZeroMemory(buf, 10);

			port.read(buf);

			if(!buf[0])
				return;

			cout << endl;

			str = Package::encode(buf);

			str.erase(str.size() - 8, 8);

			cout << "Pack: " << str << endl;

			str = Package::unpack(str);

			if (!str.size()) {

				cout << "Unpacking error" << endl;
				break;
			}

			cout << "Unpc: " << str << endl;

			Package::decode(str, buf);

			ZeroMemory(buf+7, 3);

			cout << "Message: " << buf << endl;

			str = buf;

			if (str.find("\n") != -1)
				break;

		}

	}
	
}