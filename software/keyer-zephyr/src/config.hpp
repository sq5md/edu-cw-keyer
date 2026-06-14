#pragma once
#include "projectConfig.hpp"
#include <cstdint>

namespace morse
{

    class Config
    {
      public:
        constexpr Config() noexcept = default;

        [[nodiscard]] constexpr uint32_t DotDurationMs() const noexcept
        {
            constexpr auto kParisWordInDits = 50UL;
            constexpr auto kMsInSecond = 1000UL;
            constexpr auto kMsInMinute = kMsInSecond * 60UL;
            return kMsInMinute / (wpm_ * kParisWordInDits);
        }
        [[nodiscard]] constexpr uint32_t FrequencyHz() const noexcept
        {
            return frequencyHz_;
        }
        [[nodiscard]] constexpr uint32_t Wpm() const noexcept
        {
            return wpm_;
        }

        constexpr bool SetWpm(uint32_t value) noexcept
        {
            if (value < project_config::kMinWpm || value > project_config::kMaxWpm) {
                return false;
            }
            wpm_ = value;
            return true;
        }

        constexpr bool SetFrequency(uint32_t value) noexcept
        {
            if (value < project_config::kMinFrequency || value > project_config::kMaxFrequency) {
                return false;
            }
            frequencyHz_ = value;
            return true;
        }

      private:
        uint32_t wpm_{project_config::kDefWpm};
        uint32_t frequencyHz_{project_config::kDefFrequency};
    };

} // namespace morse