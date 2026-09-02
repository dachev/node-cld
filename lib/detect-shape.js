// Shared detect() implementation used by both index.js (Node, native-first
// with WASM fallback) and wasm/browser-entry.mjs (bundler/browser, WASM
// only) -- the validation/defaults/error-mapping logic is identical for
// both, only how the backend is loaded differs.

const _ = require('underscore');

function createDetect(loadBackend, meta) {
  return async function detect(text, options) {
    let cb = arguments[2];
    if (typeof cb !== 'function' && typeof options === 'function') {
      cb = options;
      options = {};
    }

    try {
      if (arguments.length < 1) {
        throw new Error('Not enough arguments provided');
      }

      if (!_.isString(text) || text.length < 1) {
        throw new Error('Empty or invalid text');
      }

      const defaults = {
        isHTML       : false,
        languageHint : '',
        encodingHint : '',
        tldHint      : '',
        httpHint     : '',
        bestEffort   : false
      };
      options = _.defaults({}, options, defaults);

      if (!_.isBoolean(options.isHTML)) {
        throw new Error('Invalid isHTML value');
      }
      if (!_.isBoolean(options.bestEffort)) {
        throw new Error('Invalid bestEffort value');
      }
      if (!_.isString(options.languageHint)) {
        throw new Error('Invalid languageHint');
      }
      if (!_.isString(options.encodingHint)) {
        throw new Error('Invalid encodingHint');
      }
      if (!_.isString(options.tldHint)) {
        throw new Error('Invalid tldHint');
      }
      if (!_.isString(options.httpHint)) {
        throw new Error('Invalid httpHint');
      }
      if (options.encodingHint.length > 0 &&
        !~meta.ENCODINGS.indexOf(options.encodingHint)) {

        throw new Error('Invalid encodingHint, see ENCODINGS');
      }
      if (options.languageHint.length > 0 &&
        !~_.keys(meta.LANGUAGES).indexOf(options.languageHint) &&
        !~_.values(meta.LANGUAGES).indexOf(options.languageHint)) {

        throw new Error('Invalid languageHint, see LANGUAGES');
      }

      const cld2 = await loadBackend();
      const result = await cld2.detectAsync(
        text,
        !options.isHTML,
        options.languageHint,
        options.encodingHint,
        options.tldHint,
        options.httpHint,
        options.bestEffort
      );

      if (result.languages.length < 1) {
        throw new Error('Failed to identify language');
      }

      if (cb) {
        return cb(null, result);
      } else {
        return result;
      }
    } catch (error) {
      if (cb) {
        cb(error);
      } else {
        throw error;
      }
    }
  };
}

module.exports = { createDetect };
