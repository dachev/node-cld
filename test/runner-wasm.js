#!/usr/bin/env node
// Confirms the WASM backend produces results identical to the native
// backend for the same inputs -- expected since it's the same algorithm
// and compiled-in tables, just a different compile target. Requires both
// a native build (`npm run build`) and a WASM build
// (`npm run build:wasm`) to exist.

const assert = require('assert');
const data   = require('./data');
const meta   = require('../lib/metadata.json');

const { createDetect } = require('../lib/detect-shape');
const { loadWasmBackend } = require('../lib/backend');

const native = require('../build/Release/cld');
const detectNative = createDetect(async () => native, meta);

const wasmBackendPromise = loadWasmBackend(); // instantiate once, reuse for every fixture
const detectWasm = createDetect(() => wasmBackendPromise, meta);

(async () => {
  for (const item of data.all) {
    const nativeResult = await detectNative(item.sample);
    const wasmResult = await detectWasm(item.sample);
    assert.deepStrictEqual(
      wasmResult, nativeResult,
      `WASM/native mismatch for ${item.name} (default options)`
    );

    const nativeBestEffort = await detectNative(item.sample, { bestEffort: true });
    const wasmBestEffort = await detectWasm(item.sample, { bestEffort: true });
    assert.deepStrictEqual(
      wasmBestEffort, nativeBestEffort,
      `WASM/native mismatch for ${item.name} (bestEffort)`
    );
  }

  console.log(`WASM/native parity verified across ${data.all.length} fixtures`);
})();
