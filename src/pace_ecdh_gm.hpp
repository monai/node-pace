#ifndef pace_ecdh_gm_hpp
#define pace_ecdh_gm_hpp

#include <openssl/ec.h>

#include <vector>

namespace pace {
namespace ecdh_gm {

int calculate_ephemeral_key(EC_KEY* pcd_key_pair,
                            EC_POINT* ic_public_key,
                            std::vector<unsigned char>& nonce,
                            EC_KEY* ephemeral_key_pair);

int generate_key_pair(const EC_GROUP* group,
                      EC_POINT* generator,
                      EC_KEY* key_pair);

int map_nonce_to_generator(const EC_GROUP* group,
                           std::vector<unsigned char>& nonce,
                           EC_POINT* shared_secret,
                           EC_POINT* generator);

int calculate_shared_secret(EC_KEY* pcd_key_pair,
                            EC_POINT* ic_public_key,
                            EC_POINT* shared_secret);

}  // namespace ecdh_gm
}  // namespace pace

#endif
