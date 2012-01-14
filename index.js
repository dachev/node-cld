try {
    module.exports = require('./build/default/cld');
} catch(e) {
    // node v0.5.5+
    module.exports = require('./build/Release/cld');
}
