#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <iostream>

void drawGraph(const std::vector<double> &prices);
void printSimulationStats(int buyCount, int sellCount, double totalChange, int numSteps);

#endif