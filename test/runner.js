#!/usr/bin/env node

const cld    = require('../index');
const data   = require('./data');
const assert = require('assert');
const _      = require('underscore');

function runCoreTests(detected) {
  _.each(data.all, function(val, key) {
    try {
      const result = cld.detect(val.sample);
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
      assert.equal(val.name, result.languages[0].name);
      detected[val.name] = true;
    } catch (err) {
      detected[val.name] = false;
      assert.equal(err, null);
    }
  });
}

function runChunkTests() {
  _.each(data.all, function(val, key) {
    try {
      const result = cld.detect(val.sample);
      assert.equal(result.textBytes > 0, true);
      
      if (val.sample == data.frEnLatn) {
        assert.equal(_.isArray(result.chunks), true);
        assert.equal(result.chunks.length, 3);
	
        var chunkCodes = _.pluck(result.chunks, 'code');
        assert.deepEqual(chunkCodes, ['en', 'fr', 'en']);
      }
    } catch (err) {
      assert.equal(err, null);
    }
  });
}

function runEncodingHintTests() {
  _.each(data.all, function(item, idx) {
    _.each(cld.ENCODINGS, function(encoding, idx) {
      try {
	const result = cld.detect(item.sample, {encodingHint:encoding});
       
        assert.equal(_.isArray(result.languages), true);
        assert.equal(result.languages.length > 0, true);
      } catch (err) {
	assert.equal(err, null);
      }
    });
  });

  try {
    const result = cld.detect(data.all[0].sample, {encodingHint:'p'});
  } catch (err) {
    assert.equal(err.message, 'Invalid encodingHint, see ENCODINGS');
  }
}

function runLanguageHintTests() {
  _.each(data.all, function(item, idx) {
    _.each(_.keys(cld.LANGUAGES), function(name, idx) {
      try {
	const result = cld.detect(item.sample, {languageHint:name});
	assert.equal(_.isArray(result.languages), true);
      } catch (err) {
	assert.equal(err.message, 'Failed to identify language');
      }
    });
    _.each(_.values(cld.LANGUAGES), function(code, idx) {
      try {
	const result = cld.detect(item.sample, {languageHint:code});
	assert.equal(_.isArray(result.languages), true);
      } catch (err) {
        assert.equal(err.message, 'Failed to identify language');
      }

    });
  });

  try {
    const result = cld.detect(data.all[0].sample, {languageHint:'foo-bar-baz'});
  } catch (err) {
    assert.equal(err.message, 'Invalid languageHint, see LANGUAGES');
  }
}

function runTldHintTests() {
  _.each(data.all, function(item, idx) {
    try {
      const result = cld.detect(item.sample, {tldHint:'edu'});
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
    } catch (err) {
      assert.equal(err, null);
    }
    try {
      const result = cld.detect(item.sample, {tldHint:'com'});
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
    } catch (err) {
      assert.equal(err, null);
    }
    try {
      const result = cld.detect(item.sample, {tldHint:'id'});
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
    } catch (err) {
      assert.equal(err, null);
    }
  });
}

function runHttpHintTests() {
  _.each(data.all, function(item, idx) {
    try {
      const result = cld.detect(item.sample, {httpHint:'mi,en'});
      assert.equal(_.isArray(result.languages), true);
    } catch (err) {
      assert.equal(err.message, 'Failed to identify language');
    }
  });
}

function runUnreliableTests() {
  try {
    const result = cld.detect('interaktive infografik \xc3\xbcber videospielkonsolen');
    assert.equal(true, false);
  } catch (err) {
    assert.equal(err.message, 'Failed to identify language');
  }
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
