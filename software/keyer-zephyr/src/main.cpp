#include <cstdlib>
#include <string_view>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/usb/usb_device.h>

#include "config.hpp"
#include "decoder.hpp"
#include "keyer.hpp"

static constexpr gpio_dt_spec kLed              = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static constexpr gpio_dt_spec kBuzzerGen        = GPIO_DT_SPEC_GET(DT_NODELABEL(buzzer_gen), gpios);
static constexpr gpio_dt_spec kDotPaddleButton  = GPIO_DT_SPEC_GET(DT_NODELABEL(dot_button), gpios);
static constexpr gpio_dt_spec kDashPaddleButton = GPIO_DT_SPEC_GET(DT_NODELABEL(dash_button), gpios);
static constexpr pwm_dt_spec kBuzzer            = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_buzzer));

static morse::Config g_config;

static int CmdSetWpm(const shell* sh, size_t argc, char** argv)
{
    if (argc < 2) {
        shell_error(sh, "Błąd: Podaj wartość WPM (np. morse wpm 25)");
        return -EINVAL;
    }

    uint32_t val = std::strtoul(argv[1], nullptr, 10);
    if (g_config.SetWpm(val)) {
        shell_print(sh, "WPM ustawiono na: %u (Długość kropki: %u ms)", g_config.Wpm(), g_config.DotDurationMs());
        return 0;
    }

    shell_error(sh, "Błąd: WPM poza zakresem (5 - 60)");
    return -ERANGE;
}

static int CmdSetFreq(const shell* sh, size_t argc, char** argv)
{
    if (argc < 2) {
        shell_error(sh, "Błąd: Podaj częstotliwość (np. morse freq 750)");
        return -EINVAL;
    }

    uint32_t val = std::strtoul(argv[1], nullptr, 10);
    if (g_config.SetFrequency(val)) {
        shell_print(sh, "Częstotliwość ustawiona na: %u Hz", g_config.FrequencyHz());
        return 0;
    }

    shell_error(sh, "Błąd: Częstotliwość poza wymaganym zakresem 600 - 800 Hz");
    return -ERANGE;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_morse,
                               SHELL_CMD_ARG(wpm, NULL, "Ustaw WPM (5-60)", CmdSetWpm, 2, 0),
                               SHELL_CMD_ARG(freq, NULL, "Ustaw czestotliwosc buzzera (600-800Hz)", CmdSetFreq, 2, 0),
                               SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(morse, &sub_morse, "Konfiguracja generatora kodu Morse'a", NULL);

int main()
{
    if (int ret = usb_enable(nullptr); ret != 0) {
        return ret;
    }

    k_msleep(1000);

    const device* kUsbSerialPortDecoder             = DEVICE_DT_GET(DT_NODELABEL(cdc_acm_uart1));

    if (!pwm_is_ready_dt(&kBuzzer) || !gpio_is_ready_dt(&kDotPaddleButton) || !gpio_is_ready_dt(&kDashPaddleButton) ||
        !gpio_is_ready_dt(&kLed) || !gpio_is_ready_dt(&kBuzzerGen) || !device_is_ready(kUsbSerialPortDecoder)) {
        return -1;
    }

    morse::Decoder decoder;

    morse::Keyer keyer{kBuzzer, kDotPaddleButton, kDashPaddleButton, kLed, kBuzzerGen, kUsbSerialPortDecoder};
    keyer.PollTask(decoder, g_config);

    return 0;
}