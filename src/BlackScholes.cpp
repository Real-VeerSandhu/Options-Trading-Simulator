#include "BlackScholes.h"
#include <cmath>

double BlackScholes::normCDF(double x)
{
    return 0.5 * (1 + erf(x / sqrt(2)));
}

double BlackScholes::priceOption(const Option &opt, double spot, double rate, double volatility)
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
