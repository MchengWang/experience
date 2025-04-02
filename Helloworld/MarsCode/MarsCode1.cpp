#include <iostream>
#include <vector>

int solution(std::vector<int> cards) {
    // Edit your code here
    int temp = cards[0];
    for (int i = 1; i < cards.size(); i++) {
        temp ^= cards[i];
    }
    return temp;
}

int main() {
    // Add your test cases here

    std::cout << (solution({ 1, 1, 2, 2, 3, 3, 4, 5, 5 }) == 4) << std::endl;
    std::cout << (solution({ 0, 1, 0, 1, 2 }) == 2) << std::endl;

    return 0;
}