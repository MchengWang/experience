#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int solution(int n, int H, int A, vector<int> h, vector<int> a) {
   // 初始化DP数组，所有元素初始值为1
    vector<int> dp(n, 1);
    
    // 遍历每个怪物
    for (int i = 0; i < n; ++i) {
        // 遍历其之前的所有怪物
        for (int j = 0; j < i; ++j) {
            // 如果怪物j的血量和攻击力都小于怪物i的血量和攻击力
            // 并且小E的初始血量和攻击力大于怪物i的血量和攻击力
            if (h[j] < h[i] && a[j] < a[i] && H > h[i] && A > a[i]) {
                // 更新dp[i]
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }
    
    // 返回dp数组中的最大值
    return *max_element(dp.begin(), dp.end());
}

int main() {
    cout << (solution(3, 4, 5, {1, 2, 3}, {3, 2, 1}) == 1) << endl;
    cout << (solution(5, 10, 10, {6, 9, 12, 4, 7}, {8, 9, 10, 2, 5}) == 2) << endl;
    cout << (solution(4, 20, 25, {10, 15, 18, 22}, {12, 18, 20, 26}) == 3) << endl;
    return 0;
}