    #include <string>
    #include <iostream>

    std::string solution(const std::string& s) {
        // Step 1: 去除前导零
        size_t start = s.find_first_not_of('0');
        if (start == std::string::npos) {
            start = 0; // 如果全是0，则保留一个0
        }
        std::string trimmed = s.substr(start);

        // Step 2: 分离整数部分和小数部分
        size_t dotPos = trimmed.find('.');
        std::string integerPart;
        std::string fractionalPart;
        if (dotPos != std::string::npos) {
            integerPart = trimmed.substr(0, dotPos);
            fractionalPart = trimmed.substr(dotPos);
        } else {
            integerPart = trimmed;
        }

        // Step 3: 处理整数部分
        std::string formattedInteger;
        int count = 0;
        for (int i = integerPart.size() - 1; i >= 0; --i) {
            formattedInteger = integerPart[i] + formattedInteger;
            count++;
            if (count % 3 == 0 && i != 0) {
                formattedInteger = ',' + formattedInteger;
            }
        }

        // Step 4: 合并整数部分和小数部分
        std::string result = formattedInteger;
        if (!fractionalPart.empty()) {
            result += fractionalPart;
        }

        return result;
    }

    int main() {
        std::cout << (solution("1294512.12412") == "1,294,512.12412") << std::endl;
        std::cout << (solution("0000123456789.99") == "123,456,789.99") << std::endl;
        std::cout << (solution("987654321") == "987,654,321") << std::endl;
    }