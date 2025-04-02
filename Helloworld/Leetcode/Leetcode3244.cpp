#include <vector>
#include <set>
#include <unordered_map>

class Solution {
public:
    std::vector<int> shortestDistanceAfterQueries(int n, std::vector<std::vector<int>>& queries) {
        // ��һ�� set ά����ǰ�����·������Щ����
        std::set<std::pair<int, int>> sp;
        // �Ȱѳ�ʼ�� i - 1 -> i ���ӽ���
        for (int i = 1; i < n; i++) sp.insert(std::pair<int, int>(i - 1, i));

        std::vector<int> ans;
        for (auto& query : queries) {
            int l = query[0], r = query[1];
            // �ж���������Ƿ�������·�����������
            // �����ǰ���·��������� [l', r') ���� l' == l �� r' < r����ô������ [l, r) �϶�������
            // ����������϶��Ѿ���������û�м���ı�Ҫ
            auto it = sp.lower_bound(std::pair<int, int>(l, -1));
            if (it != sp.end() && it->first == l && it->second < r) {
                // �ߵ����������� [l, r) ������������
                while (it != sp.end() && it->first < r) it = sp.erase(it);
                sp.insert(std::pair<int, int>(l, r));
            }
            // �𰸾��� set �Ĵ�С
            ans.push_back(sp.size());
        }
        return ans;
    }
};