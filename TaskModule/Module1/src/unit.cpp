#include <random>

bool LostPercentChance(double lost) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double randomNum = dis(gen);
    return randomNum < lost;
}