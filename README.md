# node-cld
[![Build Status](https://secure.travis-ci.org/dachev/node-cld.png)](http://travis-ci.org/#!/dachev/node-cld)
[![Dependencies](https://david-dm.org/dachev/node-cld.png)](https://david-dm.org/dachev/node-cld)
[![NPM version](https://badge.fury.io/js/cld.svg)](http://badge.fury.io/js/cld)

Language detection for Javascript. Based on the CLD2 (Compact Language Detector) library from Google.

Highly optimized for space and speed. Runs about 10x faster than other libraries. Detects over 160 languages. Full test coverage. Runs on Linux, OS X, and Windows.

## Installation

```bash
$ npm install cld
```

## Examples
### Simple
```js
require('cld').detect('This is a language recognition example', function(err, result) {
  console.log(result);
});
```

### Advanced
```js
var text    = 'Това е пример за разпознаване на Български език';
var options = {
  isHTML       : false,
  languageHint : 'BULGARIAN',
  encodingHint : 'ISO_8859_5',
  tldHint      : 'bg',
  httpHint     : 'bg'
};

require('cld').detect(text, options, function(err, result) {
  console.log(result);
});
```


## Options

### isHTML

Set to true if the string contains HTML tags

### languageHint

Pass a LANGUAGES key or value as a hint

### encodingHint

Pass an ENCODINGS value as a hint

### tldHint

Pass top level domain as a hint

### httpHint

Pass an HTTP "Content-Encoding" value as a hint

## Copyright
Copyright 2011-2014, Blagovest Dachev.

## License
Apache 2
