#pragma once
#include "config.hpp"
#include "decoder.hpp"
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

namespace morse
{
    class Keyer
    {
      public:
        Keyer(const pwm_dt_spec buzzer,
              const gpio_dt_spec dotPin,
              const gpio_dt_spec dashPin,
              const gpio_dt_spec ledPin,
              const gpio_dt_spec buzzerGenPin,
              const device* outputUart)
            : buzzer_{buzzer}, dotPin_{dotPin}, dashPin_{dashPin}, ledPin_{ledPin}, buzzerGenPin_{buzzerGenPin},
              uart_{outputUart}
        {
            gpio_pin_configure_dt(&dotPin_, GPIO_INPUT);
            gpio_pin_configure_dt(&dashPin_, GPIO_INPUT);
            gpio_pin_configure_dt(&ledPin_, GPIO_OUTPUT_INACTIVE);
            gpio_pin_configure_dt(&buzzerGenPin_, GPIO_OUTPUT_INACTIVE);
        }

        void PollTask(Decoder& decoder, const Config& cfg)
        {
            uint32_t idleTimeMs       = 0;
            uint32_t idleTimeSpaceMs = 0;
            bool insertSpace            = false;
            while (true) {
                const uint32_t ditLen = cfg.DotDurationMs();

                const bool dotPressed  = gpio_pin_get_dt(&dotPin_) > 0;
                const bool dashPressed = gpio_pin_get_dt(&dashPin_) > 0;

                if (dotPressed || dashPressed) {
                    idleTimeMs       = 0;
                    idleTimeSpaceMs = 0;

                    if (dotPressed) {
                        PlayTone(ditLen, cfg.FrequencyHz());
                        decoder.InsertSymbol(false);
                    }
                    else if (dashPressed) {
                        PlayTone(ditLen * project_config::kDashLength, cfg.FrequencyHz());
                        decoder.InsertSymbol(true);
                    }

                    k_msleep(ditLen * project_config::kSymbolToSymbolDistanceLength);
                }
                else {
                    k_msleep(1);
                    idleTimeMs += 1;
                    idleTimeSpaceMs += 1;

                    if (idleTimeMs >= (ditLen * project_config::kLetterToLetterDistanceLength)) {
                        if (const auto character = decoder.Flush(); character.has_value()) {
                            uart_poll_out(uart_, static_cast<char>(*character));
                            insertSpace = true;
                        }
                        idleTimeMs = 0;
                    }

                    if (insertSpace && idleTimeSpaceMs >= (ditLen * project_config::kWordToWordDistanceLength)) {
                        insertSpace = false;
                        uart_poll_out(uart_, ' ');
                    }
                }
            }
        }

      private:
        void PlayTone(uint32_t durationMs, uint32_t freqHz)
        {
            uint32_t period = PWM_HZ(freqHz);

            gpio_pin_set_dt(&ledPin_, 1);
            gpio_pin_set_dt(&buzzerGenPin_, 1);
            pwm_set_dt(&buzzer_, period, period / 2);

            k_msleep(durationMs);

            pwm_set_dt(&buzzer_, period, 0);
            gpio_pin_set_dt(&ledPin_, 0);
            gpio_pin_set_dt(&buzzerGenPin_, 0);
        }

        const pwm_dt_spec buzzer_;
        const gpio_dt_spec dotPin_;
        const gpio_dt_spec dashPin_;
        const gpio_dt_spec ledPin_;
        const gpio_dt_spec buzzerGenPin_;
        const device* uart_;
    };

} // namespace morse