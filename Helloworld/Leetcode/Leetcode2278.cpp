class Solution {
public:
    int percentageLetter(std::string s, char letter) {
        int count = 0;
        for (char& ss : s)
            if (ss == letter)
                count++;
        return (count * 100 / s.size());
    }
};