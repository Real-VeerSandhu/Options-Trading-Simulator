#ifndef TRADE_H
#define TRADE_H

#include <string>

class Trade
{
public:
    std::string optionType;
    double strikePrice;
    double expiration;
    double premium;
    std::string action; // "BUY" or "SELL"

    Trade(std::string type, double strike, double expiry, double prem, std::string act);
};

#endif