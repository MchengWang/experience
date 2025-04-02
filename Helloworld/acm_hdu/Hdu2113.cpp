#include <iostream>
#include <string>

int main()
{
    std::string strs;
    int flag = 0;
    while (std::cin >> strs)
    {
        int result = 0;
        for (int i = 0; i < strs.length(); i++)
        {
            if (strs.at(i) % 2 == 0) result += (strs.at(i) - 48);
        }
        std::cout << result << std::endl;
        if (!flag)
        {
            std::cout << std::endl;
            flag = 1;
        }
        if (!flag) flag = 1;
    }
    return 0;
}