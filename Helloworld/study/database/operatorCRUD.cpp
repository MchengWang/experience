#include <mysqlx/xdevapi.h>

#include <iostream>

int main()
{
	// �������ݿ����ӻỰ
	mysqlx::Session sess(
		mysqlx::SessionOption::HOST, "127.0.0.1",
		mysqlx::SessionOption::PORT, 33060,
		mysqlx::SessionOption::USER, "root",
		mysqlx::SessionOption::PWD, "12120326"
	);

	// ��ȡ���ݿ�
	mysqlx::Schema schema = sess.getSchema("ssmosyn3fd3");

	// ��ȡ�������ݱ����Ϣ
	mysqlx::Collection coll = schema.getCollection("yonghu");

	// ��ѯ
	mysqlx::DocResult docs = coll.find().execute();

    int i = 0;
    for (mysqlx::DbDoc doc : docs)
    {
        std::cout << "doc#" << i++ << ": " << doc << std::endl;

        for (mysqlx::Field fld : doc)
        {
            std::cout << " field `" << fld << "`: " << doc[fld] << std::endl;
        }

        mysqlx::string name = doc["name"];
        std::cout << " name: " << name << std::endl;

        if (doc.hasField("date") && mysqlx::Value::DOCUMENT == doc.fieldType("date"))
        {
            std::cout << "- date field" << std::endl;
            mysqlx::DbDoc date = doc["date"];
            for (mysqlx::Field fld : date)
            {
                std::cout << "  date `" << fld << "`: " << date[fld] << std::endl;
            }
            mysqlx::string month = doc["date"]["month"];
            int day = date["day"];
            std::cout << "  month: " << month << std::endl;
            std::cout << "  day: " << day << std::endl;
        }

        if (doc.hasField("toys") && mysqlx::Value::ARRAY == doc.fieldType("toys"))
        {
            std::cout << "- toys:" << std::endl;
            for (auto toy : doc["toys"])
            {
                std::cout << "  " << toy << std::endl;
            }
        }

        std::cout << std::endl;
    }
	return 0;
}