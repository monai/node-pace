const util = require('util');

let addon;

try {
  addon = require('./build/Debug/addon.node');
} catch (ex) {
  addon = require('./build/Release/addon.node');
}

module.exports = addon;

[
  'map',
  'generateKeys'
].reduce((acc, fn, i) => {
  acc[fn + 'P'] = util.promisify(addon.gm[fn]);
  return acc;
}, module.exports.gm);
