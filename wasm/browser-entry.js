// Entry point for bundlers/browsers (wired via package.json's "exports"
// "browser" condition). Deliberately a separate file from index.js rather
// than a runtime branch: bundlers statically choke on index.js's
// require('../build/Release/cld') even inside an unreachable branch, since
// a .node file can't be resolved at bundle time. This file never attempts
// the native path at all -- it goes straight to the WASM backend.

import createCldModule from './dist/cld.web.mjs';
import meta from '../lib/metadata.json';
import { createDetect } from '../lib/detect-shape.js';
import { wrapWasmModule } from '../lib/wasm-wrap.js';

let modulePromise = null;
function loadBackend() {
  if (!modulePromise) {
    modulePromise = createCldModule().then(wrapWasmModule);
  }
  return modulePromise;
}

export const LANGUAGES = meta.LANGUAGES;
export const DETECTED_LANGUAGES = meta.DETECTED_LANGUAGES;
export const ENCODINGS = meta.ENCODINGS;
export const detect = createDetect(loadBackend, meta);
