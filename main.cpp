#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread> // For sleep
#include <iomanip>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

// Base Option class
class Option
{
protected:
    double strikePrice;
    double expiration;
    std::string type; // "Call" or "Put"
public:
    Option(double strike, double expiry, std::string optType) : strikePrice(strike), expiration(expiry), type(optType) {}
    virtual double price(double spot, double rate, double volatility) const = 0;
    virtual ~Option() {}
    double getStrikePrice() const { return strikePrice; }
    double getExpiration() const { return expiration; }
    std::string getType() const { return type; }
};

// Black-Scholes Pricing Model
class BlackScholes
{
public:
    static double normCDF(double x)
    {
        return 0.5 * (1 + erf(x / sqrt(2)));
    }

    static double priceOption(const Option &opt, double spot, double rate, double volatility)
    {
        double d1 = (log(spot / opt.getStrikePrice()) + (rate + 0.5 * volatility * volatility) * opt.getExpiration()) / (volatility * sqrt(opt.getExpiration()));
        double d2 = d1 - volatility * sqrt(opt.getExpiration());

        if (opt.getType() == "Call")
        {
            return spot * normCDF(d1) - opt.getStrikePrice() * exp(-rate * opt.getExpiration()) * normCDF(d2);
        }
        else
        {
            return opt.getStrikePrice() * exp(-rate * opt.getExpiration()) * normCDF(-d2) - spot * normCDF(-d1);
        }
    }
};

// Call and Put Options
class CallOption : public Option
{
public:
    CallOption(double strike, double expiry) : Option(strike, expiry, "Call") {}
    double price(double spot, double rate, double volatility) const override
    {
        return BlackScholes::priceOption(*this, spot, rate, volatility);
    }
};

class PutOption : public Option
{
public:
    PutOption(double strike, double expiry) : Option(strike, expiry, "Put") {}
    double price(double spot, double rate, double volatility) const override
    {
        return BlackScholes::priceOption(*this, spot, rate, volatility);
    }
};

// Trade class to store trades
class Trade
{
public:
    std::string optionType;
    double strikePrice;
    double expiration;
    double premium;
    std::string action; // "BUY" or "SELL"

    Trade(std::string type, double strike, double expiry, double prem, std::string act)
        : optionType(type), strikePrice(strike), expiration(expiry), premium(prem), action(act) {}
};

// File Manager to handle file operations
class FileManager
{
public:
    static void saveTrade(const Trade &trade)
    {
        std::ofstream file("data/trade_history.txt", std::ios::app);
        if (file.is_open())
        {
            file << trade.action << " " << trade.optionType << " " << trade.strikePrice << " " << trade.expiration << " " << trade.premium << "\n";
            file.close();
        }
    }
};

// Market simulation
class Market
{
public:
    static void executeTrade(const Option &opt, double spot, double rate, double volatility, std::string action, int &buyCount, int &sellCount, double &totalChange)
    {
        double price = opt.price(spot, rate, volatility);
        Trade trade(opt.getType(), opt.getStrikePrice(), opt.getExpiration(), price, action);
        FileManager::saveTrade(trade);
        std::cout << "Trade executed: " << action << " " << opt.getType() << " at " << price << "\n";

        if (action == "BUY")
            buyCount++;
        else
            sellCount++;
        totalChange += price;
    }
};

void printSimulationParams(double spotPrice, double interestRate, double volatility, int numSteps, int runDelay)
{
    // Display Parameters
    std::cout << "\nSimulation Input Parameters:\n";
    std::cout << "=====================\n";
    std::cout << "Spot Price:       " << std::fixed << std::setprecision(2) << spotPrice << "\n";
    std::cout << "Interest Rate:    " << std::fixed << std::setprecision(4) << interestRate << "\n";
    std::cout << "Volatility:       " << std::fixed << std::setprecision(4) << volatility << "\n";
    std::cout << "Number of Steps:  " << numSteps << "\n";
    std::cout << "Run Delay:        " << runDelay << " seconds\n\n";
}

// ASCII Graph function
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

// Simulation statistics function
void printSimulationStats(int buyCount, int sellCount, double totalChange, int numSteps)
{
    std::cout << "\nSimulation Statistics:\n";
    std::cout << "=====================\n";
    std::cout << "Total Trades Executed: " << (buyCount + sellCount) << "\n";
    std::cout << "Total Buys: " << buyCount << "\n";
    std::cout << "Total Sells: " << sellCount << "\n";
    std::cout << "Total Price Change Over Time: " << totalChange << "\n";
    std::cout << "Total Steps: " << numSteps << "\n";
}

// Live Simulation Function GRAPHED
void runSimulation(double spotPrice = 100, double interestRate = 0.05, double volatility = 0.2, int numSteps = 20, int runDelay = 200)
{
    srand(time(0));
    // double spotPrice = 100;
    // double interestRate = 0.05;
    // double volatility = 0.2;

    // int numSteps = 20;
    int buyCount = 0, sellCount = 0;
    double totalChange = 0;
    std::vector<double> priceHistory;

    for (int i = 0; i < numSteps; ++i)
    {
        std::system(CLEAR_SCREEN);

        CallOption call(100, 1);
        PutOption put(100, 1);

        Market::executeTrade(call, spotPrice, interestRate, volatility, rand() % 2 ? "BUY" : "SELL", buyCount, sellCount, totalChange);
        Market::executeTrade(put, spotPrice, interestRate, volatility, rand() % 2 ? "BUY" : "SELL", buyCount, sellCount, totalChange);

        spotPrice += ((rand() % 200) - 100) / 100.0; // Random market movement
        priceHistory.push_back(spotPrice);

        drawGraph(priceHistory);
        std::this_thread::sleep_for(std::chrono::milliseconds(runDelay));
    }

    printSimulationStats(buyCount, sellCount, totalChange, numSteps);
}

// Simulation function
void runSimulation2()
{
    srand(time(0));
    double spotPrice = 100;
    double interestRate = 0.05;
    double volatility = 0.2;
    std::vector<double> priceHistory;

    int buyCount = 0, sellCount = 0;
    double totalChange = 0;

    for (int i = 0; i < 50; ++i)
    {
        CallOption call(100, 1);
        PutOption put(100, 1);

        Market::executeTrade(call, spotPrice, interestRate, volatility, rand() % 2 ? "BUY" : "SELL", buyCount, sellCount, totalChange);
        Market::executeTrade(put, spotPrice, interestRate, volatility, rand() % 2 ? "BUY" : "SELL", buyCount, sellCount, totalChange);

        spotPrice += ((rand() % 200) - 100) / 100.0; // Random market movement
        priceHistory.push_back(spotPrice);
    }
    drawGraph(priceHistory);
}

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
    printSimulationParams(spotPrice, interestRate, volatility, numSteps, runDelay);
    std::cout << "#" << std::string(40, '=') << "#\n";

    // Ask for confirmation before starting the simulation
    char userInput;
    std::cout << "\nConfirm parameters. Do you wish to start the simulation? (y/n): ";
    std::cin >> userInput;

    if (userInput == 'y' || userInput == 'Y')
    {
        std::cout << "\nStarting...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        runSimulation();
    }
    else
    {
        std::cout << "\nSimulation aborted.\n";
    }
    printSimulationParams(spotPrice, interestRate, volatility, numSteps, runDelay);
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
