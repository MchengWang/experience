#include <iostream>

struct BTree
{
	char data[10];
	BTree* left, * right;
};

void BTreeToE(BTree* root)
{
	BtreeToExp(root, 1); // ���ĸ߶�Ϊ 1
}

void BtreeToExp(BTree* root, int deep)
{
	// �սڵ㷵��
	if (!root) return;
	else if (!root->left && !root->right) // ��ΪҶ�ڵ�
		std::cout << root->data; // ���������
	else
	{
		if (deep > 1) std::cout << "("; // �����ӱ��ʽ���һ������
		BtreeToExp(root->left, deep + 1);
		std::cout << root->data; // ���������
		BtreeToExp(root->right, deep + 1);
		if (deep > 1) std::cout << ")"; // �����ӱ��ʽ���һ������
	}
}

int main()
{
	return 0;
}