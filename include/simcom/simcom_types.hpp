#ifndef SIMCOM_TYPES_HPP
#define SIMCOM_TYPES_HPP

// ---------------------------------------------------------------------------
// SIMCom SIM800 Series - core types
// SPDX-License-Identifier: MIT
// ---------------------------------------------------------------------------

#include <cstdint>
#include <cstddef>

namespace simcom {

// ---------------------------------------------------------------------------
// Enum class definitions
// ---------------------------------------------------------------------------
enum class SimCardType : uint8_t {
    UNKNOWN = 0,
    MICRO,
    NANO,
    STANDARD
};

enum class AntennaType : uint8_t {
    UNKNOWN = 0,
    UFL,
    SPRING,
    SMA,
    PCB
};

enum class JammingPin : uint8_t {
    NONE = 0,
    PIN5,
    PIN29,
    PIN63,
    PIN67
};

enum class AtManualVersion : uint8_t {
    UNKNOWN = 0,
    V1_01,
    V1_10,
    V1_12
};

// ---------------------------------------------------------------------------
// to_string - exact inverse of YAML -> enum mappings (round-trip safe)
// ---------------------------------------------------------------------------
inline const char* to_string(SimCardType v) noexcept {
    switch (v) {
        case SimCardType::UNKNOWN:  return "UNKNOWN";
        case SimCardType::MICRO:    return "MICRO";
        case SimCardType::NANO:     return "NANO";
        case SimCardType::STANDARD: return "STANDARD";
    }
    return "UNKNOWN";
}

inline const char* to_string(AntennaType v) noexcept {
    switch (v) {
        case AntennaType::UNKNOWN: return "UNKNOWN";
        case AntennaType::UFL:     return "UFL";
        case AntennaType::SPRING:  return "SPRING";
        case AntennaType::SMA:     return "SMA";
        case AntennaType::PCB:     return "PCB";
    }
    return "UNKNOWN";
}

inline const char* to_string(JammingPin v) noexcept {
    switch (v) {
        case JammingPin::NONE:  return "None";
        case JammingPin::PIN5:  return "PIN5";
        case JammingPin::PIN29: return "PIN29";
        case JammingPin::PIN63: return "PIN63";
        case JammingPin::PIN67: return "PIN67";
    }
    return "None";
}

inline const char* to_string(AtManualVersion v) noexcept {
    switch (v) {
        case AtManualVersion::UNKNOWN: return "UNKNOWN";
        case AtManualVersion::V1_01:   return "V1.01";
        case AtManualVersion::V1_10:   return "V1.10";
        case AtManualVersion::V1_12:   return "V1.12";
    }
    return "UNKNOWN";
}

// ---------------------------------------------------------------------------
// HardwareSpec - physical/electrical specification
// ---------------------------------------------------------------------------
struct HardwareSpec {
    const char* package_type;
    uint16_t    pin_count;
    uint16_t    voltage_min_mv;
    uint16_t    voltage_max_mv;
    uint32_t    flash_kb;

    uint16_t    length_mm;
    uint16_t    width_mm;
    uint16_t    height_mm;
    uint16_t    weight_mg;
    int8_t      operating_temp_min_c;
    int8_t      operating_temp_max_c;
    uint16_t    current_sleep_ua;
    uint16_t    current_peak_ma;
    uint8_t     gprs_class;
    uint16_t    gprs_downlink_kbps;
    uint16_t    gprs_uplink_kbps;
    uint32_t    baud_rate_min;
    uint32_t    baud_rate_max;
    SimCardType sim_card_type;
    AntennaType antenna_connector;
    uint8_t     uart_count;
    bool        usb_support;
    bool        rtc_backup;
    const char* network_status_pin;
    const char* operating_status_pin;
};

// ---------------------------------------------------------------------------
// FeatureFlags - hardware capabilities (booleans + numeric counts)
// ---------------------------------------------------------------------------
struct FeatureFlags {
    bool     has_bluetooth;
    bool     has_gps;
    bool     has_fm;
    bool     has_audio;
    bool     has_keypad;
    bool     has_sd;
    bool     has_pcm;
    bool     has_i2c;

    uint8_t  gpio_count;
    uint8_t  adc_count;
    uint8_t  pwm_count;

    bool     has_uart;
    bool     has_usb;
    bool     has_rtc;
    bool     has_kpled;
    bool     has_rf_sync;
    bool     has_antenna_gps;
    bool     has_antenna_bt;
    bool     has_tdd;
};

// ---------------------------------------------------------------------------
// ATCommandDeltas - AT command variations
// ---------------------------------------------------------------------------
struct ATCommandDeltas {
    bool        cband_quad_band;
    uint8_t     cmic_channels;
    uint8_t     sidet_channels;
    bool        supports_csclk2;
    uint8_t     cfgri_default;
    bool        chfa_pcm_support;
    const char* jamming_pin;
    const char* extra_note;
    JammingPin  jamming_pin_enum;
};

// ---------------------------------------------------------------------------
// ATCommandVersionDelta - differences between AT manual versions
// ---------------------------------------------------------------------------
struct ATCommandVersionDelta {
    const char*     command_name;
    AtManualVersion added_in;
    AtManualVersion modified_in;
    AtManualVersion deprecated_in;
    const char*     description;
};

// ---------------------------------------------------------------------------
// ModuleCrossReference - auxiliary pins and related AT commands
// ---------------------------------------------------------------------------
struct ModuleCrossReference {
    const char* module;
    const char* audio_pins[8];
    const char* related_commands[16];
};

// ---------------------------------------------------------------------------
// ModuleSpec - complete record for one module
// ---------------------------------------------------------------------------
struct ModuleSpec {
    const char*     name;
    HardwareSpec    hw;
    FeatureFlags    feat;
    ATCommandDeltas at;
};

} // namespace simcom

#endif // SIMCOM_TYPES_HPP
