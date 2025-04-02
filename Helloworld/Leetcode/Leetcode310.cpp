#include <iostream>
#include <vector>
#include <queue>

class Solution {
public:
    std::vector<int> findMinHeightTrees(int n, std::vector<std::vector<int>>& edges) {
        // ��� n Ϊ 1����˵����ֻ�и��ڵ�
        if (n == 1) return { 0 };

        // ����ڵ��������, ��������Ĵ�С��ʼ��Ϊ n
        std::vector<int> degree(n);
        // ����һ���ڽӾ��󣬴洢�����֮��ı�
        std::vector<std::vector<int>> adjMatrix(n);
        // �������ı߼�¼��������
        for (auto& edge : edges)
        {
            // �������е����Ĺ�ϵ�� [0,1] �����֪������������Ϣ 0-1 1-0
            adjMatrix[edge[0]].emplace_back(edge[1]);
            adjMatrix[edge[1]].emplace_back(edge[0]);
            // ���������Ķ���
            degree[edge[0]]++;
            degree[edge[1]]++;
        }

        // ����һ�����У��洢����Ϊ 1 �Ķ���
        std::queue<int> queue;
        // ����һ���������
        std::vector<int> result;

        // �����������ж���Ϊ 1 �ĵ���������
        for (int i = 0; i < degree.size(); i++)
            if (degree[i] == 1) queue.emplace(i);

        int node = n;

        while (node > 2)
        {
            int len = queue.size();
            node -= len;
            for (int i = 0; i < len; i++)
            {
               // ȡ������ͷԪ��
                int curr = queue.front();
                // �ƶ�����ָ�룬�������һ��λ��
                queue.pop();
                // ��������
                for (auto& matrix : adjMatrix[curr])
                    if (--degree[matrix] == 1) queue.emplace(matrix);
            }
        }

        while (!queue.empty())
        {
            result.emplace_back(queue.front());
            queue.pop();
        }

        return result;
    }
};