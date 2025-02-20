#include <memory>
#include <string>
#include <vector>
#include "Observer.h"

// Abstract base class for views
class View : public Observer
{
public:
    virtual void update() = 0;
    virtual void render() = 0;
    virtual ~View() = default;
};

// Console view implementation
class ConsoleView : public View
{
    std::vector<double> priceHistory_;
    int height_;
    int width_;

public:
    ConsoleView(int height = 20, int width = 80) : height_(height), width_(width) {}

    void update() override
    {
        // Update from model...
    }

    void render() override
    {
        drawGraph();
        drawStats();
    }

private:
    void drawGraph();
    void drawStats();
};

// Real-time chart view using ASCII
class ASCIIChartView : public View
{
    std::vector<double> prices_;
    std::vector<double> volumes_;

public:
    void update() override;
    void render() override;

private:
    void drawCandles();
    void drawVolume();
    void drawAxis();
};