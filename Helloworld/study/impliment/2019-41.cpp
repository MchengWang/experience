#include <iostream>

struct Node
{
	int data;
	Node* next;
	Node() :data(0), next(nullptr) {}
	Node(int x) :data(x), next(nullptr) {}
	Node(int x, Node* next) :data(x), next(next) {}
};

Node* change_list(Node* h)
{
	Node* hear = h;
	Node* fear = h;

	while (fear->next != nullptr) // Ѱ���м�ڵ�
	{
		hear = hear->next; // hear ��һ��
		fear = fear->next;
		if (fear->next != nullptr) fear = fear->next; // fear ������
	}

	fear = hear->next; // hear ��ָ��ڵ�Ϊ�м�ڵ㣬fear Ϊ����������׽ڵ�
	hear->next = nullptr;
	while (fear != nullptr) // �������������
	{
		Node* temp = fear->next;
		fear->next = hear->next;
		hear->next = fear;
		fear = temp;
	}

	Node* dd = h->next; // dd ָ��ǰ��εĵ�һ�����ݽڵ� �� �����
	fear = hear->next; // fear ָ����εĵ�һ�����ݽڵ�
	hear->next = nullptr; 
	while (fear != nullptr) // ��������εĽڵ����ָ��λ��
	{
		Node* temp = fear->next; // temp ָ����ε���һ���ڵ�
		fear->next = dd->next; // �� fear ��ָ�ڵ���� dd ��ָ�ڵ�֮��
		dd->next = fear;
		dd = fear->next; // dd ָ��ǰ��ε���һ�������
		fear = temp;
	}

	return h->next;
}

int main()
{
	return 0;
}