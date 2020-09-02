let addon;

try {
  console.log('debug');
  addon = require('./build/Debug/addon.node');
} catch (ex) {
  console.log('release');
  addon = require('./build/Release/addon.node');
}

module.exports = addon;
