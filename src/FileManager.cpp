#include "FileManager.h"

void FileManager::saveTrade(const Trade &trade)
{
    std::ofstream file("data/trade_history.txt", std::ios::app);
    if (file.is_open())
    {
        file << trade.action << " " << trade.optionType << " " << trade.strikePrice << " " << trade.expiration << " " << trade.premium << "\n";
        file.close();
    }
}