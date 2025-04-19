#include <iostream>
#include <vector>

#include <algorithm> 

class Solution {
public:
    int countGoodTriplets(std::vector<int>& arr, int a, int b, int c) {
        int n = arr.size(), cnt = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    if (abs(arr[i] - arr[j]) <= a && abs(arr[j] - arr[k]) <= b && abs(arr[i] - arr[k]) <= c) {
                        ++cnt;
                    }
                }
            }
        }
        return cnt;
    }
};

int main()
{
	Solution sol;
	std::vector<int> vec = { 3,0,1,1,9,7 }; 
	std::cout << sol.countGoodTriplets(vec, 7, 2, 3 ) << std::endl; 
	return 0;
} 
