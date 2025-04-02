#include <iostream>

int main()
{
    int t;
    while (std::cin >> t)
    {
        
        while (t--)
        {
            char* x = new char();
            int* y = new int();
            std::cin >> *x >> *y;
            if (*x >= 65 && *x <= 90)
            {
                *y += ((*x) - 64);
            }
            if (*x >= 97 && *x <= 122)
            {
                *y += (96 - (*x));
            }
            std::cout << *y << std::endl;

            delete x, y;
        }
    }
    return 0;
}