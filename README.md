# CLD
[![Build Status](https://secure.travis-ci.org/dachev/cld.png)](http://travis-ci.org/#!/dachev/cld)

Language detection for Javascript. Based on the CLD2 (Compact Language Detector) library from Google. Highly optimized for space and speed. Runs about 10x faster than other libraries. Detects over a 160 languages. Full test coverage.

## Installation

``` bash
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

Pass a cld.LANGUAGES key or value as a hint

### encodingHint

Pass a cld.ENCODINGS value as a hint

### tldHint

Pass top level domain as a hint

### httpHint

Pass a Content-Encoding value as a hint

## Copyright
Copyright 2011, Blagovest Dachev.

## License
Apache 2