const _    = require('underscore');
const cld2 = require('./build/Release/cld');

module.exports = {
  LANGUAGES          : cld2.LANGUAGES,
  DETECTED_LANGUAGES : cld2.DETECTED_LANGUAGES,
  ENCODINGS          : cld2.ENCODINGS,

  async detect(text, options) {
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
        !~cld2.ENCODINGS.indexOf(options.encodingHint)) {

        throw new Error('Invalid encodingHint, see ENCODINGS');
      }
      if (options.languageHint.length > 0 &&
        !~_.keys(cld2.LANGUAGES).indexOf(options.languageHint) &&
        !~_.values(cld2.LANGUAGES).indexOf(options.languageHint)) {

        throw new Error('Invalid languageHint, see LANGUAGES');
      }

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
  }
};
