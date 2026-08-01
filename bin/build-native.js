#!/usr/bin/env node
// Runs as the package's "install" script. A failed native build here must
// NOT fail the overall `npm install` -- lib/backend.js falls back to the
// WASM backend at runtime when the native addon isn't present, but that
// fallback is unreachable if npm never finishes installing in the first
// place. So: try the native build, but always exit 0.

const { spawnSync } = require('child_process');

const result = spawnSync('node-gyp', ['rebuild'], {
  stdio: 'inherit',
  shell: true
});

if (result.status !== 0) {
  console.warn(
    '\n[cld] Native build failed (no working C++ toolchain for this ' +
    'platform?) -- this is not fatal. cld will use its WASM fallback ' +
    'backend at runtime instead of the native addon.\n'
  );
}

process.exit(0);
