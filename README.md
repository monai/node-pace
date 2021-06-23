# @monai/pace

ICAO Doc 9303 PACE protocol mapping routines.

## Support

Algorithm           | Support
------------------- | -------
`id-PACE-DH-GM`     | ❌
`id-PACE-ECDH-GM`   | ✅
`id-PACE-DH-IM`     | ❌
`id-PACE-ECDH-IM`   | ❌
`id-PACE-ECDH-CAM`  | ❌

## Usage

```js
const assert = require('assert');
const { ecdh } = require('./index');

const nonce = Buffer.from('c99bdbcfc8ee32b5', 'hex');
const skPcd = Buffer.from('a9f9099d66d017923a222850ebf7997bdd15e6035636c97eb7abcd394fdf45bb', 'hex');
const pkIc = Buffer.from([
  '04',
  'e3b70d3d1d3ee946fcc2a842d32d0e5b4588ceb8bf430addde1abc2f4d24a83d',
  'd6934d71b009e38a2c2f1ae4c3f955727962ddf9b9575d4debbe5431a639f56f',
].join(''), 'hex');

const generator = ecdh.gm.map(skPcd, pkIc, nonce, 'prime256v1');
assert.ok(generator.equals(Buffer.from([
  '04',
  '2acfec1ce13e058148337a5bc046c605e53e9564b667318f0d663968cde9cbc1',
  'ffd872ef1f631bc185db0975f1d5eba78445d9426180b55ce2141e0f93715b49',
].join(''), 'hex')));
```

## API

### ecdh.gm.map(skPcd, pkIc, nonce, curve)

- `skPcd` \<Buffer\> PCD private key.
- `pkIc` \<Buffer\> IC public key.
- `nonce` \<Buffer\>
- `curve` \<string\> Name of the curve to use.
- Returns: \<Buffer\> Mapped generator.

### ecdh.gm.generateKeys(generator, curve)

- `generator` \<Buffer\> The curve generator point.
- `curve` \<string\> Name of the curve to use.
- Returns: \<Buffer[]\> An array with public and private keys.

## License

ISC
