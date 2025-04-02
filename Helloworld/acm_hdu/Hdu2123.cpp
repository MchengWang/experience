#include <iostream>

int main()
{
    int c;
    while (std::cin >> c)
    {
        while (c--)
        {
            int n;
            std::cin >> n;
            for (int i = 1; i <= n; i++)
            {
                int flag = 1;
                for (int j = 1; j <= n; j++)
                {
                    if (flag)
                    {
                        std::cout << i * j;
                        flag = 0;
                    }
                    else std::cout << ' ' << i * j;
                }
                std::cout << std::endl;
            }
            
        }
    }
    return 0;
}