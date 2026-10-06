# Schema Evolution

## Current version: 1

All four YAML files (`modules.yaml`, `at_deltas.yaml`, `version_deltas.yaml`,
`cross_references.yaml`) declare `schema_version: 1`.

## Migration policy

`schema_version` is bumped only when the YAML structure changes in a
backward-incompatible way. Field additions are not a version bump.

## Example migration to version 2

Hypothetical: renaming `jamming_pin` to `jamming_pin_id`.

1. Bump `schema_version: 2` in all four files.
2. Add a migration function in `tools/migrate_yaml.py` (future work).
3. Update `enum_mappings.py` accordingly.
4. Update `validate_yaml.py` and `generate_cpp.py`.
5. Regenerate `include/simcom/simcom_database.hpp`.