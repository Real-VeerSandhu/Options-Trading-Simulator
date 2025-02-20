/* Simulation.cpp */
#include "Simulation.h"
#include "Market.h"
#include "Utility.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>

void startSimulation(double spotPrice, double interestRate, double volatility, int numSteps, int runDelay)
{
    srand(time(0));
    int buyCount = 0, sellCount = 0;
    double totalChange = 0;
    std::vector<double> priceHistory;

    for (int i = 0; i < numSteps; ++i)
    {
        std::system("clear");
        CallOption call(100, 1);
        PutOption put(100, 1);
        Market::executeTrade(call, spotPrice, interestRate, volatility, rand() % 2 ? "BUY" : "SELL", buyCount, sellCount, totalChange);
        Market::executeTrade(put, spotPrice, interestRate, volatility, rand() % 2 ? "BUY" : "SELL", buyCount, sellCount, totalChange);
        spotPrice += ((rand() % 200) - 100) / 100.0;
        priceHistory.push_back(spotPrice);
        drawGraph(priceHistory);
        std::this_thread::sleep_for(std::chrono::milliseconds(runDelay));
    }
    printSimulationStats(buyCount, sellCount, totalChange, numSteps);
}