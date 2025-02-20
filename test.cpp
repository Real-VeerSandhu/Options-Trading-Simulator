#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <thread>

void printTitle()
{
    std::string title = R"(
  ____        __   _                    ______               __ _              ____ _              __       __   _           
 / __ \ ___  / /_ (_)___   ___   ___   /_  __/____ ___ _ ___/ /(_)___  ___ _  / __/(_)__ _  __ __ / /___ _ / /_ (_)___   ___ 
/ /_/ // _ \/ __// // _ \ / _ \ (_-<    / /  / __// _ `// _  // // _ \/ _ `/ _\ \ / //  ' \/ // // // _ `// __// // _ \ / _ \
\____// .__/\__//_/ \___//_//_//___/   /_/  /_/   \_,_/ \_,_//_//_//_/\_, / /___//_//_/_/_/\_,_//_/ \_,_/ \__//_/ \___//_//_/
     /_/                                                             /___/                                                                                                                              
    )";

    std::cout << title << "\n";
    std::cout << "#" << std::string(40, '=') << "#\n";
}

void startSimulation(double spotPrice, double interestRate, double volatility, int numSteps, int runDelay)
{
    // Display Parameters
    std::cout << "\nSimulation Parameters:\n";
    std::cout << "=====================\n";
    std::cout << "Spot Price:       " << std::fixed << std::setprecision(2) << spotPrice << "\n";
    std::cout << "Interest Rate:    " << std::fixed << std::setprecision(4) << interestRate << "\n";
    std::cout << "Volatility:       " << std::fixed << std::setprecision(4) << volatility << "\n";
    std::cout << "Number of Steps:  " << numSteps << "\n";
    std::cout << "Run Delay:        " << runDelay << " seconds\n\n";
    std::cout << "#" << std::string(40, '=') << "#\n";
    std::cout << "\nStarting...\n";
}

int main(int argc, char *argv[])
{
    // Default values
    double spotPrice = 100;
    double interestRate = 0.05;
    double volatility = 0.2;
    int numSteps = 20;
    int runDelay = 1;

    printTitle();

    // Parse command-line arguments if provided, else ask for input
    if (argc > 1)
        spotPrice = std::stod(argv[1]);
    else
    {
        std::cout << "Enter Spot Price: ";
        std::cin >> spotPrice;
    }

    if (argc > 2)
        interestRate = std::stod(argv[2]);
    else
    {
        std::cout << "Enter Interest Rate: ";
        std::cin >> interestRate;
    }

    if (argc > 3)
        volatility = std::stod(argv[3]);
    else
    {
        std::cout << "Enter Volatility: ";
        std::cin >> volatility;
    }

    if (argc > 4)
        numSteps = std::stoi(argv[4]);
    else
    {
        std::cout << "Enter Number of Steps: ";
        std::cin >> numSteps;
    }

    if (argc > 5)
        runDelay = std::abs(std::stoi(argv[5])); // Ensure positive value
    else
    {
        std::cout << "Enter Run Delay (seconds): ";
        std::cin >> runDelay;
        runDelay = std::abs(runDelay);
    }

    startSimulation(spotPrice, interestRate, volatility, numSteps, runDelay);
    return 0;
}
