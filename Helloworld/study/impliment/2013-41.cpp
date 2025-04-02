#include <iostream>

int getMajory(int* nums, int size)
{
	// ����һ����ʱ�����洢��ǰԪ��ֵ
	int temp = nums[0];
	// ����һ�������������洢��ǰԪ�س��ֵĴ���
	int count = 1;
	for (int i = 1; i < size; i++)
	{
		if (nums[i] == temp)
			count++;
		else
			count--;

		if (!count)
		{
			temp = nums[i];
			count = 1;
		}
	}

	count = 0;
	for (int i = 0; i < size; i++)
		if (nums[i] == temp) count++;

	return count >= size / 2 ? temp : 0;
}

int main()
{
	std::cout << getMajory(new int[] { 0, 5, 5, 3, 5, 1, 5, 7 }, 8);
	return 0;
}