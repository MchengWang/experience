#include <iostream>

struct Node
{
	int data;
	Node* next;
	Node() :data(0), next(nullptr) {}
	Node(int x) :data(x), next(nullptr) {}
	Node(int x, Node* next) :data(x), next(next) {}
};

// �ÿռ任ʱ��
Node* func(Node* node, int n)
{
	// ����һ����СΪ n ������ ��Ϊ�����±��Ǵ� 0 ��ʼ�ģ�Ϊ�˷���ֱ��������� �±� 1 ��ʼ�洢
	int* nums = new int[n + 1] { 0 };
	
	// ���������ڵ㣬�洢��ǰ�ڵ���Ϣ
	Node* cur = node;

	// ѭ��������ǰ�ڵ����һ���ڵ㲻���ڣ����˳�
	while (cur->next)
	{
		// ����һ����ʱ�洢���ݱ���
		int temp = cur->next->data > 0 ? cur->next->data : -(cur->next->data);
		if (nums[temp])
		{
			Node* t = cur->next;
			cur->next = t->next;
			delete t;
		}
		else
		{
			nums[temp] = 1;
			cur = cur->next;
		}
	}

	return node->next;
}

int main()
{
	Node* node = new Node(0, new Node(21, new Node(-15, new Node(-15, new Node(-7, new Node(15, nullptr))))));
	Node* s = func(node, 21);
	while (s)
	{
		std::cout << s->data << " ";
		s = s->next;
	}
	return 0;
}