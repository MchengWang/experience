#include <vector>
#include <unordered_map>

class NeighborSum {
public:
    NeighborSum(std::vector<std::vector<int>>& grid) {
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                pos[grid[i][j]] = { i, j };
            }
        }
        this->grid = move(grid);
    }

    int adjacentSum(int value) {
        return getSum(value, 0);
    }

    int diagonalSum(int value) {
        return getSum(value, 1);
    }

    int getSum(int value, int idx) {
        auto [x, y] = pos[value];
        int ans = 0;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dirs[idx][d][0];
            int ny = y + dirs[idx][d][1];
            if (0 <= nx && nx < grid.size() && 0 <= ny && ny < grid[0].size()) {
                ans += grid[nx][ny];
            }
        }
        return ans;
    }

private:
    std::vector<std::vector<int>> grid;
    std::unordered_map<int, std::pair<int, int>> pos;
    static constexpr int dirs[2][4][2] = {
        {{-1, 0}, {1, 0}, {0, -1}, {0, 1}},
        {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}
    };
};