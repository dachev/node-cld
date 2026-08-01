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

// `score` is excluded from the strict comparison below: it's the one field
// observed to differ between native builds compiled with different
// compilers (MSVC vs GCC/Clang) for the *same* CLD2 source -- verified on
// Linux, native (gcc/clang) and this WASM build (also clang, via emcc)
// match byte-for-byte including score across all fixtures; on Windows,
// native (MSVC) disagrees with both. That's a pre-existing MSVC-vs-Clang
// numerical quirk in CLD2's own scoring code, not something introduced by
// the WASM port, so it shouldn't fail this parity check. name/code/percent
// (the fields the public API and docs treat as meaningful) and chunks are
// still compared exactly.
function withoutScore(result) {
  return {
    ...result,
    languages: result.languages.map(({ score, ...rest }) => rest)
  };
}

(async () => {
  for (const item of data.all) {
    const nativeResult = await detectNative(item.sample);
    const wasmResult = await detectWasm(item.sample);
    assert.deepStrictEqual(
      withoutScore(wasmResult), withoutScore(nativeResult),
      `WASM/native mismatch for ${item.name} (default options)`
    );

    const nativeBestEffort = await detectNative(item.sample, { bestEffort: true });
    const wasmBestEffort = await detectWasm(item.sample, { bestEffort: true });
    assert.deepStrictEqual(
      withoutScore(wasmBestEffort), withoutScore(nativeBestEffort),
      `WASM/native mismatch for ${item.name} (bestEffort)`
    );
  }

  console.log(`WASM/native parity verified across ${data.all.length} fixtures`);
})();
