let addon;

try {
  addon = require('./build/Debug/addon.node');
} catch (ex) {
  addon = require('./build/Release/addon.node');
}

module.exports = addon;
