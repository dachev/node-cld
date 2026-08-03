// Wraps an instantiated Emscripten module (Node or browser target -- both
// expose the same ccall/UTF8ToString shape) into the {detectAsync(...)}
// interface shared with the native backend. Deliberately has no other
// requires: wasm/browser-entry.mjs imports this directly (not through
// lib/backend.js) so bundlers never see lib/backend.js's
// require('../build/Release/cld') at all.

function wrapWasmModule(mod) {
  return {
    async detectAsync(text, isPlainText, languageHint, encodingHint, tldHint, httpHint, bestEffort) {
      const numBytes = new TextEncoder().encode(text).length;
      const ptr = mod.ccall(
        'cld_detect', 'number',
        ['string', 'number', 'number', 'string', 'string', 'string', 'string', 'number'],
        [text, numBytes, isPlainText ? 1 : 0, languageHint, encodingHint, tldHint, httpHint, bestEffort ? 1 : 0]
      );
      try {
        return JSON.parse(mod.UTF8ToString(ptr));
      } finally {
        // Must free explicitly: ccall's 'number' return type gives us the
        // raw pointer (not emscripten's 'string' convenience type, which
        // decodes the string but never frees the underlying C buffer).
        mod.ccall('cld_free', null, ['number'], [ptr]);
      }
    }
  };
}

module.exports = { wrapWasmModule };
