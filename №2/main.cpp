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

	PROCESS_INFORMATION client;																	//�������� ��������� PROCESS_INFORMATION ��� ������ ��������

	//��� ������������� ��������
	if (master) {

		//��������� ������ ������ ��������
		string cmd(argv[0]);
		cmd += " ";
		cmd += argv[2];
		cmd += " ";
		cmd += argv[2];

		//����� �������
		STARTUPINFO si;																			//��������� ������������ �������� ������ ��������	
		ZeroMemory(&si, sizeof(STARTUPINFO));													//��������� ��������� si
		si.cb = sizeof(STARTUPINFO);															//������������� ���� cb ��������� si �������� ��������

		ZeroMemory(&client, sizeof(PROCESS_INFORMATION));										//��������� ��������� pi

		if (!CreateProcess(NULL,																//�������� ������ ��������
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

		//�������� handl'��
		CloseHandle(client.hProcess);
		CloseHandle(client.hThread);
	}

	return 0;
}

void chat(SerialPort port) {

	string str;

	while (1) {

		//���� ������ ��� ��������
		cout << "Input message or press 'Enter' to exit: ";
		getline(cin, str);

		port.write(str);

		//������ 0 ������� - ����� ������
		if (!str.size())
			return;

		//������ �� �����
		str = port.read();

		if (!str.size())
			return;

		cout << "Message: " << str << endl;

	}
	
}