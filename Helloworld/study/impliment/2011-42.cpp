#include <iostream>

// ��ʵ����������������������ֵ�жϣ���Ϊ��������϶�����˵����������λ�����ǵ� i ��Ԫ�ء�
class Solution
{
public:
	int getMedium(int* num1, int* num2, int size)
	{
		// ��������ָ�����ڷֱ�ָ����������
		int p1 = 0, p2 = 0;
		// ����һ����ʱ�����ô洢��ǰֵ
		int temp = num1[p1];
		// size - 1��
		while (size)
		{
			// �ȶ����������е�Ԫ�صĴ�С
			if (num1[p1] < num2[p2])
			{
				// �洢��ǰ p1 ָ��ָ�������Ԫ��
				temp = num1[p1];
				// ��ָ������ƶ�
				p1++;
			}
			else
			{
				// �洢��ǰ p2 ��ָ�������Ԫ��
				temp = num2[p2];
				// ָ������ƶ�
				p2++;
			}

			// ���Բ��ҵĴ�����һ
			size--;
		}

		return temp;
	}
};

int main()
{

	Solution sol;
	int* num1 = new int[] { 11, 13, 15, 17, 19 };
	int* num2 = new int[] { 2, 4, 6, 8, 20};

	std::cout << sol.getMedium(num1, num2, 5);
	return 0;
}