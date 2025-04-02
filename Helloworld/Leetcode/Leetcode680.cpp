class Solution {
public:
    bool validPalindrome(string s) {
        int cnt = 1;
        int l = 0, r = s.size() - 1;
        while (l < r) {
            if (s[l] == s[r]) {
                l++;
                r--;
            } else {
                cnt--;
                break;
            }
        }
        int nl = l + 1;
        int nr = r - 1;
        while (nl < r) {
            if (s[nl] != s[r]) {
                cnt--;
                break;
            } else {
                nl++;
                r--;
            }
        }
        while (l < nr) {
            if (s[l] != s[nr]) {
                cnt--;
                break;
            } else {
                l++;
                nr--;
            }
        }
        return cnt >= -1;
    }
};