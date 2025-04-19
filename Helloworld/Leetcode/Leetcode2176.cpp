#include <iostream>
#include <vector>

class Solution {
public:
    int countPairs(std::vector<int>& nums, int k) {
        int count = 0;
        
        for (int i = 0; i < nums.size(); i++ )
        {
        	for (int j = i + 1; j < nums.size(); j++)
        	{
        		if (nums[i] != nums[j])
        		{
        			continue;
				}
				else
				{
					if ( (i * j) % k == 0)
						count++;
				}
			}
		}
        
        return count;
    }
};

int main()
{
	Solution sol;
	std::vector<int> vec = { 3, 1, 2, 2, 2, 1, 3 };
	int count = sol.countPairs(vec, 2);
	std::cout << count << std::endl;
	return 0;
}
