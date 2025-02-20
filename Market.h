#include <atomic>
#include <memory>
#include <thread>
#include <vector>
#include "Option.h"
#include "Trade.h"

class PriceObserver;

class Market
{
public:
    Market(size_t numThreads = std::thread::hardware_concurrency());
    ~Market();

    // Lock-free price updates
    void updatePrice(double newPrice) noexcept;

    // Observer pattern
    void registerObserver(std::weak_ptr<PriceObserver> observer);
    void notifyObservers();

    // Batch processing
    void executeBatchTrades(const std::vector<Trade> &trades);

    // Thread-safe trade execution
    void executeTrade(const Option &opt, double spot, double rate, double volatility,
                      std::string_view action, std::atomic<int> &buyCount,
                      std::atomic<int> &sellCount, std::atomic<double> &totalChange);

private:
    // Lock-free price storage
    std::atomic<double> currentPrice_;
    std::atomic<bool> priceUpdated_;

    // Observer management
    std::vector<std::weak_ptr<PriceObserver>> observers_;
    std::mutex observerMutex_;
};