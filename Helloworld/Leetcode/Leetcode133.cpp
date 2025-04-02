#include <vector>
#include <unordered_map>

class Node
{
public:
    int val;
    std::vector<Node*> neighbors;

    Node()
    {
        val = 0;
        neighbors = std::vector<Node*>();
    }

    Node(int _val)
    {
        val = _val;
        neighbors = std::vector<Node*>();   
    }

    Node(int _val, std::vector<Node*> _neighbors)
    {
        val = _val;
        neighbors = _neighbors;
    }
};

class Solution {
public:
    Node* cloneGraph(Node* node) {
        // �����ǰ�ڵ�Ϊ�սڵ㣬ֱ�ӷ��ؿռ���
        if (!node) return nullptr;

        // ��ȡ��ǰ�ڵ���ھӸ���
        if (clone.count(node->val)) return clone[node->val];

        // �����½ڵ㣬���ڴ洢�ھ�
        Node* ans = new Node();
        ans->val = node->val;
        clone[ans->val] = ans;

        // �����ھӼ���
        std::vector<Node*> neighbors = node->neighbors;
        for (auto neighbor : neighbors)
        {
            Node* temp = cloneGraph(neighbor);
            ans->neighbors.emplace_back(temp);
        }

        return ans;
    }

private:
    std::unordered_map<int, Node*> clone;
};