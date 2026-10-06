# API Reference

The public API lives in `include/simcom/simcom.hpp`. All functions are
`inline` or `inline constexpr`, noexcept, and use no dynamic allocation.

## Lookup

- `lookup_by_name(const char*) -> const ModuleSpec*`
- `lookup_by_index(std::size_t) -> const ModuleSpec*`
- `total_modules() -> std::size_t`
- `get_all() -> const ModuleSpec*`

## Feature finders

For each of the 16 features (bluetooth, gps, fm, audio, keypad, sd, pcm,
i2c, uart, usb, rtc, kpled, rf_sync, antenna_gps, antenna_bt, tdd):

    find_modules_with_<feature>(const ModuleSpec** results,
                                std::size_t max_results) -> std::size_t

Rule (all find_* functions): if `results == nullptr`, returns count only;
otherwise writes up to `max_results` pointers and returns total count.

## Hardware finders

- `find_modules_by_voltage(uint16_t min_mv, uint16_t max_mv, ...)`
- `find_modules_by_pin_count(uint16_t min_pins, uint16_t max_pins, ...)`
- `find_modules_by_package_type(const char*, ...)`
- `find_modules_by_flash_range(uint32_t min_kb, uint32_t max_kb, ...)`
- `find_modules_by_sim_card_type(SimCardType, ...)`
- `find_modules_by_antenna_type(AntennaType, ...)`
- `find_modules_by_jamming_pin(JammingPin, ...)`

## Aggregations

- `get_min_voltage_mv() / get_max_voltage_mv()`
- `get_min_pin_count() / get_max_pin_count()`
- `get_sum_of_known_flash_kb()` (excludes unknown=0)
- `count_modules_with_unknown_flash()`
- `count_modules_with_quad_band()`

## Serialisation

- `module_to_json(const ModuleSpec*, char* buf, std::size_t size)`
- `compare_modules(const ModuleSpec* a, const ModuleSpec* b, char*, std::size_t)`

Both return characters written (or required when `size == 0`) and never
allocate.