/* eslint-disable node/no-unpublished-require */
/* eslint-disable node/no-missing-require */
/* eslint-disable import/no-unresolved */
/* eslint-disable global-require */
const { promisify } = require('util');

let addon;

try {
  addon = require('./build/Debug/addon.node');
} catch (ex1) {
  try {
    addon = require('./build/Release/addon.node');
  // eslint-disable-next-line no-empty
  } catch (ex2) {}
}

if (!addon) {
  try {
    addon = require('./build/DerivedData/binding/Build/Products/Debug/addon.node');
  } catch (ex) {
    addon = require('./build/DerivedData/binding/Build/Products/Release/addon.node');
  }
}

module.exports = addon;

Object
  .keys(addon.ecdh.gm)
  .reduce((acc, fn) => {
    acc[`${fn}P`] = promisify(addon.ecdh.gm[fn]);
    return acc;
  }, module.exports.ecdh.gm);
