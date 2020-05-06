#!/usr/bin/env node

var cld    = require('../index');
var data   = require('./data');
var assert = require('assert');
var _      = require('underscore');

async function runCoreTests(detected) {
  for (const val of data.all) {
    if (!val.testOnWindows) {
      return;
    }

    const result = await cld.detct(val.sample);
    assert.equal(_.isArray(result.languages), true);
    assert.equal(result.languages.length > 0, true);
    assert.equal(val.name, result.languages[0].name);

    detected[val.name] = true;
  }
}

async function runChunkTests() {
  for (const val of data.all) {
    if (!val.testOnWindows) {
      return;
    }

    const result = await cld.detect(val.sample);
    assert.equal(result.textBytes > 0, true);
    if (val.sample == data.frEnLatn) {
      assert.equal(_.isArray(result.chunks), true);
      assert.equal(result.chunks.length, 3);

      var chunkCodes = _.pluck(result.chunks, 'code');
      assert.deepEqual(chunkCodes, ['en', 'fr', 'en'])
    }
  }
}

async function runEncodingHintTests() {
  for (const item of data.all) {
    if (!item.testOnWindows) {
      return;
    }

    for (const encodingHint of cld.ENCODINGS) {
      const result = await cld.detect(item.sample, { encodingHint });
      assert.equal(_.isArray(result.languages), true);
      assert.equal(result.languages.length > 0, true);
    }
  }

  try {
    await cld.detect(data.all[0].sample, { encodingHint: 'p' });
    assert.ok(false, 'Should not have detected');
  }
  catch (err) {
    assert.equal(err.message, 'Invalid encodingHint, see ENCODINGS');
  }
}

async function runLanguageHintTests() {
  _.each(data.all, function(item, idx) {
    if (!item.testOnWindows) {
      return;
    }

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

async function runTldHintTests() {
  for (const item of data.all) {
    if (!item.testOnWindows) {
      return;
    }

    let result = await cld.detect(item.sample, { tldHint: 'edu' });
    assert.equal(_.isArray(result.languages), true);
    assert.equal(result.languages.length > 0, true);

    result = await cld.detect(item.sample, { tldHint: 'com' });
    assert.equal(_.isArray(result.languages), true);
    assert.equal(result.languages.length > 0, true);

    result = await cld.detect(item.sample, { tldHint: 'id' });
    assert.equal(_.isArray(result.languages), true);
    assert.equal(result.languages.length > 0, true);
  }
}

async function runHttpHintTests() {
  for (const item of data.all) {
    if (!item.testOnWindows) {
      return;
    }

    let result;
    try {
      result = await cld.detect(item.sample, { httpHint: 'mi,en' });
    }
    catch (err) {
      assert.equal(err.message, 'Failed to identify language');
      return;
    }

    assert.equal(_.isArray(result.languages), true);
  }
}

async function runUnreliableTests() {
  try {
    await cld.detect('interaktive infografik \xc3\xbcber videospielkonsolen');
  }
  catch (err) {
    assert.equal(err.message, 'Failed to identify language');
  }
}

function runCallbackTests() {
  cld.detect('This is a language recognition example', (err, result) => {
    assert.equal(result.languages.length > 0, true);
  });
  cld.detect('This is a language recognition example', {isHTML:false}, (err, result) => {
    assert.equal(result.languages.length > 0, true);
  });
}

function runCrossCheckTests(detected) {
  // Confirm that we didn't detect languages that are not listed in DETECTED_LANGUAGES
  _.each(_.values(cld.DETECTED_LANGUAGES), function(val, key) {
    delete detected[val];
  });

  assert.equal(_.keys(detected), 0);
}

(async () => {
  let detected = {};

  await runCoreTests(detected);
  await runChunkTests();
  await runEncodingHintTests();
  await runLanguageHintTests();
  await runTldHintTests();
  await runHttpHintTests();
  await runUnreliableTests();

  runCallbackTests();
  runCrossCheckTests(detected);
})();
