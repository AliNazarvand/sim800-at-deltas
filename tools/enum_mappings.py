#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Shared YAML -> C++ enum mappings used by generator and validator."""

from __future__ import annotations

JAMMING_PIN_MAP = {
    "None":  "JammingPin::NONE",
    "PIN5":  "JammingPin::PIN5",
    "PIN29": "JammingPin::PIN29",
    "PIN63": "JammingPin::PIN63",
    "PIN67": "JammingPin::PIN67",
}

SIM_CARD_TYPE_MAP = {
    "UNKNOWN":  "SimCardType::UNKNOWN",
    "MICRO":    "SimCardType::MICRO",
    "NANO":     "SimCardType::NANO",
    "STANDARD": "SimCardType::STANDARD",
}

ANTENNA_TYPE_MAP = {
    "UNKNOWN": "AntennaType::UNKNOWN",
    "UFL":     "AntennaType::UFL",
    "SPRING":  "AntennaType::SPRING",
    "SMA":     "AntennaType::SMA",
    "PCB":     "AntennaType::PCB",
}

AT_MANUAL_VERSION_MAP = {
    "V1.01": "AtManualVersion::V1_01",
    "V1.10": "AtManualVersion::V1_10",
    "V1.12": "AtManualVersion::V1_12",
    None:    "AtManualVersion::UNKNOWN",
}

SUPPORTED_SCHEMA_VERSION = 1
MODULE_ORDER = [
    "SIM800L", "SIM800C", "SIM808", "SIM868", "SIM800A",
    "SIM800F", "SIM800H", "SIM800", "SIM800C-DS",
]
