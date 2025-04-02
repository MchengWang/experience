#include <iostream>
#include <cmath>

class Solution {
public:
    bool judgeSquareSum(int c) {
        // ͨ������ a Ȼ������a^2 + b^2 = c �õ� b ��ƽ��
        for (long a = 0; a * a <= c; a++)
        {
            // ���ÿ�ƽ������������ b ������ƽ����
            double b = std::sqrt(c - a * a);
            // ��� b Ϊһ��ȷ�����������Ҳ���С������ƥ��ɹ�
            if (b == (int)b)
            {
                return true;
            }
        }
        return false;
    }
};