#include <mysqlx/xdevapi.h>

#include <iostream>
#include <string>
#include <list>

int main()
{
	std::string url = "root:12120326@127.0.0.1:33060/test";
	
	{
		mysqlx::Session mySession(url);

		// ��ȡȫ���� schemas
		std::list<mysqlx::Schema> schemaList = mySession.getSchemas();

		// ���ȫ������ schemas
		for (mysqlx::Schema schema : schemaList)
		{
			std::cout << schema.getName() << "\n";
		}
	}


	return 0;
}