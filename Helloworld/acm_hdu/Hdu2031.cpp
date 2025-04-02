#include <iostream>
#include <iomanip>
#include <cmath>
#define PI 3.1415927
using namespace std;

//进制转换，10进制数N转换成R进制
void baseConvert(int N, int R) {
    int a[32] = { 0 };
    char b[17] = "0123456789ABCDEF";
    if (N < 0) cout << '-';
    int num = abs(N);
    int i = 0;
    while (num > 0) {//辗转相除法
        a[i] = num % R;
        num = num / R;
        i++;
    }
    for (int j = i - 1; j >= 0; j--) {
        cout << b[a[j]];
    }
    cout << endl;
}

int main()
{
    int N, R;
    while (cin >> N >> R) {
        baseConvert(N, R);
    }
    return 0;
}