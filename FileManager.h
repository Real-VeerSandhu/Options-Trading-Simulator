/* FileManager.h */
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Trade.h"
#include <fstream>

class FileManager
{
public:
    static void saveTrade(const Trade &trade);
};

#endif