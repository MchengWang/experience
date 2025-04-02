#include <mysqlx/xdevapi.h>

#include <iostream>

int main()
{
	// ����������ӳ��� 7
	mysqlx::Client cli("root:12120326@127.0.0.1:33060/test", mysqlx::ClientOption::POOL_MAX_SIZE, 7);

	mysqlx::Session sess = cli.getSession();

	mysqlx::Schema s = sess.getSchema("test");

	if (s.existsInDatabase())
		std::cout << "success !\n";

	cli.close(); // �ر�ȫ���Ự

	return 0;
}