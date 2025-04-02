#include <iostream>

#define ABS(x) x > 0 ? x : -x

// ��ʵ�����ÿռ任ʱ����㷨���
int getNotHave(int* nums)
{
	// �������鳤��
	int size = (sizeof(nums) / sizeof(nums[0]));

	// ����һ����ʱ�������ڴ洢���������Ԫ��
	int max = 0;
	for (int i = 0; i < size; i++)
		if (max < ABS(nums[i])) max = ABS(nums[i]);

	// �������飬��������Ϊ max ����������Ԫ��Ĭ�ϸ�ֵΪ 0
	int* temp = new int[max + 1] {0};

	// �������飬Ȼ�󽫶�Ӧ�±��ֵ��Ϊ 1
	for (int i = 0; i < size; i++)
	{
		if (!temp[ABS(nums[i])]) temp[ABS(nums[i])] = 1;
	}

	int t = 0;

	// ���� temp ���飬����ȡ�ʼ���ֲ�Ϊ 1 ����ֵ
	for (int i = 1; i < max + 1; i++)
	{
		if (!temp[i])
		{
			t = i;
			return i;
		}
			
	}


	if (t == 0) return max + 1;

	return 0;
}

int main()
{
	//int* nums = new int[4] { -5, 3, 2, 3 };
	int* nums = new int[3] { 1, 2, 3 };

	std::cout << getNotHave(nums) << std::endl;
	return 0;
}