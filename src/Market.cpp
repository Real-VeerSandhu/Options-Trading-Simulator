#include "Market.h"

void Market::executeTrade(const Option &opt, double spot, double rate, double volatility, std::string action, int &buyCount, int &sellCount, double &totalChange)
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
