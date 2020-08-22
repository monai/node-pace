#ifndef pace_ecdh_gm_hpp
#define pace_ecdh_gm_hpp

#include <openssl/ec.h>

#include <vector>

namespace pace {
namespace ecdh_gm {

int generate_ephemeral_key(const EC_GROUP* group,
                           EC_POINT* ephemeral_generator_G,
                           EC_KEY* ephemeral_key_pair);

int map_nonce_to_G(const EC_GROUP* group,
                   std::vector<unsigned char>& nonce_s,
                   EC_POINT* shared_secret_point_H,
                   EC_POINT* ephemeral_generator_G);

int calculate_H(EC_KEY* pcd_key_pair,
                EC_POINT* ic_public_key,
                EC_POINT* shared_secret_point_H);

}  // namespace ecdh_gm
}  // namespace pace

#endif
