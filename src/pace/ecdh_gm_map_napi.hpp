#ifndef pace_ecdh_gm_map_napi_hpp
#define pace_ecdh_gm_map_napi_hpp

#include "crypto_error.hpp"

#include <node_api.h>

#include <string>
#include <vector>

namespace pace {
namespace ecdh_gm_map {
namespace napi {

struct map_data {
  std::vector<unsigned char> pcd_private_key;
  std::vector<unsigned char> ic_public_key;
  std::vector<unsigned char> nonce;
  std::string curve_name;

  std::vector<unsigned char> generator;

  crypto_error error;
};

napi_value map(napi_env env, napi_callback_info args);
void map_execute(napi_env env, void* data);
napi_value map_complete(napi_env env, void* data);

}  // namespace napi
}  // namespace ecdh_gm_map
}  // namespace pace

#endif
