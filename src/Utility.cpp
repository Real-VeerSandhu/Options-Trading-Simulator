#include "Utility.h"
#include <algorithm>
#include <iomanip>

void drawGraph(const std::vector<double> &prices)
{
    double maxPrice = *max_element(prices.begin(), prices.end());
    double minPrice = *min_element(prices.begin(), prices.end());
    int height = 20;
    for (int i = height; i >= 0; --i)
    {
        for (double price : prices)
        {
            double scaled = (price - minPrice) / (maxPrice - minPrice);
            if (scaled * height >= i)
                std::cout << "*";
            else
                std::cout << " ";
        }
        std::cout << "\n";
    }
}

void printSimulationStats(int buyCount, int sellCount, double totalChange, int numSteps)
{
    std::cout << "\nSimulation Statistics:\n";
    std::cout << "=====================" << std::endl;
    std::cout << "Total Trades Executed: " << (buyCount + sellCount) << "\n";
    std::cout << "Total Buys: " << buyCount << "\n";
    std::cout << "Total Sells: " << sellCount << "\n";
    std::cout << "Total Price Change Over Time: " << totalChange << "\n";
    std::cout << "Total Steps: " << numSteps << "\n";
}