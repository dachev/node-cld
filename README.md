# CLD
[![Build Status](https://secure.travis-ci.org/dachev/cld.png)](http://travis-ci.org/#!/dachev/cld)

A straight port of the CLD (Compact Language Detector) library embedded in Google's Chromium browser. The library detects the language from provided Javascript string. It's implemented in C++, with very basic Node bindings. Original work by [Mike McCandless](http://code.google.com/p/chromium-compact-language-detector/)

## Installation

``` bash
    $ npm install cld
```

## Usage

``` javascript
    var cld = require('cld');

    cld.detect(text);
```


## License
Copyright 2011, Blagovest Dachev.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.