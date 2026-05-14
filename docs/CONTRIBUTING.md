# Contributing to ImagoRef Mobile

Thank you for helping improve the mobile client.

## Focus

The mobile repository is the cloud-connected viewer companion for the desktop editor. Keep changes aligned with the desktop product language, sync contract, and packaging rules.

## Local Development

```bash
cmake -S . -B build
cmake --build build
```

## Change Guidelines

- preserve the existing `QML + C++` structure
- keep mobile visuals aligned with the desktop `Imago` theme
- prefer adding platform metadata under `packs/`
- keep long-form docs under `docs/`
- validate GitHub Actions changes on feature branches before merging to `main`

## Pull Requests

- explain the user-facing effect of the change
- call out any packaging or signing assumptions
- include screenshots for visual updates when possible
