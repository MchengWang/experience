#include <vector>

class Solution {
public:
    long long mostPoints(std::vector<std::vector<int>>& questions) {
        int n = questions.size();
        std::vector<long long> dp(n + 1);   // 解决每道题及以后题目的最高分数
        for (int i = n - 1; i >= 0; --i) 
            dp[i] = (dp[i + 1] >questions[i][0] + dp[n < (i + questions[i][1] + 1) ? n : (i + questions[i][1] + 1)]
                ? dp[i + 1] : questions[i][0] + dp[n < (i + questions[i][1] + 1) ? n : (i + questions[i][1] + 1)]);

        return dp[0];
    }
};