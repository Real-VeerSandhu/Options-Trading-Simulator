#include "Market.h"

Market::Market(size_t numThreads) : currentPrice_(0.0),
                                    priceUpdated_(false),
                                    threadPool_(numThreads) {}

void Market::updatePrice(double newPrice) noexcept
{
    currentPrice_.store(newPrice, std::memory_order_release);
    priceUpdated_.store(true, std::memory_order_release);
    notifyObservers();
}

void Market::executeBatchTrades(const std::vector<Trade> &trades)
{
    const size_t batchSize = 4; // Process 4 trades at once using SIMD
    std::vector<std::future<void>> futures;

    for (size_t i = 0; i < trades.size(); i += batchSize)
    {
        futures.push_back(threadPool_.enqueue([this, &trades, i, batchSize]()
                                              {
            std::array<const Option*, 4> options;
            size_t end = std::min(i + batchSize, trades.size());
            
            // Prepare batch
            for (size_t j = i; j < end; ++j) {
                options[j - i] = &trades[j].getOption();
            }
            
            // Process batch using SIMD
            auto prices = BlackScholes::batchPriceOptions(
                options,
                currentPrice_.load(std::memory_order_acquire),
            );
            
            for (size_t j = i; j < end; ++j) {
                auto& trade = trades[j];
            } }));
    }

    // Wait for all batches to complete
    for (auto &future : futures)
    {
        future.wait();
    }
}

void Market::executeTrade(const Option &opt, double spot, double rate, double volatility,
                          std::string_view action, std::atomic<int> &buyCount,
                          std::atomic<int> &sellCount, std::atomic<double> &totalChange)
{
    double price = BlackScholes::priceOption(opt, spot, rate, volatility);

    // Lock-free counter updates
    if (action == "BUY")
    {
        buyCount.fetch_add(1, std::memory_order_relaxed);
    }
    else
    {
        sellCount.fetch_add(1, std::memory_order_relaxed);
    }

    // Atomic price update
    totalChange.fetch_add(price, std::memory_order_relaxed);

    // Create and save trade
    Trade trade(opt.getType(), opt.getStrikePrice(), opt.getExpiration(), price, action);
    FileManager::saveTrade(trade);
}