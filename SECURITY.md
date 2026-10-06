# Security Policy

## Supported Versions

| Version | Supported          |
|---------|--------------------|
| 1.1.x   | :white_check_mark: |
| 1.0.x   | :white_check_mark: |
| < 1.0   | :x:                |

## Reporting a Vulnerability

Please do **not** open a public GitHub issue for security vulnerabilities.

Instead, use one of these private channels:

1. GitHub's private vulnerability reporting:
   https://github.com/AliNazarvand/sim800-at-deltas/security/advisories/new
2. Email the maintainer at **ali.nazarvand@example.com**.

Please include:

- A description of the issue and its impact.
- Steps to reproduce (a minimal example is ideal).
- The affected version and, if applicable, the platform/compiler.
- Any suggested mitigation.

## Response Timeline

- **Acknowledgement:** within 72 hours.
- **Initial assessment:** within 7 days.
- **Fix or mitigation:** target within 30 days.

We will coordinate disclosure with you and credit you in the release notes
(unless you prefer to remain anonymous).

## Scope

This project is a **header-only data library**. The typical attack surface is:

- Out-of-bounds reads in module_to_json / compare_modules when the caller
  provides an incorrect buffer size.
- Memory safety in the test-only header 	est/support/mini_json.hpp.

The library makes no network calls, performs no file I/O, and has no runtime
dependencies.