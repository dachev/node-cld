# cld-universal
[![CI](https://github.com/figulusproject/cld-universal/actions/workflows/ci.yml/badge.svg)](https://github.com/figulusproject/cld-universal/actions/workflows/ci.yml)
[![NPM version](https://badge.fury.io/js/cld-universal.svg)](http://badge.fury.io/js/cld-universal)

[![Stand With Palestine](https://raw.githubusercontent.com/standforhumanity/stand-with-palestine/main/Banners/Banner1.svg)](https://stand-with-palestine.vercel.app/)

[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner2-direct.svg)](https://stand-with-ukraine.pp.ua)

*Fork developed on stolen Wurundjeri Land. We pay our respects to Elders past, present, and emerging, and with all peoples resisting occupation.*

> **This is a fork of [dachev/node-cld](https://github.com/dachev/node-cld).** The original project has had no maintainer activity in over a year, and has outdated dependencies causing 3 high severity security issues.
>
> In addition to updating outdated dependencies, this fork adds a **WASM fallback backend** (native addon first, WASM if it's unavailable - see below), enabling use in environments without a C++ toolchain, including in the browser.
>
> **Maintenance expectations:** this fork will receive best-effort security patches, but is unlikely to see further changes to the underlying CLD2 C++ core. If upstream development resumes and merges the WASM backend, this fork will be archived in favor of the original package.

Language detection for Javascript. Based on the CLD2 (Compact Language Detector) library from Google.

Highly optimized for space and speed. Runs about 10x faster than other libraries. Detects over 160 languages. Full test coverage. Runs on Linux, OS X, and Windows, and in the browser via a WASM fallback when the native addon is unavailable.

## Installation

```bash
$ npm install cld-universal
```


If a native build isn't possible at install time (missing toolchain, browser bundling, etc.), the package falls back to a bundled WASM build automatically -- no extra configuration needed.

Linux users aiming to avoid falling back to WASM: make sure you have g++ >= 4.8.

## Examples
### Simple (ESM)
```js
import cld from 'cld-universal';
const { detect } = cld;

// As a promise
const result = await detect('This is a language recognition example');
console.log(result);
```

> Node's CommonJS/ESM interop doesn't reliably expose this package's named exports (`detect`, `LANGUAGES`, etc.) as ESM named imports: `import { detect } from 'cld-universal'` will throw a `SyntaxError`. Import the default export and destructure instead, as above.


### Simple (CommonJS)
```js
const cld = require('cld-universal');

// As a promise
cld.detect('This is a language recognition example').then((result) => {
  console.log(result);
});

// In an async function
async function testCld() {
  const result = await cld.detect('This is a language recognition example');
  console.log(result);
}
```

### Advanced
```js
import cld from 'cld-universal';
const { detect } = cld;

const text    = 'Това е пример за разпознаване на Български език';
const options = {
  isHTML       : false,
  languageHint : 'BULGARIAN',
  encodingHint : 'ISO_8859_5',
  tldHint      : 'bg',
  httpHint     : 'bg'
};

const result = await detect(text, options);
console.log(result);
```

### Legacy callback style
`detect()` also accepts the node callback pattern. If options are provided, the third parameter is the callback.
```js
const cld = require('cld-universal');

cld.detect('This is a language recognition example', (err, result) => {
  console.log(result);
});
```

### Browser / bundlers
Bundlers (webpack, esbuild, Vite, etc.) resolve `cld-universal` to a WASM-only, browser-specific entry point via the package's `"browser"` export condition. That entry point is real ESM, so named imports work there:
```js
import { detect, LANGUAGES } from 'cld-universal';

const result = await detect('This is a language recognition example');
console.log(result);
```

If your bundler serves the WASM binary from a non-default path (a CDN, a renamed/hashed asset, etc.), call `setWasmModuleOptions({ locateFile })` before the first `detect()` call to override where `cld.web.wasm` is fetched from:
```js
import { detect, setWasmModuleOptions } from 'cld-universal';

setWasmModuleOptions({
  locateFile: (path) => `/assets/${path}`,
});

const result = await detect('This is a language recognition example');
```

## Result shape

`detect()` resolves to (or, in callback form, passes as the second argument) an object of the following shape:

| Field | Type | Description |
| --- | --- | --- |
| `reliable` | `boolean` | Whether CLD2 considers the detection reliable |
| `textBytes` | `number` | Number of bytes of text CLD2 actually looked at |
| `languages` | `Array<{ name, code, percent, score }>` | Detected languages, most likely first. `percent` is 0-100, `score` is CLD2's internal confidence score |
| `chunks` | `Array<{ name, code, offset, bytes }>` | Per-chunk breakdown showing which language was detected at which byte range of the input |

TypeScript types for this shape (and for `Options`, below) ship with the package via `index.d.ts` — no `@types` package needed.

## Options

| Option | Type | Description |
| --- | --- | --- |
| `isHTML` | `boolean` | Set to `true` if the string contains HTML tags |
| `languageHint` | `string` | A `LANGUAGES` key or value, passed as a hint |
| `encodingHint` | `string` | An `ENCODINGS` value, passed as a hint |
| `tldHint` | `string` | Top level domain, passed as a hint |
| `httpHint` | `string` | An HTTP `Content-Encoding` value, passed as a hint |
| `bestEffort` | `boolean` | Set to `true` to get a best-effort answer instead of `UNKNOWN_LANGUAGE`. Useful for short text if the caller prefers an approximate answer over none |

## Exported constants

Alongside `detect`, the package exports:

- **`LANGUAGES`**: an object mapping every language name CLD2 recognizes to its language code (e.g. `{ ENGLISH: 'en', ... }`). Use its keys/values as `languageHint`.
- **`DETECTED_LANGUAGES`**: an array of the language names `detect()` can actually return in `result.languages`.
- **`ENCODINGS`**: an array of the encoding names usable as `encodingHint`.

## Backends

This fork tries the native N-API addon first (built during `npm install`) and, if that's unavailable  (build failed, no C++ toolchain, running in a bundler/browser) transparently falls back to a bundled WASM build of the same CLD2 sources. Both backends are tested against each other for output parity (see `test/runner-wasm.js`); there's no behavior difference to code using this package, and no configuration needed to opt into the fallback.

## Warning
Once the module has been installed, the underlying C sources will remain in the ```deps/cld``` folder and continue to occupy considerable space. This is because they will be required if you ever need to run `npm rebuild`. If you are under severe constraints you can delete this folder and reclam >100M

## Copyright
Copyright 2011-2015, Blagovest Dachev.

This fork Copyright 2026, Figulus Project.

## License
Apache 2
