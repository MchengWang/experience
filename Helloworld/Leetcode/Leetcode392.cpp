class Solution {
public:
    bool isSubsequence(std::string s, std::string t) {
        int size1 = s.size(), size2 = t.size(), i = 0, j = 0;
        
        // 在字符串中寻找子字符串
        // 模式串和主串的字符进行对比，当模式串中的每一个字符与主串中的每一个字符中匹配后，模式串指针向后移动一位
        // 否则主串的指针向后移动一位（默认情况中匹配成功后，主串和模式串的指针均向后移动一位）
        while (i < size1 && j < size2)
        {
            if (s[i] == t[j])
                i++;
            j++;
        }

        return i == size1;
    }
};