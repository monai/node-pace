#ifndef pace_ecdh_gm_map_napi_hpp
#define pace_ecdh_gm_map_napi_hpp

#include "pace_crypto_error.hpp"

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

  std::string error_code;
  std::string error_message;
  crypto_error error;

  napi_ref callback;
  napi_async_work work;
};

napi_value map(napi_env env, napi_callback_info args);
void map_execute(napi_env env, void* data);
void map_complete(napi_env env, napi_status status, void* data);

}  // namespace napi
}  // namespace ecdh_gm_map
}  // namespace pace

#endif