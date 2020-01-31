#include <ios>
#include <iostream>
#include <string>
#include <unordered_map>


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string answer;
    int maxCount = -1;
    std::unordered_map<std::string, int> counts;
    std::unordered_map<std::string, int> prices;
    while (!std::cout.eof()) {
        std::string name, phone;
        int price;
        std::cin >> name >> phone >> price;
        if (phone.empty())
            break;
        auto it = counts.find(phone);
        if (it != counts.end()) {
            prices[phone] = std::min(price, prices[phone]);
            ++it->second;
            if (it->second > maxCount || (it->second == maxCount && prices[phone] < prices[answer])) {
                maxCount = it->second;
                answer = phone;
            }
        } else {
            counts[phone] = 1;
            prices[phone] = price;
            if (1 > maxCount || (1 == maxCount && prices[phone] < prices[answer])) {
                maxCount = 1;
                answer = phone;
            }
        }
    }
    std::cout << answer << std::endl;
    return 0;
}


