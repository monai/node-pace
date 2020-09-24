const assert = require('assert');
const addon = require('./index');

const mapSkPcd = buffer`
  7F4EF07B 9EA82FD7 8AD689B3 8D0BC78C
  F21F249D 953BC46F 4C6E1925 9C010F99
`;

// DO'81
const mapPkPcd = buffer`
  04
  7ACF3EFC 982EC455 65A4B155 129EFBC7
  4650DCBF A6362D89 6FC70262 E0C2CC5E,
  544552DC B6725218 799115B5 5C9BAA6D
  9F6BC3A9 618E70C2 5AF71777 A9C4922D
`;

// DO'82
const mapPkIc = buffer`
  04
  824FBA91 C9CBE26B EF53A0EB E7342A3B
  F178CEA9 F45DE0B7 0AA60165 1FBA3F57,
  30D8C879 AAA9C9F7 3991E61B 58F4D52E
  B87A0A0C 709A49DC 63719363 CCD13C54
`;

const nonceS = buffer`
  3F00C4D3 9D153F2B 2A214A07 8D899B22
`;

const ephSkPcd = buffer`
  A73FB703 AC1436A1 8E0CFA5A BB3F7BEC
  7A070E7A 6788486B EE230C4A 22762595
`;

const ephPkPcd = buffer`
  04
  2DB7A64C 0355044E C9DF1905 14C625CB
  A2CEA487 54887122 F3A5EF0D 5EDD301C,
  3556F3B3 B186DF10 B857B58F 6A7EB80F
  20BA5DC7 BE1D43D9 BF850149 FBB36462
`;

const ephPkIc = buffer`
  04
  9E880F84 2905B8B3 181F7AF7 CAA9F0EF
  B743847F 44A306D2 D28C1D9E C65DF6DB,
  7764B222 77A2EDDC 3C265A9F 018F9CB8
  52E111B7 68B32690 4B59A019 3776F094
`;

const ephSharedSecret = buffer`
  28768D20 701247DA E81804C9 E780EDE5
  82A9996D B4A31502 0B273319 7DB84925
`;

const mappedGeneratorG = buffer`
  04
  8CED63C9 1426D4F0 EB1435E7 CB1D74A4
  6723A0AF 21C89634 F65A9AE8 7A9265E2,
  8C879506 743F8611 AC33645C 5B985C80
  B5F09A0B 83407C1B 6A4D857A E76FE522
`;

testMap();

function testKeyAgreement() {
  addon.gm.keyAgreement(ephSkPcd, ephPkIc, 'brainpoolP256r1', (error, sharedSecret) => {
    if (error) {
      console.error('keyAgreement error', error);
      return;
    }

    console.log('shared secret', sharedSecret);
    assert.ok(ephSharedSecret.equals(sharedSecret.slice(1, 33)));
  });
}

function testMap() {
  addon.gm.map(mapSkPcd, mapPkIc, nonceS, 'brainpoolP256r1', (error, generator) => {
    if (error) {
      console.error('map error', error);
      return;
    }

    console.log('generator', generator);
    assert.ok(mappedGeneratorG.equals(generator));
  });
}

function testGenerateKeys() {
  addon.gm.map(mapSkPcd, mapPkIc, nonceS, 'brainpoolP256r1', (error, generator) => {
    if (error) {
      console.error('map error', error);
      return;
    }

    addon.gm.generateKeys(generator, 'brainpoolP256r1', (error, [ephPkPcdCalc, ephSkPcdCalc]) => {
      if (error) {
        console.error('generateKeys error', error);
        return;
      }

      console.log('ephemeral public key', ephPkPcdCalc);
      console.log('ephemeral private key', ephSkPcdCalc);
    });
  });
}

process.on('uncaughtException', error => {
  console.error(`uncaughtException code: ${error.code}, message: ${error.message}`);
});

function buffer(strs) {
  return Buffer.from(strs.join('').replace(/[\s,]/g, ''), 'hex');
}
