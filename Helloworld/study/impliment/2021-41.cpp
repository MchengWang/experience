#include <iostream>

#define MAXV 3

struct MGraph // ͼ�Ķ���
{
	int numVertices = 3, numEdges = 3; // ͼ��ʵ�ʵĶ������ͱ���
	char VerticesList[MAXV] = { 1, 2, 3 }; // �����
	int Edge[MAXV][MAXV] = { { 0, 1, 0 }, { 1, 0, 1 }, { 1, 1, 0 } }; // �ڽӾ���
};

int isExistEL(MGraph G)
{
	// ���� EL ����
	int count = 0;
	for (int i = 0; i < G.numVertices; i++)
	{
		int degree = 0;
		for (int j = 0; j < G.numVertices; j++)
			degree += G.Edge[i][j];
		if (degree % 2 != 0)
			count++;
	}

	if (count == 0 || count == 1) return 1;
	else return -1;
}

int main()
{
	MGraph G;
	std::cout << isExistEL(G) << std::endl;
	return 0;
}