#!/usr/bin/env node
// Proves wasm/browser-entry.js's setWasmModuleOptions({ locateFile }) hook
// actually controls where the WASM backend fetches cld.web.wasm from --
// not just that the option is accepted, but that redirecting it to a
// custom URL is what makes detection succeed.
//
// Each case runs in its own child process: browser-entry.js caches
// moduleOptions/modulePromise at module scope on first use, so "no
// override" and "with override" can't share one process/import.
//
// With no override, browser-entry.js's default WASM URL resolves against
// import.meta.url, which is a file:// URL here -- and Node's fetch()
// doesn't support file://, so detect() is expected to reject. That's not a
// Node quirk being worked around, it's exactly the gap the override exists
// for, and it doubles as proof that the override (not some other
// already-working path) is what makes the second case below succeed.

const assert = require('assert');
const http = require('http');
const path = require('path');
const fs = require('fs');
const { execFile } = require('child_process');
const { promisify } = require('util');

const execFileAsync = promisify(execFile);

const SAMPLE = require('./data').basic[0];
assert.equal(SAMPLE.name, 'ENGLISH');

const wasmBinary = fs.readFileSync(path.join(__dirname, '..', 'wasm', 'dist', 'cld.web.wasm'));

// Runs the child asynchronously (not execFileSync): the second case below
// needs an HTTP server alive *in this same process* to answer the child's
// request, which a synchronous, event-loop-blocking child_process call
// would deadlock against.
function runChild(script) {
  return execFileAsync(process.execPath, ['-e', script], { encoding: 'utf8', cwd: __dirname });
}

(async () => {
  const withoutOverride = `
    import('../wasm/browser-entry.js')
      .then(m => m.detect(${JSON.stringify(SAMPLE.sample)}))
      .then(() => { console.log('UNEXPECTED_SUCCESS'); process.exit(0); })
      .catch(() => { console.log('EXPECTED_FAILURE'); process.exit(0); });
  `;
  let out1;
  try {
    out1 = (await runChild(withoutOverride)).stdout;
  } catch (e) {
    throw new Error(`child process crashed running detect() without an override: ${e.message}`);
  }
  assert.match(out1, /EXPECTED_FAILURE/, 'detect() should fail without a locateFile override (file:// is not fetchable in Node)');

  const server = http.createServer((req, res) => res.end(wasmBinary));
  await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
  const port = server.address().port;

  try {
    const withOverride = `
      import('../wasm/browser-entry.js').then(async m => {
        m.setWasmModuleOptions({ locateFile: () => 'http://127.0.0.1:${port}/custom-path/cld.web.wasm' });
        const result = await m.detect(${JSON.stringify(SAMPLE.sample)});
        console.log(JSON.stringify(result.languages[0]));
        process.exit(0);
      }).catch(e => { console.error(e.stack); process.exit(1); });
    `;
    const out2 = (await runChild(withOverride)).stdout;
    const topLanguage = JSON.parse(out2.trim().split('\n').pop());
    assert.equal(topLanguage.name, 'ENGLISH', 'detect() with a locateFile override should still detect correctly');
  } finally {
    server.close();
  }

  console.log('Browser entry setWasmModuleOptions()/locateFile override verified');
})();
