#include <iostream>

typedef enum { atom, list } elemtag; // atom = 0:ԭ�ӣ�list=1���ӱ�

typedef int atomtype;

typedef struct glnode
{
	elemtag tag; // �������֣���������ԭ�ӽڵ�ͱ�ڵ�
	union // ԭ�ӽڵ�ͱ�ڵ����������
	{
		atomtype tag; // atom��ԭ�ӽڵ��ֵ��atomtype���û�����
		struct { struct glnode* hp, * tp; } ptr; // ptr�Ǳ�ڵ��ָ����ptr.hp��ptr.tp�ֱ�ָ���ͷ�ͱ�β
	};
}*glist; // ���������

// ������Ⱦ���
void virus_detction()
{
	// ����bf�㷨ʵ�ֲ������
	std::ifstream infile("������Ⱦ�����������.txt");
	std::ofstream outfile("������Ⱦ���������.txt");
	infile >> num; // ��ȡ������������
	while (num--) // ���μ��ÿ�Բ���dna���˵�dna�Ƿ�ƥ�� 
	{
		infile >> virus.ch + 1; // ��ȡ����dna���У��ַ������±�1��ʼ�洢
		infile >> person.ch + 1; // ��ȡ�˵�dna����
		vir = virus.ch; // ������dna��ʱ�ݴ���vir�У��Ա����
		flag = 0; // ������ʶ�Ƿ�ƥ�䣬��ʼΪ0��ƥ���Ϊ��0
		m = virus.length; // ����dna���еĳ�����m
		for (i = m + 1, j = 1; j <= m; j++)
			virus.ch[i++] = virus.ch[j]; // �������ַ����ĳ�������2��
		virus.ch[2 * m + 1] = '\0'; // ��ӽ�������
		for (i = 0; i < m; i++) // ����ȡ��ÿ������Ϊm�Ĳ���dna��״�ַ���temp
		{
			for (j = 1; j < m; j++)
				temp.ch[j] = virus.ch[i + j];
			temp.ch[m + 1] = '\0';  // ��ӽ�������
			flag = index_bf(person, temp, 1); // ģʽƥ��
			if (flag) break; // ƥ�伴���˳�ѭ��
		} // for
		if (flag) outfile << vir + 1 << "  " << person.ch + 1 << "  " << "yes" << std::endl;
		else outfile << vir + 1 << "  " << person.ch + 1 << "  " << "no" << std::endl;
	} // while
}