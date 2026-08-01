// Resolves which compiled backend (native N-API addon, or the WASM
// fallback) actually runs detectAsync(). Both backends expose the same
// {detectAsync(text, isPlainText, languageHint, encodingHint, tldHint,
// httpHint, bestEffort)} shape, so index.js needs no branching beyond
// awaiting loadBackend() once.

const { wrapWasmModule } = require('./wasm-wrap');

let backendPromise = null;

function loadBackend() {
  if (!backendPromise) {
    backendPromise = (async () => {
      try {
        return require('../build/Release/cld');
      } catch (nativeErr) {
        return loadWasmBackend();
      }
    })();
  }
  return backendPromise;
}

async function loadWasmBackend() {
  const createCldModule = require('../wasm/dist/cld.node.js');
  const mod = await createCldModule();
  return wrapWasmModule(mod);
}

module.exports = { loadBackend, loadWasmBackend };
