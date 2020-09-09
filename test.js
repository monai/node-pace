let addon;
try {
  try {
    console.log('debug');
    addon = require('./build/DerivedData/Binding/Build/Products/Debug/addon.node');
  } catch (ex) {
    console.log('release');
    addon = require('./build/DerivedData/Binding/Build/Products/Release/addon.node');
  }
} catch (ex) {
  try {
    console.log('debug');
    addon = require('./build/Debug/addon.node');
  } catch (ex) {
    console.log('release');
    addon = require('./build/Release/addon.node');
  }
}

const pcdPrivateKey = Buffer.from([
  0x7F, 0x4E, 0xF0, 0x7B, 0x9E, 0xA8, 0x2F, 0xD7, 0x8A, 0xD6, 0x89,
  0xB3, 0x8D, 0x0B, 0xC7, 0x8C, 0xF2, 0x1F, 0x24, 0x9D, 0x95, 0x3B,
  0xC4, 0x6F, 0x4C, 0x6E, 0x19, 0x25, 0x9C, 0x01, 0x0F, 0x99
]);

// DO'81
const pcdPublicKey = Buffer.from([
  0x04, 0x7A, 0xCF, 0x3E, 0xFC, 0x98, 0x2E, 0xC4, 0x55, 0x65, 0xA4,
  0xB1, 0x55, 0x12, 0x9E, 0xFB, 0xC7, 0x46, 0x50, 0xDC, 0xBF, 0xA6,
  0x36, 0x2D, 0x89, 0x6F, 0xC7, 0x02, 0x62, 0xE0, 0xC2, 0xCC, 0x5E,
  0x54, 0x45, 0x52, 0xDC, 0xB6, 0x72, 0x52, 0x18, 0x79, 0x91, 0x15,
  0xB5, 0x5C, 0x9B, 0xAA, 0x6D, 0x9F, 0x6B, 0xC3, 0xA9, 0x61, 0x8E,
  0x70, 0xC2, 0x5A, 0xF7, 0x17, 0x77, 0xA9, 0xC4, 0x92, 0x2D

  // 0x04, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04
]);

// DO'82
const icPublicKey = Buffer.from([
  0x04, 0x82, 0x4F, 0xBA, 0x91, 0xC9, 0xCB, 0xE2, 0x6B, 0xEF, 0x53,
  0xA0, 0xEB, 0xE7, 0x34, 0x2A, 0x3B, 0xF1, 0x78, 0xCE, 0xA9, 0xF4,
  0x5D, 0xE0, 0xB7, 0x0A, 0xA6, 0x01, 0x65, 0x1F, 0xBA, 0x3F, 0x57,
  0x30, 0xD8, 0xC8, 0x79, 0xAA, 0xA9, 0xC9, 0xF7, 0x39, 0x91, 0xE6,
  0x1B, 0x58, 0xF4, 0xD5, 0x2E, 0xB8, 0x7A, 0x0A, 0x0C, 0x70, 0x9A,
  0x49, 0xDC, 0x63, 0x71, 0x93, 0x63, 0xCC, 0xD1, 0x3C, 0x54

  // 0x04, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  // 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04
]);

const nonceS = Buffer.from([
  0x3F, 0x00, 0xC4, 0xD3, 0x9D, 0x15, 0x3F, 0x2B,
  0x2A, 0x21, 0x4A, 0x07, 0x8D, 0x89, 0x9B, 0x22
]);

console.log('nonce: ', nonceS);

addon.gm.map(pcdPrivateKey, icPublicKey, nonceS, 'brainpoolP256r1', (error, generator) => {
  console.log('generator', generator);

  addon.gm.generateKeys(generator, 'brainpoolP256r1', (error, ephemeralPublicKey, ephemeralPrivateKey) => {
    console.error(error);
    console.log(ephemeralPublicKey);
    console.log(ephemeralPrivateKey);
  });
});

process.on('uncaughtException', error => {
  console.error(`uncaughtException code: ${error.code}, message: ${error.message}`);
});
