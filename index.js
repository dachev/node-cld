const _    = require('underscore');
const cld2 = require('./build/Release/cld');

module.exports = {
  LANGUAGES          : cld2.LANGUAGES,
  DETECTED_LANGUAGES : cld2.DETECTED_LANGUAGES,
  ENCODINGS          : cld2.ENCODINGS,

  detect : function (text, options = {}) {
    if (!_.isString(text) || text.length < 1) {
      throw new Error('Empty or invalid text');
    }
    
    const defaults = {
      isHTML       : false,
      languageHint : '',
      encodingHint : '',
      tldHint      : '',
      httpHint     : ''
    };
    options = _.defaults(options, defaults);

    if (!_.isBoolean(options.isHTML)) {
      throw new Error('Invalid isHTML value');
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

    const result = cld2.detect(
      text,
      !options.isHTML,
      options.languageHint,
      options.encodingHint,
      options.tldHint,
      options.httpHint
    );

    if (result.languages.length < 1) {
      throw new Error('Failed to identify language');
    }

    return result;
  }
};
