#ifndef MARKET_H
#define MARKET_H

#include "Option.h"
#include "FileManager.h"
#include <iostream>

class Market
{
public:
    static void executeTrade(const Option &opt, double spot, double rate, double volatility, std::string action, int &buyCount, int &sellCount, double &totalChange);
};

#endif