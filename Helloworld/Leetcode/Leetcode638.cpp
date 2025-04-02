#include <vector>
#include <map>
#include <algorithm>

class Solution {
public:
    std::map<std::vector<int>, int> memo;

    int shoppingOffers(std::vector<int>& price, std::vector<std::vector<int>>& special, std::vector<int>& needs) {
        int n = price.size();

        // ���˲���Ҫ����Ĵ������ֻ������Ҫ����Ĵ����
        std::vector<std::vector<int>> filterSpecial;
        for (auto& sp : special) {
            int totalCount = 0, totalPrice = 0;
            for (int i = 0; i < n; ++i) {
                totalCount += sp[i];
                totalPrice += sp[i] * price[i];
            }
            if (totalCount > 0 && totalPrice > sp[n]) {
                filterSpecial.emplace_back(sp);
            }
        }

        return dfs(price, special, needs, filterSpecial, n);
    }

    // ���仯�����������㹺���嵥���軨�ѵ���ͼ۸�
    int dfs(std::vector<int> price, const std::vector<std::vector<int>>& special, std::vector<int> curNeeds, std::vector<std::vector<int>>& filterSpecial, int n) {
        if (!memo.count(curNeeds)) {
            int minPrice = 0;
            for (int i = 0; i < n; ++i) {
                minPrice += curNeeds[i] * price[i]; // �������κδ������ԭ�۹������嵥�е�������Ʒ
            }
            for (auto& curSpecial : filterSpecial) {
                int specialPrice = curSpecial[n];
                std::vector<int> nxtNeeds;
                for (int i = 0; i < n; ++i) {
                    if (curSpecial[i] > curNeeds[i]) { // ���ܹ��򳬳������嵥ָ����������Ʒ
                        break;
                    }
                    nxtNeeds.emplace_back(curNeeds[i] - curSpecial[i]);
                }
                if (nxtNeeds.size() == n) { // ��������Թ���
                    minPrice = std::min(minPrice, dfs(price, special, nxtNeeds, filterSpecial, n) + specialPrice);
                }
            }
            memo[curNeeds] = minPrice;
        }
        return memo[curNeeds];
    }
};