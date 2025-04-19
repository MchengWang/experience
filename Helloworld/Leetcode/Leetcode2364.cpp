#include <iostream>
#include <vector>
#include <unordered_map> 

using llong = long long;

class Solution {
public:
    llong countBadPairs(std::vector<int>& nums) {
        llong count = 0;
        std::unordered_map<int, int> mp;
        
        for (int i = 0; i < nums.size(); i ++)
        {
        	count += i - mp[nums[i] - i];
        	mp[nums[i] - i] ++;
		}
		
		return count;
    }
};

int main()
{
	
}
