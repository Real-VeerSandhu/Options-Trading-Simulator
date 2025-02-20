/* Option.h */
#ifndef OPTION_H
#define OPTION_H

#include <string>

class Option
{
protected:
    double strikePrice;
    double expiration;
    std::string type; // "Call" or "Put"
public:
    Option(double strike, double expiry, std::string optType);
    virtual double price(double spot, double rate, double volatility) const = 0;
    virtual ~Option() {}
    double getStrikePrice() const;
    double getExpiration() const;
    std::string getType() const;
};

class CallOption : public Option
{
public:
    CallOption(double strike, double expiry);
    double price(double spot, double rate, double volatility) const override;
};

class PutOption : public Option
{
public:
    PutOption(double strike, double expiry);
    double price(double spot, double rate, double volatility) const override;
};

#endif