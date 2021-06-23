/* eslint-disable node/no-unpublished-require */
/* eslint-disable node/no-missing-require */
/* eslint-disable import/no-unresolved */
/* eslint-disable global-require */

let addon;

try {
  addon = require('./build/DerivedData/binding/Build/Products/Debug/addon.node');
} catch (ex1) {
  assertError(ex1);
  try {
    addon = require('./build/DerivedData/binding/Build/Products/Release/addon.node');
  } catch (ex2) {
    assertError(ex2);
  }
}

if (!addon) {
  try {
    addon = require('./build/Debug/addon.node');
  } catch (ex1) {
    assertError(ex1);
    try {
      addon = require('./build/Release/addon.node');
    } catch (ex2) {
      assertError(ex2);
    }
  }
}

module.exports = addon;

function assertError(ex) {
  if (ex.code !== 'MODULE_NOT_FOUND') {
    throw ex;
  }
}
