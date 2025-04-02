#include <string>

class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0 || (x % 10 == 0 && x != 0)) {
            return false;
        }
        //c++��׼����ã���������ת�����ַ���
        std::string str = std::to_string(x);
        int right = 0, left = str.length() - 1;
        while (right < left) {
            if (str[right] != str[left]) {
                return false;
            }
            left--;
            right++;
        }
        return true;
    }
};