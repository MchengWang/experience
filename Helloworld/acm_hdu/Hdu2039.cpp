#include<iostream>

int main()
{
    int m;
    std::cin >> m;
    while (m--)
    {
        double a, b, c;
        std::cin >> a >> b >> c;
        if ((a + b) > c && (a + c) > b && (b + c) > a)
        {
            std::cout << "YES" << std::endl;
        }
        else
            std::cout << "NO" << std::endl;
    }
    return 0;
}