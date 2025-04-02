#include <iostream>

class Solution
{
public:
	void Reverse(int start, int end, int* nums)
	{
		// ����ָ������ָ��Ҫ������ֵ������߽�
		while (start < end)
		{
			// ����Ԫ��
			int temp = nums[start];
			nums[start] = nums[end];
			nums[end] = temp;

			start++; // ��ָ������ƶ�
			end--; // ��ָ����ǰ�ƶ�
		}
	}

	int* getArray(int size, int index, int* nums)
	{
		// ����ǽ��з�Χ�жϣ�����ת���ĵ㳬��������ĳ��ȣ����޷������������ת���ܵ�
		if (index > 0 && index < size)
		{
			// ��������ߵ� 1 2 3 4 5 6 -> 6 5 4 3 2 1
			Reverse(0, size - 1, nums); 
			// ��������벿�ֵ���ֵ�ߵ� 6 5 4 3 2 1 -> 4 5 6 3 2 1
			Reverse(0, size - index - 1, nums); 
			// �������Ұ벿�ֵߵ� 4 5 6 3 2 1 -> 4 5 6 1 2 3
			Reverse(size - index, size - 1, nums); 
		}

		return nums;
	}
};

int main()
{

	int* nums = new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Solution sol;
	int* ss = sol.getArray(10, 5, nums);
	for (int i = 0; i < 10; i++)
	{
		std::cout << nums[i] << " ";
	}
	return 0;
}