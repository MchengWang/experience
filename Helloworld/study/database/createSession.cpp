#include <mysqlx/xdevapi.h>
#include <iostream>
#include <string>

//#define SSL_CASSL_MODEVERIFY_CAVERIFY_IDENTITYSSL_MODESSL_CAVERIFY_CA

int main()
{
	/*std::string username = "root";
	std::string password = "12120326";*/

	// ��ȡ���ݿ�Ự��һ�ַ�ʽ
	/*mysqlx::Session mySession(mysqlx::SessionOption::HOST, "127.0.0.1",
							  mysqlx::SessionOption::PORT, 33060,
							  mysqlx::SessionOption::USER, username, 
							  mysqlx::SessionOption::PWD, password);*/


	// ����Ự���õ���һ�ַ�����
	/*mysqlx::SessionSettings settings(mysqlx::SessionOption::HOST, "127.0.0.1",
									 mysqlx::SessionOption::PORT, 33060);

	settings.set(mysqlx::SessionOption::USER, "root");
	settings.set(mysqlx::SessionOption::PWD, "12120326");*/

	/*mysqlx::SessionSettings from_option_list(
		mysqlx::SessionOption::USER, "root",
		mysqlx::SessionOption::PWD, "12120326",
		mysqlx::SessionOption::HOST, "host",
		mysqlx::SessionOption::PORT, 33060
	);*/

	//mysqlx::Session mySession(settings);



	mysqlx::Session mySession("localhost", 33060, "root", "12120326");


	mysqlx::Schema myDb = mySession.getSchema("test");

	if (myDb.existsInDatabase())
		std::cout << "success !\n";
	else std::cout << "failure!\n";

	return 0;
}