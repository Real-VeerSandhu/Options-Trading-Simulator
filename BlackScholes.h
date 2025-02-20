#include <immintrin.h> // For SIMD
#include <array>
#include <cmath>
#include "Option.h"
#include "Utility.h"

class BlackScholes
{
public:
    // SIMD-optimized normal CDF calculation
    static __m256d vectorizedNormCDF(__m256d x);

    // Batch process multiple options simultaneously
    static std::array<double, 4> batchPriceOptions(
        const std::array<const Option *, 4> &options,
        double spot,
        double rate,
        double volatility);

    // Original single-option pricing with improved accuracy
    static double priceOption(const Option &opt, double spot, double rate, double volatility);

private:
    // Constexpr constants for improved performance
    static constexpr double SQRT_2PI = 2.506628274631000502415765284811;
    static constexpr double SQRT_2 = 1.4142135623730950488016887242097;

    // Pre-computed coefficients for Moro's inverse normal approximation
    static constexpr std::array<double, 4> MORO_COEFFS = {
        2.50662823884, -18.61500062529, 41.39119773534, -25.44106049637};
};