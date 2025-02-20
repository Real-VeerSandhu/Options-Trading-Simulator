/* main.cpp */
#include "Simulation.h"
#include <iostream>
#include <cstdlib>

void printTitle()
{
    std::cout << "=====================" << std::endl;
    std::cout << "   Option Trading Simulator" << std::endl;
    std::cout << "=====================" << std::endl;
}

int main(int argc, char *argv[])
{
    double spotPrice = 100;
    double interestRate = 0.05;
    double volatility = 0.2;
    int numSteps = 20;
    int runDelay = 1;

    printTitle();

    if (argc > 1)
        spotPrice = std::stod(argv[1]);
    if (argc > 2)
        interestRate = std::stod(argv[2]);
    if (argc > 3)
        volatility = std::stod(argv[3]);
    if (argc > 4)
        numSteps = std::stoi(argv[4]);
    if (argc > 5)
        runDelay = std::abs(std::stoi(argv[5]));

    startSimulation(spotPrice, interestRate, volatility, numSteps, runDelay);
    return 0;
}