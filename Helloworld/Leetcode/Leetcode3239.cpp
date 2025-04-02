#include <vector>
#include <algorithm>

/* ��������ָ�룬�������ǰ����б������Ա��Ƿ�Ϊ��ͬԪ��
   �����һ�б�����󣬴��ڲ���ͬ�ģ��Ϳ����ж���һ��(��)���ǻ��Ĵ�
   ����Ŀ���ʵ��Ƿ�ת���� 0 -> 1 or 1 -> 0 ��ʹ���������ǻ��Ĵ�
   ÿ�β�ͬ��ʱ�򣬶�����з�ת����Ϳ���ȷ�����Ĵ�
*/

class Solution {
public:
    int minFlips(std::vector<std::vector<int>>& grid) {
        // ��¼�У��з�ת����
        int row = 0, col = 0;

        // ��ȡ���������������
        int r = grid.size(), c = grid[0].size();

        // �������飬ʹ��˫ָ����б������ֱ������ͷ �� β �������� ͷβ�������˳�ѭ��
        for (int i = 0; i < r; i++)
            for (int front = 0, rear = c - 1; front < rear; front++, rear--)
                if (grid[i][front] ^ grid[i][rear]) row++;

        // �������飬ʹ��˫ָ����б������ֱ������ͷ �� β �������� ͷβ�������˳�ѭ��
        for (int i = 0; i < c; i++)
            for (int front = 0, rear = r - 1; front < rear; front++, rear--)
                if (grid[front][i] ^ grid[rear][i]) col++;

        // �ȶ����б任�Ĵ����������ٴ�����ʹ����ÿһ�У�ÿһ�У���Ϊ���Ĵ�
        return std::min(col, row);
    }
};