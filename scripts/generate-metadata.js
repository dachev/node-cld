#!/usr/bin/env node
// Dev-only script, NOT part of npm install/publish.
//
// LANGUAGES/DETECTED_LANGUAGES/ENCODINGS are pure compile-time-static data
// (from Constants::init() in src/constants.cc), identical regardless of
// which backend (native or WASM) ends up running detect(). Rather than
// have index.js depend on either backend just to read constant strings
// (WASM instantiation is async, which would force async metadata exports
// too), snapshot them once here into a checked-in JSON file that both
// backends' index.js consumers read synchronously.
//
// Re-run this (`node scripts/generate-metadata.js`) only if deps/cld's
// language/encoding tables are ever upgraded -- CLD2 upstream is frozen,
// so in practice this should rarely, if ever, need to run again. Requires a
// native build to exist first (`npm run build`).

const fs = require('fs');
const path = require('path');

const cld2 = require('../build/Release/cld');

const metadata = {
  LANGUAGES: cld2.LANGUAGES,
  DETECTED_LANGUAGES: cld2.DETECTED_LANGUAGES,
  ENCODINGS: cld2.ENCODINGS
};

const outPath = path.resolve(__dirname, '..', 'lib', 'metadata.json');
fs.writeFileSync(outPath, JSON.stringify(metadata, null, 2) + '\n');

console.log(`Wrote ${outPath}`);
console.log(`  LANGUAGES: ${Object.keys(metadata.LANGUAGES).length} entries`);
console.log(`  DETECTED_LANGUAGES: ${metadata.DETECTED_LANGUAGES.length} entries`);
console.log(`  ENCODINGS: ${metadata.ENCODINGS.length} entries`);
