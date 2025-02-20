#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
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
    static void executeTrade(const Option &opt, double spot, double rate, double volatility, std::string action)
    {
        double price = opt.price(spot, rate, volatility);
        Trade trade(opt.getType(), opt.getStrikePrice(), opt.getExpiration(), price, action);
        FileManager::saveTrade(trade);
        std::cout << "Trade executed: " << action << " " << opt.getType() << " at " << price << "\n";
    }
};

// int main()
// {
//     CallOption call(100, 1);
//     PutOption put(100, 1);

//     double spotPrice = 105;
//     double interestRate = 0.05;
//     double volatility = 0.2;

//     Market::executeTrade(call, spotPrice, interestRate, volatility, "BUY");
//     Market::executeTrade(put, spotPrice, interestRate, volatility, "SELL");

//     return 0;
// }

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

// Simulation function
void runSimulation()
{
    srand(time(0));
    double spotPrice = 100;
    double interestRate = 0.05;
    double volatility = 0.2;
    std::vector<double> priceHistory;

    for (int i = 0; i < 50; ++i)
    {
        CallOption call(100, 1);
        PutOption put(100, 1);

        Market::executeTrade(call, spotPrice, interestRate, volatility, rand() % 2 ? "BUY" : "SELL");
        Market::executeTrade(put, spotPrice, interestRate, volatility, rand() % 2 ? "BUY" : "SELL");

        spotPrice += ((rand() % 200) - 100) / 100.0; // Random market movement
        priceHistory.push_back(spotPrice);
    }
    drawGraph(priceHistory);
}

int main()
{
    runSimulation();
    return 0;
}
