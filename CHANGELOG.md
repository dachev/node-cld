# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

This fork ([`cld-universal`](https://github.com/figulusproject/cld-universal)) diverged from
[`dachev/node-cld`](https://github.com/dachev/node-cld) at `2.10.1`. Entries below start from that
point.

## [2.11.1] - 2026-08-03

### Added

- `setWasmModuleOptions({ locateFile })` in the browser entry point, letting bundlers/apps override
  where the WASM backend fetches `cld.web.wasm` from when it isn't served at the default relative
  path.

### Fixed

- Browser entry point's `metadata.json` import now uses the `with { type: 'json' }` attribute
  required by Node's ESM loader.

### Security

- Added a CodeQL Advanced workflow (`codeql.yml`) analyzing the `actions`, `c-cpp`, and
  `javascript-typescript` languages on push/PR to `main` and weekly on a schedule.
- Added a `security.yml` workflow running `npm audit --audit-level=high` against the package's
  npm dependencies on push/PR to `main` and weekly on a schedule, separate from the CodeQL
  workflow above.
- Restricted the default `GITHUB_TOKEN` permissions to `contents: read` on the `ci.yml` and
  `security.yml` workflows.

## [2.11.0] - 2026-08-02

### Added

- WASM fallback backend: the native addon is tried first, and the package now falls back to a
  bundled WASM build (built via Emscripten from the same CLD2 sources) when the native addon is
  unavailable, including in the browser and in environments without a C++ toolchain.
- GitHub Actions CI covering both the native and WASM backends across Node 20/22, replacing the
  old Travis/AppVeyor setup.
- npm publish workflow.

### Changed

- Package renamed from `cld` to `cld-universal`; metadata, description, keywords, and
  repository/bugs/homepage links updated to point at this fork.
- README rewritten to document the fork's scope, maintenance expectations, and the WASM fallback.
- Dropped support for Node 8/10/12; `engines` now requires `20 || >=22`.
- `postinstall.js` updated to use `globSync` (required by the `glob` upgrade below).
- Install step (`bin/build-native.js`) now always exits `0` after attempting `node-gyp rebuild`, so
  a missing or broken C++ toolchain no longer fails `npm install` before the WASM fallback is
  reachable.
- `package.json` now declares an explicit `files` field (previously unset, so publishing silently
  fell back to `.gitignore` rules).

### Fixed

- Missing emsdk setup step in the publish workflow.
- AppVeyor/Windows (MSVC) build failures: pinned `node-addon-api` to `^2.0.0`, moved
  `NODE_API_MODULE` out of the `NodeCld` namespace, and bumped the CI image to Visual Studio 2022.

### Security

- Updated `glob` from `7.x` to `^12` (fixes 2 high severity advisories, including the
  brace-expansion DoS in [GHSA-mh99-v99m-4gvg](https://github.com/advisories/GHSA-mh99-v99m-4gvg))
  while retaining support for Node < 20.
- Updated `underscore` from `^1.12.1` to `^1.13.7` (fixes a high severity advisory).
- Updated `minimatch` from `3.0.4` to `3.1.2`.
- Migrated the lockfile from v1 to v3.
