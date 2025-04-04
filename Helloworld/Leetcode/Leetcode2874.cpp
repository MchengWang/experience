#include <vector>

class Solution {
public:
    long long maximumTripletValue(std::vector<int>& nums) {
        int n = nums.size();
        long long res = 0, imax = 0, dmax = 0;
        for (int k = 0; k < n; k++) {
            res = res > dmax * nums[k] ? res : dmax * nums[k];
            dmax = dmax > imax - nums[k] ? dmax : imax - nums[k];
            imax = imax > static_cast<long long>(nums[k]) ? imax : static_cast<long long>(nums[k]);
        }
        return res;
    }
};