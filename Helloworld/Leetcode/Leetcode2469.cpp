class Solution {
public:
    std::vector<double> convertTemperature(double celsius) {
        double Kelvin = celsius + 273.15;
        double Fahrenheit = 32 + (celsius * 1.8);

        return { Kelvin, Fahrenheit };
    }
};