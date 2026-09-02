#!/usr/bin/env node
// Dev-only, maintainer-run script -- NOT part of npm install/publish.
//
// Captures a golden snapshot of detect() results from the native build,
// for test/runner-wasm.js to compare the WASM backend against. This is
// deliberately a fixed, checked-in snapshot rather than a live comparison
// against "whatever native build happens to be present" -- native builds
// on different platforms/compilers (verified: MSVC vs GCC/Clang) can
// produce different scores/chunk boundaries for the same CLD2 source, a
// pre-existing quirk in CLD2's own algorithm unrelated to the WASM port.
// The WASM build is proven to match native exactly when built from the
// same compiler family (Clang, via emcc) as this snapshot was captured
// with, so comparing against a fixed snapshot -- rather than trusting
// whatever native binary a given CI runner happens to produce -- is what
// actually makes this test portable across platforms.
//
// Re-run this (`node scripts/generate-wasm-fixtures.js`) only if
// test/data.js's fixtures change. Requires a native build to exist first
// (`npm run build`), built on a GCC/Clang-family toolchain (Linux/macOS).

const fs = require('fs');
const path = require('path');

const data = require('../test/data');
const meta = require('../lib/metadata.json');
const { createDetect } = require('../lib/detect-shape');

const native = require('../build/Release/cld');
const detect = createDetect(async () => native, meta);

(async () => {
  const fixtures = [];
  for (const item of data.all) {
    fixtures.push({
      default: await detect(item.sample),
      bestEffort: await detect(item.sample, { bestEffort: true })
    });
  }

  const outPath = path.resolve(__dirname, '..', 'test', 'wasm-fixtures.json');
  fs.writeFileSync(outPath, JSON.stringify(fixtures, null, 2) + '\n');
  console.log(`Wrote ${outPath} (${fixtures.length} fixtures)`);
})();
