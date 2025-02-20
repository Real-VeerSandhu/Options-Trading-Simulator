/* Option.cpp */
#include "Option.h"
#include "BlackScholes.h"

Option::Option(double strike, double expiry, std::string optType)
    : strikePrice(strike), expiration(expiry), type(optType) {}

double Option::getStrikePrice() const { return strikePrice; }
double Option::getExpiration() const { return expiration; }
std::string Option::getType() const { return type; }

CallOption::CallOption(double strike, double expiry)
    : Option(strike, expiry, "Call") {}

double CallOption::price(double spot, double rate, double volatility) const
{
    return BlackScholes::priceOption(*this, spot, rate, volatility);
}

PutOption::PutOption(double strike, double expiry)
    : Option(strike, expiry, "Put") {}

double PutOption::price(double spot, double rate, double volatility) const
{
    return BlackScholes::priceOption(*this, spot, rate, volatility);
}
