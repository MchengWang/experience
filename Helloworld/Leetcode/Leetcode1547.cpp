#include <vector>
#include <algorithm>

class Solution {
public:
    int minCost(int n, std::vector<int>& cuts) {
        int m = cuts.size();
        sort(cuts.begin(), cuts.end());
        cuts.insert(cuts.begin(), 0);
        cuts.push_back(n);
        std::vector<std::vector<int>> f(m + 2, std::vector<int>(m + 2));
        for (int i = m; i >= 1; --i) {
            for (int j = i; j <= m; ++j) {
                f[i][j] = (i == j ? 0 : INT_MAX);
                for (int k = i; k <= j; ++k) {
                    f[i][j] = std::min(f[i][j], f[i][k - 1] + f[k + 1][j]);
                }
                f[i][j] += cuts[j + 1] - cuts[i - 1];
            }
        }
        return f[1][m];
    }
};