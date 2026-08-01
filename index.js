const meta             = require('./lib/metadata.json');
const backend          = require('./lib/backend');
const { createDetect } = require('./lib/detect-shape');

module.exports = {
  LANGUAGES          : meta.LANGUAGES,
  DETECTED_LANGUAGES : meta.DETECTED_LANGUAGES,
  ENCODINGS          : meta.ENCODINGS,
  detect             : createDetect(backend.loadBackend, meta)
};
