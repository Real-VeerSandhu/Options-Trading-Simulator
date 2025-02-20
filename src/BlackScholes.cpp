#include "BlackScholes.h"

__m256d BlackScholes::vectorizedNormCDF(__m256d x)
{
    // Vectorized implementation of normal CDF using AVX
    __m256d one = _mm256_set1_pd(1.0);
    __m256d half = _mm256_set1_pd(0.5);
    __m256d sqrt2 = _mm256_set1_pd(SQRT_2);

    // Calculate erf using polynomial approximation
    __m256d x_sqrt2 = _mm256_div_pd(x, sqrt2);
    __m256d result = _mm256_mul_pd(half, _mm256_add_pd(one, _mm256_erf_pd(x_sqrt2)));

    return result;
}

std::array<double, 4> BlackScholes::batchPriceOptions(
    const std::array<const Option *, 4> &options,
    double spot,
    double rate,
    double volatility)
{
    // Load common values into vectors
    __m256d spotVec = _mm256_set1_pd(spot);
    __m256d rateVec = _mm256_set1_pd(rate);
    __m256d volVec = _mm256_set1_pd(volatility);

    // Load option-specific values
    __m256d strikeVec = _mm256_setr_pd(
        options[0]->getStrikePrice(),
        options[1]->getStrikePrice(),
        options[2]->getStrikePrice(),
        options[3]->getStrikePrice());

    __m256d expiryVec = _mm256_setr_pd(
        options[0]->getExpiration(),
        options[1]->getExpiration(),
        options[2]->getExpiration(),
        options[3]->getExpiration());

    // Calculate d1 and d2 vectorized
    __m256d logSpotStrike = _mm256_log_pd(_mm256_div_pd(spotVec, strikeVec));
    __m256d volSq = _mm256_mul_pd(volVec, volVec);
    __m256d half = _mm256_set1_pd(0.5);
    __m256d rateVolTerm = _mm256_add_pd(rateVec, _mm256_mul_pd(half, volSq));
    __m256d sqrtTime = _mm256_sqrt_pd(expiryVec);

    __m256d d1 = _mm256_div_pd(
        _mm256_add_pd(logSpotStrike, _mm256_mul_pd(rateVolTerm, expiryVec)),
        _mm256_mul_pd(volVec, sqrtTime));

    __m256d d2 = _mm256_sub_pd(d1, _mm256_mul_pd(volVec, sqrtTime));

    // Calculate option prices
    __m256d Nd1 = vectorizedNormCDF(d1);
    __m256d Nd2 = vectorizedNormCDF(d2);

    __m256d discountFactor = _mm256_exp_pd(_mm256_mul_pd(_mm256_neg_pd(rateVec), expiryVec));
    __m256d callPrices = _mm256_sub_pd(
        _mm256_mul_pd(spotVec, Nd1),
        _mm256_mul_pd(_mm256_mul_pd(strikeVec, discountFactor), Nd2));

    // Store results
    std::array<double, 4> results;
    _mm256_store_pd(results.data(), callPrices);

    return results;
}

double BlackScholes::priceOption(const Option &opt, double spot, double rate, double volatility)
{
    // High-precision single option pricing
    double d1 = (std::log(spot / opt.getStrikePrice()) +
                 (rate + 0.5 * volatility * volatility) * opt.getExpiration()) /
                (volatility * std::sqrt(opt.getExpiration()));

    double d2 = d1 - volatility * std::sqrt(opt.getExpiration());

    if (opt.getType() == "Call")
    {
        return spot * std::erf(d1 / SQRT_2) -
               opt.getStrikePrice() * std::exp(-rate * opt.getExpiration()) * std::erf(d2 / SQRT_2);
    }
    else
    {
        return opt.getStrikePrice() * std::exp(-rate * opt.getExpiration()) * std::erf(-d2 / SQRT_2) -
               spot * std::erf(-d1 / SQRT_2);
    }
}