const util = require('util');

let addon;

try {
  addon = require('./build/Debug/addon.node');
} catch (ex) {
  try {
    addon = require('./build/Release/addon.node');
  } catch (ex) {}
}

if (!addon) {
  try {
    addon = require('./build/DerivedData/binding/Build/Products/Debug/addon.node');
  } catch (ex) {
    addon = require('./build/DerivedData/binding/Build/Products/Release/addon.node');
  }
}

module.exports = addon;

[
  'map',
  'generateKeys'
].reduce((acc, fn, i) => {
  acc[fn + 'P'] = util.promisify(addon.gm[fn]);
  return acc;
}, module.exports.gm);
