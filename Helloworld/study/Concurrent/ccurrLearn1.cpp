#include <iostream>
#include <thread>

void hello()
{
	std::cout << "Hellow concurrent world\n";
}

int main()
{
	// �� thread ������ʹ�õķ�������Ϊ���Ӧ�̵߳��������
	std::thread t(hello); 
	t.join(); // ʹ���̵߳����߳�����
}