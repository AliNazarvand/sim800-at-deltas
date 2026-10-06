#ifndef SIMCOM_API_HPP
#define SIMCOM_API_HPP

#include "simcom_types.hpp"
#include "simcom_database.hpp"
#include <cstddef>

namespace simcom {

inline bool str_equal(const char* a, const char* b) noexcept {
    if (a == nullptr || b == nullptr) return false;
    while (*a != '\0' && *b != '\0') {
        if (*a != *b) return false;
        ++a; ++b;
    }
    return *a == *b;
}

inline const ModuleSpec* lookup_by_name(const char* name) noexcept {
    if (name == nullptr) return nullptr;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (str_equal(MODULES[i].name, name)) return &MODULES[i];
    }
    return nullptr;
}

constexpr const ModuleSpec* lookup_by_index(std::size_t index) noexcept {
    if (index >= MODULE_COUNT) return nullptr;
    return &MODULES[index];
}

constexpr std::size_t total_modules() noexcept {
    return MODULE_COUNT;
}

constexpr const ModuleSpec* get_all() noexcept {
    return MODULES;
}




// ---------------------------------------------------------------------------
// Internal helper: append text to a buffer with capacity accounting.
// ---------------------------------------------------------------------------
namespace detail {
inline std::size_t append_str(char* buffer, std::size_t buffer_size,
                              std::size_t pos, const char* s) noexcept {
    if (s == nullptr) s = "";
    while (*s != '\0') {
        if (buffer != nullptr && pos + 1 < buffer_size) buffer[pos] = *s;
        ++pos; ++s;
    }
    return pos;
}
inline std::size_t append_u64(char* buffer, std::size_t buffer_size,
                              std::size_t pos, unsigned long long v) noexcept {
    char tmp[32];
    int n = 0;
    if (v == 0) { tmp[n++] = '0'; }
    else { while (v > 0) { tmp[n++] = char('0' + (v % 10)); v /= 10; } }
    for (int i = n - 1; i >= 0; --i) {
        if (buffer != nullptr && pos + 1 < buffer_size) buffer[pos] = tmp[i];
        ++pos;
    }
    return pos;
}
inline std::size_t append_i64(char* buffer, std::size_t buffer_size,
                              std::size_t pos, long long v) noexcept {
    if (v < 0) {
        if (buffer != nullptr && pos + 1 < buffer_size) buffer[pos] = '-';
        ++pos;
        return append_u64(buffer, buffer_size, pos, (unsigned long long)(-v));
    }
    return append_u64(buffer, buffer_size, pos, (unsigned long long)v);
}
inline std::size_t append_bool(char* buffer, std::size_t buffer_size,
                               std::size_t pos, bool b) noexcept {
    return append_str(buffer, buffer_size, pos, b ? "true" : "false");
}
inline std::size_t append_json_str(char* buffer, std::size_t buffer_size,
                                   std::size_t pos, const char* s) noexcept {
    if (buffer != nullptr && pos + 1 < buffer_size) buffer[pos] = '"';
    ++pos;
    if (s == nullptr) {
        if (buffer != nullptr && pos + 4 < buffer_size) {
            buffer[pos]='n'; buffer[pos+1]='u'; buffer[pos+2]='l'; buffer[pos+3]='l';
        }
        pos += 4;
    } else {
        while (*s != '\0') {
            char c = *s++;
            if (c == '"' || c == '\\') {
                if (buffer != nullptr && pos + 2 < buffer_size) {
                    buffer[pos] = '\\'; buffer[pos+1] = c;
                }
                pos += 2;
            } else {
                if (buffer != nullptr && pos + 1 < buffer_size) buffer[pos] = c;
                ++pos;
            }
        }
    }
    if (buffer != nullptr && pos + 1 < buffer_size) buffer[pos] = '"';
    ++pos;
    return pos;
}
} // namespace detail

// ---------------------------------------------------------------------------
// compare_modules
// Format: one field per line, "prefix.field_name: value_a | value_b".
// Prefixes: "hw." for HardwareSpec, "feat." for FeatureFlags,
//           "at." for ATCommandDeltas, "" for name.
// Field order matches the order of declaration in ModuleSpec.
// If a == nullptr or b == nullptr, the string "null" is used as the value.
// Returns: number of characters written (excluding null terminator).
//          If buffer_size == 0, returns the number of characters needed.
// ---------------------------------------------------------------------------
inline std::size_t compare_modules(
    const ModuleSpec* a, const ModuleSpec* b,
    char* buffer, std::size_t buffer_size) noexcept
{
    using namespace detail;
    std::size_t p = 0;
    if (a == nullptr || b == nullptr) {
        p = append_str(buffer, buffer_size, p, "null");
        if (buffer != nullptr && p < buffer_size) buffer[p] = '\0';
        return p;
    }

    #define LINE_STR(prefix, field, va, vb) do { \
        p = append_str(buffer, buffer_size, p, prefix "." field ": "); \
        p = append_str(buffer, buffer_size, p, (va) ? (va) : "null"); \
        p = append_str(buffer, buffer_size, p, " | "); \
        p = append_str(buffer, buffer_size, p, (vb) ? (vb) : "null"); \
        p = append_str(buffer, buffer_size, p, "\n"); \
    } while (0)
    #define LINE_U(prefix, field, va, vb) do { \
        p = append_str(buffer, buffer_size, p, prefix "." field ": "); \
        p = append_u64(buffer, buffer_size, p, (unsigned long long)(va)); \
        p = append_str(buffer, buffer_size, p, " | "); \
        p = append_u64(buffer, buffer_size, p, (unsigned long long)(vb)); \
        p = append_str(buffer, buffer_size, p, "\n"); \
    } while (0)
    #define LINE_I(prefix, field, va, vb) do { \
        p = append_str(buffer, buffer_size, p, prefix "." field ": "); \
        p = append_i64(buffer, buffer_size, p, (long long)(va)); \
        p = append_str(buffer, buffer_size, p, " | "); \
        p = append_i64(buffer, buffer_size, p, (long long)(vb)); \
        p = append_str(buffer, buffer_size, p, "\n"); \
    } while (0)
    #define LINE_B(prefix, field, va, vb) do { \
        p = append_str(buffer, buffer_size, p, prefix "." field ": "); \
        p = append_bool(buffer, buffer_size, p, (va)); \
        p = append_str(buffer, buffer_size, p, " | "); \
        p = append_bool(buffer, buffer_size, p, (vb)); \
        p = append_str(buffer, buffer_size, p, "\n"); \
    } while (0)

    p = append_str(buffer, buffer_size, p, "name: ");
    p = append_str(buffer, buffer_size, p, a->name ? a->name : "null");
    p = append_str(buffer, buffer_size, p, " | ");
    p = append_str(buffer, buffer_size, p, b->name ? b->name : "null");
    p = append_str(buffer, buffer_size, p, "\n");
    LINE_STR("hw",   "package_type",       a->hw.package_type,    b->hw.package_type);
    LINE_U  ("hw",   "pin_count",          a->hw.pin_count,       b->hw.pin_count);
    LINE_U  ("hw",   "voltage_min_mv",     a->hw.voltage_min_mv,  b->hw.voltage_min_mv);
    LINE_U  ("hw",   "voltage_max_mv",     a->hw.voltage_max_mv,  b->hw.voltage_max_mv);
    LINE_U  ("hw",   "flash_kb",           a->hw.flash_kb,        b->hw.flash_kb);
    LINE_U  ("hw",   "length_mm",          a->hw.length_mm,       b->hw.length_mm);
    LINE_U  ("hw",   "width_mm",           a->hw.width_mm,        b->hw.width_mm);
    LINE_U  ("hw",   "height_mm",          a->hw.height_mm,       b->hw.height_mm);
    LINE_U  ("hw",   "weight_mg",          a->hw.weight_mg,       b->hw.weight_mg);
    LINE_I  ("hw",   "operating_temp_min_c", a->hw.operating_temp_min_c, b->hw.operating_temp_min_c);
    LINE_I  ("hw",   "operating_temp_max_c", a->hw.operating_temp_max_c, b->hw.operating_temp_max_c);
    LINE_U  ("hw",   "current_sleep_ua",   a->hw.current_sleep_ua, b->hw.current_sleep_ua);
    LINE_U  ("hw",   "current_peak_ma",    a->hw.current_peak_ma,  b->hw.current_peak_ma);
    LINE_U  ("hw",   "gprs_class",         a->hw.gprs_class,       b->hw.gprs_class);
    LINE_U  ("hw",   "gprs_downlink_kbps", a->hw.gprs_downlink_kbps, b->hw.gprs_downlink_kbps);
    LINE_U  ("hw",   "gprs_uplink_kbps",   a->hw.gprs_uplink_kbps, b->hw.gprs_uplink_kbps);
    LINE_U  ("hw",   "baud_rate_min",      a->hw.baud_rate_min,    b->hw.baud_rate_min);
    LINE_U  ("hw",   "baud_rate_max",      a->hw.baud_rate_max,    b->hw.baud_rate_max);
    LINE_STR("hw",   "sim_card_type",      to_string(a->hw.sim_card_type), to_string(b->hw.sim_card_type));
    LINE_STR("hw",   "antenna_connector",  to_string(a->hw.antenna_connector), to_string(b->hw.antenna_connector));
    LINE_U  ("hw",   "uart_count",         a->hw.uart_count,       b->hw.uart_count);
    LINE_B  ("hw",   "usb_support",        a->hw.usb_support,      b->hw.usb_support);
    LINE_B  ("hw",   "rtc_backup",         a->hw.rtc_backup,       b->hw.rtc_backup);
    LINE_STR("hw",   "network_status_pin", a->hw.network_status_pin, b->hw.network_status_pin);
    LINE_STR("hw",   "operating_status_pin", a->hw.operating_status_pin, b->hw.operating_status_pin);

    LINE_B("feat", "has_bluetooth",  a->feat.has_bluetooth,  b->feat.has_bluetooth);
    LINE_B("feat", "has_gps",        a->feat.has_gps,        b->feat.has_gps);
    LINE_B("feat", "has_fm",         a->feat.has_fm,         b->feat.has_fm);
    LINE_B("feat", "has_audio",      a->feat.has_audio,      b->feat.has_audio);
    LINE_B("feat", "has_keypad",     a->feat.has_keypad,     b->feat.has_keypad);
    LINE_B("feat", "has_sd",         a->feat.has_sd,         b->feat.has_sd);
    LINE_B("feat", "has_pcm",        a->feat.has_pcm,        b->feat.has_pcm);
    LINE_B("feat", "has_i2c",        a->feat.has_i2c,        b->feat.has_i2c);
    LINE_U("feat", "gpio_count",     a->feat.gpio_count,     b->feat.gpio_count);
    LINE_U("feat", "adc_count",      a->feat.adc_count,      b->feat.adc_count);
    LINE_U("feat", "pwm_count",      a->feat.pwm_count,      b->feat.pwm_count);
    LINE_B("feat", "has_uart",       a->feat.has_uart,       b->feat.has_uart);
    LINE_B("feat", "has_usb",        a->feat.has_usb,        b->feat.has_usb);
    LINE_B("feat", "has_rtc",        a->feat.has_rtc,        b->feat.has_rtc);
    LINE_B("feat", "has_kpled",      a->feat.has_kpled,      b->feat.has_kpled);
    LINE_B("feat", "has_rf_sync",    a->feat.has_rf_sync,    b->feat.has_rf_sync);
    LINE_B("feat", "has_antenna_gps",a->feat.has_antenna_gps,b->feat.has_antenna_gps);
    LINE_B("feat", "has_antenna_bt", a->feat.has_antenna_bt, b->feat.has_antenna_bt);
    LINE_B("feat", "has_tdd",        a->feat.has_tdd,        b->feat.has_tdd);

    LINE_B("at", "cband_quad_band",   a->at.cband_quad_band,   b->at.cband_quad_band);
    LINE_U("at", "cmic_channels",     a->at.cmic_channels,     b->at.cmic_channels);
    LINE_U("at", "sidet_channels",    a->at.sidet_channels,    b->at.sidet_channels);
    LINE_B("at", "supports_csclk2",   a->at.supports_csclk2,   b->at.supports_csclk2);
    LINE_U("at", "cfgri_default",     a->at.cfgri_default,     b->at.cfgri_default);
    LINE_B("at", "chfa_pcm_support",  a->at.chfa_pcm_support,  b->at.chfa_pcm_support);
    LINE_STR("at", "jamming_pin",     a->at.jamming_pin,       b->at.jamming_pin);
    LINE_STR("at", "extra_note",      a->at.extra_note,        b->at.extra_note);
    LINE_STR("at", "jamming_pin_enum",to_string(a->at.jamming_pin_enum), to_string(b->at.jamming_pin_enum));

    #undef LINE_STR
    #undef LINE_U
    #undef LINE_I
    #undef LINE_B

    if (buffer != nullptr && p < buffer_size) buffer[p] = '\0';
    return p;
}

// ---------------------------------------------------------------------------
// module_to_json
// Top-level format: {"name":"...","hw":{...},"feat":{...},"at":{...}}
// Key order exactly matches the declaration order in ModuleSpec.
// nullptr fields are rendered as null.
// Empty string fields are rendered as "".
// Enums are rendered as strings via to_string.
// Returns: number of characters written (excluding null terminator).
//          If buffer_size == 0, returns the number of characters needed.
//          If m == nullptr, writes "null".
// ---------------------------------------------------------------------------
inline std::size_t module_to_json(
    const ModuleSpec* m,
    char* buffer, std::size_t buffer_size) noexcept
{
    using namespace detail;
    std::size_t p = 0;
    if (m == nullptr) {
        p = append_str(buffer, buffer_size, p, "null");
        if (buffer != nullptr && p < buffer_size) buffer[p] = '\0';
        return p;
    }

    p = append_str(buffer, buffer_size, p, "{\"name\":");
    p = append_json_str(buffer, buffer_size, p, m->name);

    p = append_str(buffer, buffer_size, p, ",\"hw\":{");
    p = append_str(buffer, buffer_size, p, "\"package_type\":"); p = append_json_str(buffer, buffer_size, p, m->hw.package_type);
    p = append_str(buffer, buffer_size, p, ",\"pin_count\":"); p = append_u64(buffer, buffer_size, p, m->hw.pin_count);
    p = append_str(buffer, buffer_size, p, ",\"voltage_min_mv\":"); p = append_u64(buffer, buffer_size, p, m->hw.voltage_min_mv);
    p = append_str(buffer, buffer_size, p, ",\"voltage_max_mv\":"); p = append_u64(buffer, buffer_size, p, m->hw.voltage_max_mv);
    p = append_str(buffer, buffer_size, p, ",\"flash_kb\":"); p = append_u64(buffer, buffer_size, p, m->hw.flash_kb);
    p = append_str(buffer, buffer_size, p, ",\"length_mm\":"); p = append_u64(buffer, buffer_size, p, m->hw.length_mm);
    p = append_str(buffer, buffer_size, p, ",\"width_mm\":"); p = append_u64(buffer, buffer_size, p, m->hw.width_mm);
    p = append_str(buffer, buffer_size, p, ",\"height_mm\":"); p = append_u64(buffer, buffer_size, p, m->hw.height_mm);
    p = append_str(buffer, buffer_size, p, ",\"weight_mg\":"); p = append_u64(buffer, buffer_size, p, m->hw.weight_mg);
    p = append_str(buffer, buffer_size, p, ",\"operating_temp_min_c\":"); p = append_i64(buffer, buffer_size, p, m->hw.operating_temp_min_c);
    p = append_str(buffer, buffer_size, p, ",\"operating_temp_max_c\":"); p = append_i64(buffer, buffer_size, p, m->hw.operating_temp_max_c);
    p = append_str(buffer, buffer_size, p, ",\"current_sleep_ua\":"); p = append_u64(buffer, buffer_size, p, m->hw.current_sleep_ua);
    p = append_str(buffer, buffer_size, p, ",\"current_peak_ma\":"); p = append_u64(buffer, buffer_size, p, m->hw.current_peak_ma);
    p = append_str(buffer, buffer_size, p, ",\"gprs_class\":"); p = append_u64(buffer, buffer_size, p, m->hw.gprs_class);
    p = append_str(buffer, buffer_size, p, ",\"gprs_downlink_kbps\":"); p = append_u64(buffer, buffer_size, p, m->hw.gprs_downlink_kbps);
    p = append_str(buffer, buffer_size, p, ",\"gprs_uplink_kbps\":"); p = append_u64(buffer, buffer_size, p, m->hw.gprs_uplink_kbps);
    p = append_str(buffer, buffer_size, p, ",\"baud_rate_min\":"); p = append_u64(buffer, buffer_size, p, m->hw.baud_rate_min);
    p = append_str(buffer, buffer_size, p, ",\"baud_rate_max\":"); p = append_u64(buffer, buffer_size, p, m->hw.baud_rate_max);
    p = append_str(buffer, buffer_size, p, ",\"sim_card_type\":"); p = append_json_str(buffer, buffer_size, p, to_string(m->hw.sim_card_type));
    p = append_str(buffer, buffer_size, p, ",\"antenna_connector\":"); p = append_json_str(buffer, buffer_size, p, to_string(m->hw.antenna_connector));
    p = append_str(buffer, buffer_size, p, ",\"uart_count\":"); p = append_u64(buffer, buffer_size, p, m->hw.uart_count);
    p = append_str(buffer, buffer_size, p, ",\"usb_support\":"); p = append_bool(buffer, buffer_size, p, m->hw.usb_support);
    p = append_str(buffer, buffer_size, p, ",\"rtc_backup\":"); p = append_bool(buffer, buffer_size, p, m->hw.rtc_backup);
    p = append_str(buffer, buffer_size, p, ",\"network_status_pin\":"); p = append_json_str(buffer, buffer_size, p, m->hw.network_status_pin);
    p = append_str(buffer, buffer_size, p, ",\"operating_status_pin\":"); p = append_json_str(buffer, buffer_size, p, m->hw.operating_status_pin);
    p = append_str(buffer, buffer_size, p, "}");

    p = append_str(buffer, buffer_size, p, ",\"feat\":{");
    p = append_str(buffer, buffer_size, p, "\"has_bluetooth\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_bluetooth);
    p = append_str(buffer, buffer_size, p, ",\"has_gps\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_gps);
    p = append_str(buffer, buffer_size, p, ",\"has_fm\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_fm);
    p = append_str(buffer, buffer_size, p, ",\"has_audio\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_audio);
    p = append_str(buffer, buffer_size, p, ",\"has_keypad\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_keypad);
    p = append_str(buffer, buffer_size, p, ",\"has_sd\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_sd);
    p = append_str(buffer, buffer_size, p, ",\"has_pcm\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_pcm);
    p = append_str(buffer, buffer_size, p, ",\"has_i2c\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_i2c);
    p = append_str(buffer, buffer_size, p, ",\"gpio_count\":"); p = append_u64(buffer, buffer_size, p, m->feat.gpio_count);
    p = append_str(buffer, buffer_size, p, ",\"adc_count\":"); p = append_u64(buffer, buffer_size, p, m->feat.adc_count);
    p = append_str(buffer, buffer_size, p, ",\"pwm_count\":"); p = append_u64(buffer, buffer_size, p, m->feat.pwm_count);
    p = append_str(buffer, buffer_size, p, ",\"has_uart\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_uart);
    p = append_str(buffer, buffer_size, p, ",\"has_usb\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_usb);
    p = append_str(buffer, buffer_size, p, ",\"has_rtc\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_rtc);
    p = append_str(buffer, buffer_size, p, ",\"has_kpled\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_kpled);
    p = append_str(buffer, buffer_size, p, ",\"has_rf_sync\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_rf_sync);
    p = append_str(buffer, buffer_size, p, ",\"has_antenna_gps\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_antenna_gps);
    p = append_str(buffer, buffer_size, p, ",\"has_antenna_bt\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_antenna_bt);
    p = append_str(buffer, buffer_size, p, ",\"has_tdd\":"); p = append_bool(buffer, buffer_size, p, m->feat.has_tdd);
    p = append_str(buffer, buffer_size, p, "}");

    p = append_str(buffer, buffer_size, p, ",\"at\":{");
    p = append_str(buffer, buffer_size, p, "\"cband_quad_band\":"); p = append_bool(buffer, buffer_size, p, m->at.cband_quad_band);
    p = append_str(buffer, buffer_size, p, ",\"cmic_channels\":"); p = append_u64(buffer, buffer_size, p, m->at.cmic_channels);
    p = append_str(buffer, buffer_size, p, ",\"sidet_channels\":"); p = append_u64(buffer, buffer_size, p, m->at.sidet_channels);
    p = append_str(buffer, buffer_size, p, ",\"supports_csclk2\":"); p = append_bool(buffer, buffer_size, p, m->at.supports_csclk2);
    p = append_str(buffer, buffer_size, p, ",\"cfgri_default\":"); p = append_u64(buffer, buffer_size, p, m->at.cfgri_default);
    p = append_str(buffer, buffer_size, p, ",\"chfa_pcm_support\":"); p = append_bool(buffer, buffer_size, p, m->at.chfa_pcm_support);
    p = append_str(buffer, buffer_size, p, ",\"jamming_pin\":"); p = append_json_str(buffer, buffer_size, p, m->at.jamming_pin);
    p = append_str(buffer, buffer_size, p, ",\"extra_note\":"); p = append_json_str(buffer, buffer_size, p, m->at.extra_note);
    p = append_str(buffer, buffer_size, p, ",\"jamming_pin_enum\":"); p = append_json_str(buffer, buffer_size, p, to_string(m->at.jamming_pin_enum));
    p = append_str(buffer, buffer_size, p, "}");

    p = append_str(buffer, buffer_size, p, "}");

    if (buffer != nullptr && p < buffer_size) buffer[p] = '\0';
    return p;
}



// =============================================================================
// Phase 10 additions: aggregations, feature finders, hardware finders
// =============================================================================

// ---------------------------------------------------------------------------
// Feature finders (16 shortcuts) - generated by macro.
// Rule for all of them:
//   if results == nullptr: return count only (do not write).
//   if results != nullptr: write up to max_results, return total count.
// ---------------------------------------------------------------------------
#define SIMCOM_DEFINE_FIND_BY_FEATURE(fn_name, field_name)                 \
    inline std::size_t fn_name(                                            \
        const ModuleSpec** results, std::size_t max_results) noexcept      \
    {                                                                      \
        std::size_t total = 0;                                             \
        for (std::size_t i = 0; i < MODULE_COUNT; ++i) {                   \
            if (MODULES[i].feat.field_name) {                              \
                if (results != nullptr && total < max_results) {           \
                    results[total] = &MODULES[i];                          \
                }                                                          \
                ++total;                                                   \
            }                                                              \
        }                                                                  \
        return total;                                                      \
    }

SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_bluetooth,   has_bluetooth)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_gps,         has_gps)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_fm,          has_fm)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_audio,       has_audio)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_keypad,      has_keypad)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_sd,          has_sd)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_pcm,         has_pcm)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_i2c,         has_i2c)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_uart,        has_uart)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_usb,         has_usb)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_rtc,         has_rtc)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_kpled,       has_kpled)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_rf_sync,     has_rf_sync)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_antenna_gps, has_antenna_gps)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_antenna_bt,  has_antenna_bt)
SIMCOM_DEFINE_FIND_BY_FEATURE(find_modules_with_tdd,         has_tdd)

#undef SIMCOM_DEFINE_FIND_BY_FEATURE

// ---------------------------------------------------------------------------
// find_modules_by_voltage
//   if results == nullptr: return count only.
//   if results != nullptr: write up to max_results, return total.
// ---------------------------------------------------------------------------
inline std::size_t find_modules_by_voltage(
    uint16_t min_mv, uint16_t max_mv,
    const ModuleSpec** results, std::size_t max_results) noexcept
{
    std::size_t total = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.voltage_min_mv >= min_mv &&
            MODULES[i].hw.voltage_max_mv <= max_mv) {
            if (results != nullptr && total < max_results) {
                results[total] = &MODULES[i];
            }
            ++total;
        }
    }
    return total;
}

// ---------------------------------------------------------------------------
// find_modules_by_pin_count
//   if results == nullptr: return count only.
//   if results != nullptr: write up to max_results, return total.
// ---------------------------------------------------------------------------
inline std::size_t find_modules_by_pin_count(
    uint16_t min_pins, uint16_t max_pins,
    const ModuleSpec** results, std::size_t max_results) noexcept
{
    std::size_t total = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.pin_count >= min_pins &&
            MODULES[i].hw.pin_count <= max_pins) {
            if (results != nullptr && total < max_results) {
                results[total] = &MODULES[i];
            }
            ++total;
        }
    }
    return total;
}

// ---------------------------------------------------------------------------
// find_modules_by_package_type
//   if results == nullptr: return count only.
//   if results != nullptr: write up to max_results, return total.
// ---------------------------------------------------------------------------
inline std::size_t find_modules_by_package_type(
    const char* package_type,
    const ModuleSpec** results, std::size_t max_results) noexcept
{
    std::size_t total = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (str_equal(MODULES[i].hw.package_type, package_type)) {
            if (results != nullptr && total < max_results) {
                results[total] = &MODULES[i];
            }
            ++total;
        }
    }
    return total;
}

// ---------------------------------------------------------------------------
// find_modules_by_flash_range
//   if results == nullptr: return count only.
//   if results != nullptr: write up to max_results, return total.
// ---------------------------------------------------------------------------
inline std::size_t find_modules_by_flash_range(
    uint32_t min_kb, uint32_t max_kb,
    const ModuleSpec** results, std::size_t max_results) noexcept
{
    std::size_t total = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.flash_kb >= min_kb &&
            MODULES[i].hw.flash_kb <= max_kb) {
            if (results != nullptr && total < max_results) {
                results[total] = &MODULES[i];
            }
            ++total;
        }
    }
    return total;
}

// ---------------------------------------------------------------------------
// find_modules_by_sim_card_type
//   if results == nullptr: return count only.
//   if results != nullptr: write up to max_results, return total.
// ---------------------------------------------------------------------------
inline std::size_t find_modules_by_sim_card_type(
    SimCardType type,
    const ModuleSpec** results, std::size_t max_results) noexcept
{
    std::size_t total = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.sim_card_type == type) {
            if (results != nullptr && total < max_results) {
                results[total] = &MODULES[i];
            }
            ++total;
        }
    }
    return total;
}

// ---------------------------------------------------------------------------
// find_modules_by_antenna_type
//   if results == nullptr: return count only.
//   if results != nullptr: write up to max_results, return total.
// ---------------------------------------------------------------------------
inline std::size_t find_modules_by_antenna_type(
    AntennaType type,
    const ModuleSpec** results, std::size_t max_results) noexcept
{
    std::size_t total = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.antenna_connector == type) {
            if (results != nullptr && total < max_results) {
                results[total] = &MODULES[i];
            }
            ++total;
        }
    }
    return total;
}

// ---------------------------------------------------------------------------
// find_modules_by_jamming_pin
//   if results == nullptr: return count only.
//   if results != nullptr: write up to max_results, return total.
// ---------------------------------------------------------------------------
inline std::size_t find_modules_by_jamming_pin(
    JammingPin pin,
    const ModuleSpec** results, std::size_t max_results) noexcept
{
    std::size_t total = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].at.jamming_pin_enum == pin) {
            if (results != nullptr && total < max_results) {
                results[total] = &MODULES[i];
            }
            ++total;
        }
    }
    return total;
}

// ---------------------------------------------------------------------------
// Aggregations
// ---------------------------------------------------------------------------
inline uint16_t get_min_voltage_mv() noexcept {
    uint16_t v = 0xFFFF;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.voltage_min_mv < v) v = MODULES[i].hw.voltage_min_mv;
    }
    return v;
}

inline uint16_t get_max_voltage_mv() noexcept {
    uint16_t v = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.voltage_max_mv > v) v = MODULES[i].hw.voltage_max_mv;
    }
    return v;
}

inline uint16_t get_min_pin_count() noexcept {
    uint16_t v = 0xFFFF;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.pin_count < v) v = MODULES[i].hw.pin_count;
    }
    return v;
}

inline uint16_t get_max_pin_count() noexcept {
    uint16_t v = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.pin_count > v) v = MODULES[i].hw.pin_count;
    }
    return v;
}

// Sum of flash only for modules whose flash_kb > 0 (value is known).
inline uint32_t get_sum_of_known_flash_kb() noexcept {
    uint32_t sum = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        sum += MODULES[i].hw.flash_kb;
    }
    return sum;
}

// Number of modules whose flash_kb == 0 (value unknown).
inline std::size_t count_modules_with_unknown_flash() noexcept {
    std::size_t n = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].hw.flash_kb == 0) ++n;
    }
    return n;
}

inline std::size_t count_modules_with_quad_band() noexcept {
    std::size_t n = 0;
    for (std::size_t i = 0; i < MODULE_COUNT; ++i) {
        if (MODULES[i].at.cband_quad_band) ++n;
    }
    return n;
}

} // namespace simcom

#endif // SIMCOM_API_HPP

// SPDX-License-Identifier: MIT