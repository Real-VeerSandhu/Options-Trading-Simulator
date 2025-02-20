void ConsoleView::drawGraph()
{
    double maxPrice = *std::max_element(priceHistory_.begin(), priceHistory_.end());
    double minPrice = *std::min_element(priceHistory_.begin(), priceHistory_.end());

    for (int i = height_; i >= 0; --i)
    {
        for (double price : priceHistory_)
        {
            double scaled = (price - minPrice) / (maxPrice - minPrice);
            std::cout << (scaled * height_ >= i ? "*" : " ");
        }
        std::cout << "\n";
    }
}
