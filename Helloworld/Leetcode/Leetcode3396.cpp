#include <iostream>

#include <vector>
#include <unordered_map>

class Solution {
public:
	int minimumOperations(std::vector<int>& nums) 
	{
        std::unordered_map<int, int> mp;
        int ans = 0;
        int start = 0;
        int pos = 0;

        while (pos < nums.size() && start < nums.size()) {
            if (mp.count(nums[pos]) && mp[nums[pos]] >= start) {
                start += 3;
                ans++;
            }
            else {
                mp[nums[pos]] = pos;
                pos++;
            }
        }
        
        return ans;
    }
};
