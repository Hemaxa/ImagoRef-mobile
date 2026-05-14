# ImagoRef Mobile

ImagoRef Mobile is the mobile companion client for the ImagoRef ecosystem. It focuses on cloud board access, previewing synced content, and keeping the product language aligned with the desktop editor.

## Highlights

- cloud sign-in with the same account as desktop
- synced board list and board previews
- mobile-friendly board viewing
- shared visual identity with the desktop `Imago` theme
- platform packaging metadata for Android and iOS
- GitHub Actions release automation on `main`

## Repository Layout

- `src/` — C++ app state and QML UI
- `res/` — themes and runtime assets
- `packs/` — Android and iOS packaging metadata, icons, and bundle config
- `docs/` — repository documentation and notices
- `.github/workflows/` — release workflows

## Local Build

```bash
cmake -S . -B build
cmake --build build
```

## Release Automation

Pushes to `main` trigger the mobile release workflow. It currently publishes:

- Android debug APK for direct device testing
- iOS Simulator zip for CI validation

For signed iPhone installs and App Store distribution, Apple signing assets must be supplied separately.

## Documentation

See [docs/README.md](docs/README.md) for the documentation index.
