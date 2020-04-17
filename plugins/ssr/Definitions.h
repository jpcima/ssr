#pragma once
#include <cmath>

static constexpr float kFeedbackTimeMin = 1e-3;
static constexpr float kFeedbackTimeMax = 200e-3;

static constexpr float kGainMinDb = -70.0;
static constexpr float kGainMaxDb = -50.0;

inline double gainLin2Log(double a)
{
    return 20.0 * std::log10(a);
}

inline double gainLog2Lin(double g)
{
    return std::pow(10.0, 0.05 * g);
}
