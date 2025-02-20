class Command
{
public:
    virtual void execute(Model &model) = 0;
    virtual ~Command() = default;
};

class UpdatePriceCommand : public Command
{
    double newPrice_;

public:
    explicit UpdatePriceCommand(double price) : newPrice_(price) {}
    void execute(Model &model) override
    {
        model.updatePrice(newPrice_);
    }
};

class ExecuteTradeCommand : public Command
{
    Trade trade_;

public:
    explicit ExecuteTradeCommand(Trade trade) : trade_(std::move(trade)) {}
    void execute(Model &model) override
    {
        model.addTrade(trade_);
    }
};
