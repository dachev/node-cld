#!/usr/bin/env node

var cld    = require('../index');
var data   = require('./data');
var assert = require('assert');
var _      = require('underscore');

function runCoreTests(detected) {
  _.each(data.all, function(val, key) {
    cld.detect(val.sample, function(err, result) {
      assert.equal(err, null);
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
      assert.equal(val.name, result.languages[0].name);

      detected[val.name] = true;
    });
  });
}

function runChunkTests() {
  _.each(data.all, function(val, key) {
    cld.detect(val.sample, function(err, result) {
      assert.equal(result.textBytes > 0, true);
      if (val.sample == data.frEnLatn) {
        assert.equal(_.isArray(result.chunks), true);
        assert.equal(result.chunks.length, 3);

        var chunkCodes = _.pluck(result.chunks, 'code');
        assert.deepEqual(chunkCodes, ['en', 'fr', 'en'])
      }
    });
  });
}

function runEncodingHintTests() {
  _.each(data.all, function(item, idx) {
    _.each(cld.ENCODINGS, function(encoding, idx) {
      cld.detect(item.sample, {encodingHint:encoding}, function(err, result) {
        assert.equal(err, null);
        assert.equal(_.isArray(result.languages), true);
        assert.equal(result.languages.length > 0, true);
      });
    });
  });

  cld.detect(data.all[0].sample, {encodingHint:'p'}, function(err, result) {
    assert.equal(err.message, 'Invalid encodingHint, see ENCODINGS');
  });
}

function runLanguageHintTests() {
  _.each(data.all, function(item, idx) {
    _.each(_.keys(cld.LANGUAGES), function(name, idx) {
      cld.detect(item.sample, {languageHint:name}, function(err, result) {
      if (err) {
        assert.equal(err.message, 'Failed to identify language');
      }
      else {
        assert.equal(err, null);
        assert.equal(_.isArray(result.languages), true);
      }
      });
    });
    _.each(_.values(cld.LANGUAGES), function(code, idx) {
      cld.detect(item.sample, {languageHint:code}, function(err, result) {
      if (err) {
        assert.equal(err.message, 'Failed to identify language');
      }
      else {
        assert.equal(err, null);
        assert.equal(_.isArray(result.languages), true);
      }
      });
    });
  });

  cld.detect(data.all[0].sample, {languageHint:'foo-bar-bas'}, function(err, result) {
    assert.equal(err.message, 'Invalid languageHint, see LANGUAGES');
  });
}

function runTldHintTests() {
  _.each(data.all, function(item, idx) {
    cld.detect(item.sample, {tldHint:'edu'}, function(err, result) {
      assert.equal(err, null);
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
    });
    cld.detect(item.sample, {tldHint:'com'}, function(err, result) {
      assert.equal(err, null);
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
    });
    cld.detect(item.sample, {tldHint:'id'}, function(err, result) {
      assert.equal(err, null);
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
    });
  });
}

function runHttpHintTests() {
  _.each(data.all, function(item, idx) {
    cld.detect(item.sample, {httpHint:'mi,en'}, function(err, result) {
      if (err) {
        assert.equal(err.message, 'Failed to identify language');
      }
      else {
        assert.equal(err, null);
        assert.equal(_.isArray(result.languages), true);
      }
    });
  });
}

function runUnreliableTests() {
  cld.detect('interaktive infografik \xc3\xbcber videospielkonsolen', function(err, result) {
    assert.equal(err.message, 'Failed to identify language');
  });
}

function runCrossCheckTests(detected) {
  // Confirm that we didn't detect languages that are not listed in DETECTED_LANGUAGES
  _.each(_.values(cld.DETECTED_LANGUAGES), function(val, key) {
    delete detected[val];
  });

  assert.equal(_.keys(detected), 0);
}


var detected = {};

runCoreTests(detected);
runChunkTests();
runEncodingHintTests();
runLanguageHintTests();
runTldHintTests();
runHttpHintTests();
runUnreliableTests();
runCrossCheckTests(detected);


