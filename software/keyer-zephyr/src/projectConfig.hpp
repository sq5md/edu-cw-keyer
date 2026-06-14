#pragma once

namespace project_config
{
    constexpr auto kMinWpm = 10;
    constexpr auto kMaxWpm = 40;
    constexpr auto kDefWpm = 22;

    constexpr auto kMinFrequency = 500;
    constexpr auto kMaxFrequency = 900;
    constexpr auto kDefFrequency = 700;

    constexpr auto kSymbolToSymbolDistanceLength = 1;
    constexpr auto kDotLength = 1;
    constexpr auto kDashLength = 3;
    constexpr auto kLetterToLetterDistanceLength = 3;
    constexpr auto kWordToWordDistanceLength = 7;
} // namespace project_config