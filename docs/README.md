# ImagoRef Mobile Docs

This directory contains the operational documentation for the mobile client.

## Contents

- `README.md` — documentation index
- `CONTRIBUTING.md` — contribution and change guidelines
- `NOTICE.md` — third-party notices
- `LICENSE` — repository license

## Project Structure

- `src/` — C++ managers and QML UI
- `res/` — themes and visual assets
- `packs/` — mobile packaging metadata for Android and iOS
- `.github/workflows/` — release automation

## Release Notes

The mobile release workflow builds:

- Android debug APK for device-side testing
- iOS Simulator archive for validation on macOS runners

Signed iPhone-ready `.ipa` packaging still requires Apple signing material and should be added through repository secrets later.
