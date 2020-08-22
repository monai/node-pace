#include "pace_ecdh_gm.hpp"

namespace pace {
namespace ecdh_gm {

int generate_ephemeral_key(const EC_GROUP* group,
                           EC_POINT* ephemeral_generator_G,
                           EC_KEY* ephemeral_key_pair) {
  int ret = 1;
  int status = 0;

  BIGNUM* order = BN_new();
  BIGNUM* cofactor = BN_new();
  EC_GROUP* group_ephemeral = nullptr;

  ret = EC_GROUP_get_order(group, order, nullptr);

  if (ret == 1) {
    ret = EC_GROUP_get_cofactor(group, cofactor, nullptr);
  }

  if (ret == 1) {
    group_ephemeral = EC_GROUP_dup(group);
    if (group_ephemeral == nullptr) {
      status = 1;
    }
  }

  if (ret == 1 && status == 0) {
    ret = EC_GROUP_set_generator(group_ephemeral, ephemeral_generator_G, order,
                                 cofactor);
  }

  if (ret == 1 && status == 0) {
    ret = EC_KEY_set_group(ephemeral_key_pair, group_ephemeral);
  }

  if (ret == 1 && status == 0) {
    ret = EC_KEY_generate_key(ephemeral_key_pair);
  }

  BN_free(order);
  BN_free(cofactor);
  if (group_ephemeral != nullptr) {
    EC_GROUP_clear_free(group_ephemeral);
  }

  return ret;

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
int map_nonce_to_G(const EC_GROUP* group,
                   std::vector<unsigned char>& nonce_s,
                   EC_POINT* shared_secret_point_H,
                   EC_POINT* ephemeral_generator_G) {
  int ret = 1;
  int status = 0;

  BIGNUM* bn_nonce_s = BN_new();
  const EC_POINT* generator_G;
  EC_POINT* generator_G_intermediate = nullptr;

  BN_bin2bn(nonce_s.data(), 16, bn_nonce_s);

  generator_G = EC_GROUP_get0_generator(group);
  if (generator_G == nullptr) {
    ret = 0;
    status = 1;
  }

  if (ret == 1 && status == 0) {
    generator_G_intermediate = EC_POINT_new(group);
    if (generator_G_intermediate == nullptr) {
      status = 2;
    }
  }

  if (ret == 1 && status == 0) {
    ret = EC_POINT_mul(group, generator_G_intermediate, 0, generator_G,
                       bn_nonce_s, nullptr);
  }

  if (ret == 1 && status == 0) {
    ret = EC_POINT_add(group, ephemeral_generator_G, generator_G_intermediate,
                       shared_secret_point_H, nullptr);
  }

  BN_free(bn_nonce_s);
  if (generator_G_intermediate != nullptr) {
    EC_POINT_free(generator_G_intermediate);
  }

  return ret;

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
  int ret = 1;
  int status = 0;

  const EC_GROUP* group;
  const BIGNUM* pcd_private_key;

  group = EC_KEY_get0_group(pcd_key_pair);
  if (group == nullptr) {
    status = 1;
  }

  if (ret == 1 && status == 0) {
    pcd_private_key = EC_KEY_get0_private_key(pcd_key_pair);
    if (pcd_private_key == nullptr) {
      status = 2;
    }
  }

  if (ret == 1 && status == 0) {
    ret = EC_POINT_mul(group, shared_secret_point_H, 0, ic_public_key,
                       pcd_private_key, 0);
  }

  return ret;
}

}  // namespace ecdh_gm
}  // namespace pace
