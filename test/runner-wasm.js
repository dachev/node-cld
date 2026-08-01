#!/usr/bin/env node
// Confirms the WASM backend reproduces the reference CLD2 behavior,
// captured once as test/wasm-fixtures.json (see
// scripts/generate-wasm-fixtures.js for why this compares against a fixed
// snapshot rather than a live native build: native builds on different
// platforms/compilers -- verified: MSVC vs GCC/Clang -- can produce
// different results for the same CLD2 source and the same input, a
// pre-existing quirk in CLD2's own algorithm unrelated to the WASM port.
// Comparing against a fixed snapshot makes this test give the same answer
// on every platform, rather than depending on whatever native build a
// given CI runner happens to produce.

const assert = require('assert');
const data     = require('./data');
const fixtures = require('./wasm-fixtures.json');
const meta     = require('../lib/metadata.json');

const { createDetect } = require('../lib/detect-shape');
const { loadWasmBackend } = require('../lib/backend');

const wasmBackendPromise = loadWasmBackend(); // instantiate once, reuse for every fixture
const detectWasm = createDetect(() => wasmBackendPromise, meta);

(async () => {
  assert.equal(fixtures.length, data.all.length, 'test/wasm-fixtures.json is out of sync with test/data.js -- re-run scripts/generate-wasm-fixtures.js');

  for (let i = 0; i < data.all.length; i++) {
    const item = data.all[i];
    const expected = fixtures[i];

    const wasmResult = await detectWasm(item.sample);
    assert.deepStrictEqual(wasmResult, expected.default, `WASM mismatch for ${item.name} (default options)`);

    const wasmBestEffort = await detectWasm(item.sample, { bestEffort: true });
    assert.deepStrictEqual(wasmBestEffort, expected.bestEffort, `WASM mismatch for ${item.name} (bestEffort)`);
  }

  console.log(`WASM backend verified against ${data.all.length} reference fixtures`);
})();
