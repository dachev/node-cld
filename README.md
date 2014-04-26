# CLD
[![Build Status](https://secure.travis-ci.org/dachev/cld.png)](http://travis-ci.org/#!/dachev/cld)

A straight port of the CLD (Compact Language Detector) library embedded in Google's Chromium browser. The library detects the language from provided Javascript string. It's implemented in C++, with very basic Node bindings. Original work by [Mike McCandless](http://code.google.com/p/chromium-compact-language-detector/)

## Installation

``` bash
    $ npm install cld
```

## Usage examples

``` javascript
    require('cld').detect('This is a language recognition example', function(err, result) {
      console.log(result);
    });

    var text    = 'Това е пример за разпознаване на Български език';
    var options = {isHTML:false, languageHint:'BULGARIAN', encodingHint:'ISO_8859_5', tldHint:'bg', httpHint:'bg'};

    require('cld').detect('Това е тест', options, function(err, result) {
      console.log(result);
    });
```


## Options

## Copyright
Copyright 2011, Blagovest Dachev.

## License
Apache 2