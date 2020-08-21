#include "pace.h"
#include "pace_ecdh_gm.hpp"

namespace pace {
namespace ecdh_gm {

int generate_ephemeral_key(const EC_GROUP* group,
                           EC_POINT* ephemeral_generator_G,
                           EC_KEY* ephemeral_key_pair) {
  BIGNUM* order = BN_new();
  BIGNUM* cofactor = BN_new();
  EC_GROUP* group_ephemeral;

  PACE_CALL_WITH_STATUS(EC_GROUP_get_order(group, order, NULL));
  PACE_CALL_WITH_STATUS(EC_GROUP_get_cofactor(group, cofactor, NULL));

  PACE_CALL_RET_ZERO(group_ephemeral, EC_GROUP_dup(group));

  PACE_CALL_WITH_STATUS(EC_GROUP_set_generator(
      group_ephemeral, ephemeral_generator_G, order, cofactor));
  PACE_CALL_WITH_STATUS(EC_KEY_set_group(ephemeral_key_pair, group_ephemeral));

  return EC_KEY_generate_key(ephemeral_key_pair);

  //  return new ECParameterSpec(
  //                             new EllipticCurve(
  //                                               new ECFieldFp(p),
  //                                               a,
  //                                               b
  //                                               ),
  //                             ephemeralGenerator,
  //                             order,
  //                             cofactor
  //                             );
}

// G~ = [s]G + H
int map_nonce_to_G(const EC_GROUP* ec_group,
                   std::vector<unsigned char>& nonce_s,
                   EC_POINT* shared_secret_point_H,
                   EC_POINT* ephemeral_generator_G) {
  BIGNUM* bn_nonce_s = BN_new();
  const EC_POINT* generator_G;
  EC_POINT* generator_G_intermediate;

  BN_bin2bn(nonce_s.data(), 16, bn_nonce_s);
  if (bn_nonce_s == nullptr) {
    return 0;
  }

  PACE_CALL_RET_ZERO(generator_G, EC_GROUP_get0_generator(ec_group));
  PACE_CALL_RET_ZERO(generator_G_intermediate, EC_POINT_new(ec_group));
  PACE_CALL_WITH_STATUS(EC_POINT_mul(ec_group, generator_G_intermediate, 0,
                                     generator_G, bn_nonce_s, nullptr));

  return EC_POINT_add(ec_group, ephemeral_generator_G, generator_G_intermediate,
                      shared_secret_point_H, nullptr);

  //  D~ = (p, a, b, G~, n, h) where G~ = [s]G + H

  //  ECPoint ephemeralGenerator = Util.add(
  //                                        Util.multiply(
  //                                                      Util.os2i(nonceS),
  //                                                      generator,
  //                                                      staticParameters
  //                                                      ),
  //                                        sharedSecretPointH,
  //                                        staticParameters
  //                                        );
}

int calculate_H(EC_KEY* pcd_key_pair,
                EC_POINT* ic_public_key,
                EC_POINT* shared_secret_point_H) {
  const EC_GROUP* group;
  const BIGNUM* pcd_private_key;

  PACE_CALL_RET_ZERO(group, EC_KEY_get0_group(pcd_key_pair));
  PACE_CALL_RET_ZERO(pcd_private_key, EC_KEY_get0_private_key(pcd_key_pair));
  return EC_POINT_mul(group, shared_secret_point_H, 0, ic_public_key,
                      pcd_private_key, 0);
}

}  // namespace ecdh_gm
}  // namespace pace
