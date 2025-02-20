#include "Trade.h"

Trade::Trade(std::string type, double strike, double expiry, double prem, std::string act)
    : optionType(type), strikePrice(strike), expiration(expiry), premium(prem), action(act) {}
