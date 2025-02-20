#ifndef BLACKSCHOLES_H
#define BLACKSCHOLES_H

#include "Option.h"

class BlackScholes
{
public:
    static double normCDF(double x);
    static double priceOption(const Option &opt, double spot, double rate, double volatility);
};

#endif