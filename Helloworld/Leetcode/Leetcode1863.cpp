#include <vector>

class Solution {
public:
    int subsetXORSum(std::vector<int>& nums) {
        int res = 0;
        int n = nums.size();
        for (auto num : nums) {
            res |= num;
        }
        return res << (n - 1);
    }
};