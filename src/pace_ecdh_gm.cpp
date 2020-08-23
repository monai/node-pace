#include "pace_ecdh_gm.hpp"

namespace pace {
namespace ecdh_gm {

int calculate_ephemeral_key(EC_KEY* pcd_key_pair,
                            EC_POINT* ic_public_key,
                            std::vector<unsigned char>& nonce,
                            EC_KEY* ephemeral_key_pair) {
  int ret = 1;
  int status = 0;

  const EC_GROUP* group;
  EC_POINT* shared_secret = nullptr;
  EC_POINT* generator = nullptr;

  if (ret == 1 && status == 0) {
    group = EC_KEY_get0_group(pcd_key_pair);
    if (group == nullptr) {
      status = 1;
    }
  }

  if (ret == 1 && status == 0) {
    shared_secret = EC_POINT_new(group);
    if (shared_secret == nullptr) {
      status = 2;
    }
  }

  if (ret == 1 && status == 0) {
    generator = EC_POINT_new(group);
    if (generator == nullptr) {
      status = 3;
    }
  }

  if (ret == 1 && status == 0) {
    ret = calculate_shared_secret(pcd_key_pair, ic_public_key, shared_secret);
  }

  if (ret == 1 && status == 0) {
    ret = map_nonce_to_generator(group, nonce, shared_secret, generator);
  }

  if (ret == 1 && status == 0) {
    ret = map_domain_parameters(group, generator, ephemeral_key_pair);
  }

  if (shared_secret != nullptr) {
    EC_POINT_free(shared_secret);
  }
  if (generator != nullptr) {
    EC_POINT_free(generator);
  }

  return ret;
}

int map_domain_parameters(const EC_GROUP* group,
                          EC_POINT* generator,
                          EC_KEY* key_pair) {
  int ret = 1;
  int status = 0;

  BIGNUM* order = BN_new();
  BIGNUM* cofactor = BN_new();
  EC_GROUP* new_group = nullptr;

  ret = EC_GROUP_get_order(group, order, nullptr);

  if (ret == 1) {
    ret = EC_GROUP_get_cofactor(group, cofactor, nullptr);
  }

  if (ret == 1) {
    new_group = EC_GROUP_dup(group);
    if (new_group == nullptr) {
      status = 1;
    }
  }

  if (ret == 1 && status == 0) {
    ret = EC_GROUP_set_generator(new_group, generator, order, cofactor);
  }

  if (ret == 1 && status == 0) {
    ret = EC_KEY_set_group(key_pair, new_group);
  }

  if (ret == 1 && status == 0) {
    ret = EC_KEY_generate_key(key_pair);
  }

  BN_free(order);
  BN_free(cofactor);
  if (new_group != nullptr) {
    EC_GROUP_clear_free(new_group);
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
int map_nonce_to_generator(const EC_GROUP* group,
                           std::vector<unsigned char>& nonce,
                           EC_POINT* shared_secret,
                           EC_POINT* generator) {
  int ret = 1;
  int status = 0;

  BIGNUM* bn_nonce_s = BN_new();
  const EC_POINT* input_generator;
  EC_POINT* intermediate_generator = nullptr;

  BN_bin2bn(nonce.data(), 16, bn_nonce_s);

  input_generator = EC_GROUP_get0_generator(group);
  if (input_generator == nullptr) {
    ret = 0;
    status = 1;
  }

  if (ret == 1 && status == 0) {
    intermediate_generator = EC_POINT_new(group);
    if (intermediate_generator == nullptr) {
      status = 2;
    }
  }

  if (ret == 1 && status == 0) {
    ret = EC_POINT_mul(group, intermediate_generator, 0, input_generator,
                       bn_nonce_s, nullptr);
  }

  if (ret == 1 && status == 0) {
    ret = EC_POINT_add(group, generator, intermediate_generator, shared_secret,
                       nullptr);
  }

  BN_free(bn_nonce_s);
  if (intermediate_generator != nullptr) {
    EC_POINT_free(intermediate_generator);
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

int calculate_shared_secret(EC_KEY* pcd_key_pair,
                            EC_POINT* ic_public_key,
                            EC_POINT* shared_secret) {
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
    ret = EC_POINT_mul(group, shared_secret, 0, ic_public_key, pcd_private_key,
                       0);
  }

  return ret;
}

}  // namespace ecdh_gm
}  // namespace pace
