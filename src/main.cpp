// ---------------------------------------------------------------------------
// SIMCom SIM800 Series - minimal demonstration
// Compiles both as a host C++ program and as an Arduino sketch.
// ---------------------------------------------------------------------------

#include "simcom/simcom.hpp"
#include <cstdio>

static void print_module(const simcom::ModuleSpec* m) {
    if (m == nullptr) { std::printf("[null module]\n"); return; }
    std::printf("%-12s pkg=%-8s pins=%-3u Vbat=%u-%umV flash=%uKB\n",
                m->name, m->hw.package_type,
                (unsigned)m->hw.pin_count,
                (unsigned)m->hw.voltage_min_mv,
                (unsigned)m->hw.voltage_max_mv,
                (unsigned)m->hw.flash_kb);
    std::printf("             bt=%d gps=%d fm=%d audio=%d keypad=%d sd=%d pcm=%d i2c=%d\n",
                (int)m->feat.has_bluetooth, (int)m->feat.has_gps,
                (int)m->feat.has_fm, (int)m->feat.has_audio,
                (int)m->feat.has_keypad, (int)m->feat.has_sd,
                (int)m->feat.has_pcm, (int)m->feat.has_i2c);
    std::printf("             gpio=%u adc=%u pwm=%u\n",
                (unsigned)m->feat.gpio_count,
                (unsigned)m->feat.adc_count,
                (unsigned)m->feat.pwm_count);
    std::printf("             cband_quad=%d cmic=%u sidet=%u csclk2=%d cfgri=%u chfa_pcm=%d jamming=%s\n",
                (int)m->at.cband_quad_band,
                (unsigned)m->at.cmic_channels,
                (unsigned)m->at.sidet_channels,
                (int)m->at.supports_csclk2,
                (unsigned)m->at.cfgri_default,
                (int)m->at.chfa_pcm_support,
                m->at.jamming_pin);
    if (m->at.extra_note != nullptr && m->at.extra_note[0] != '\0') {
        std::printf("             note=%s\n", m->at.extra_note);
    }
    std::printf("\n");
}

static void run_demo() {
    std::printf("=== SIMCom SIM800 Series database ===\n");
    std::printf("Total modules: %u\n\n", (unsigned)simcom::total_modules());
    for (std::size_t i = 0; i < simcom::total_modules(); ++i) {
        print_module(simcom::lookup_by_index(i));
    }
    std::printf("--- Named lookup ---\n");
    print_module(simcom::lookup_by_name("SIM808"));
    print_module(simcom::lookup_by_name("DoesNotExist"));
}

#if defined(ARDUINO)
#include <Arduino.h>
void setup() { Serial.begin(115200); delay(500); run_demo(); }
void loop()  { delay(1000); }
#else
int main() { run_demo(); return 0; }
#endif
