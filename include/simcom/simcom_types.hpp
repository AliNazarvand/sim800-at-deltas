#ifndef SIMCOM_TYPES_HPP
#define SIMCOM_TYPES_HPP

#include <cstdint>
#include <cstddef>

namespace simcom {

// ---------------------------------------------------------------------------
// HardwareSpec - physical/electrical specification
// ---------------------------------------------------------------------------
struct HardwareSpec {
    const char* package_type;    // "SMT", "LGA", "SMT+LGA", "Unknown"
    uint16_t    pin_count;       // 0 = Unknown
    uint16_t    voltage_min_mv;  // millivolts, e.g. 3400
    uint16_t    voltage_max_mv;  // e.g. 4400
    uint32_t    flash_kb;        // 0 = Unknown
};

// ---------------------------------------------------------------------------
// FeatureFlags - hardware capabilities (booleans + numeric counts)
// ---------------------------------------------------------------------------
struct FeatureFlags {
    bool     has_bluetooth;
    bool     has_gps;      // includes GNSS
    bool     has_fm;
    bool     has_audio;
    bool     has_keypad;
    bool     has_sd;
    bool     has_pcm;
    bool     has_i2c;

    uint8_t  gpio_count;
    uint8_t  adc_count;
    uint8_t  pwm_count;
};

// ---------------------------------------------------------------------------
// ATCommandDeltas - AT command variations (Chapter 21 of V1.10 manual)
// ---------------------------------------------------------------------------
struct ATCommandDeltas {
    bool        cband_quad_band;   // true = quad-band, false = EGSM/DCS only
    uint8_t     cmic_channels;     // microphone channel count (2 or 3)
    uint8_t     sidet_channels;    // side-tone channel count (2 or 3)
    bool        supports_csclk2;   // AT+CSCLK=2 support
    uint8_t     cfgri_default;     // AT+CFGRI default value
    bool        chfa_pcm_support;  // AT+CHFA PCM channel support
    const char* jamming_pin;       // "PIN5", "PIN67", "PIN63", "PIN29", "None"
    const char* extra_note;        // "" if none
};

// ---------------------------------------------------------------------------
// ModuleSpec - complete record for one module
// ---------------------------------------------------------------------------
struct ModuleSpec {
    const char*         name;
    HardwareSpec        hw;
    FeatureFlags        feat;
    ATCommandDeltas     at;
};

} // namespace simcom

#endif // SIMCOM_TYPES_HPP
