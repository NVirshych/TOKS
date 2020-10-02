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

		string str;

		str = port.read();

		if (!str.size())
			return 0;

		cout << "Message: " << str << endl;
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

	while (1) {

		//Ввод строки для передачи
		cout << "Input message or press 'Enter' to exit: ";
		getline(cin, str);

		port.write(str);

		//Запись 0 символа - конец работы
		if (!str.size())
			return;

		//Чтение из порта
		str = port.read();

		if (!str.size())
			return;

		cout << "Message: " << str << endl;

	}
	
}