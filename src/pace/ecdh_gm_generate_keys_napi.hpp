#ifndef pace_ecdh_gm_generate_keys_napi_hpp
#define pace_ecdh_gm_generate_keys_napi_hpp

#include "crypto_error.hpp"

#include <node_api.h>

#include <string>
#include <vector>

namespace pace {
namespace ecdh_gm_generate_keys {
namespace napi {

struct generate_keys_data {
  std::vector<unsigned char> generator;
  std::string curve_name;

  std::vector<unsigned char> public_key;
  std::vector<unsigned char> private_key;

  crypto_error error;
};

napi_value generate_keys(napi_env env, napi_callback_info args);
void generate_keys_execute(napi_env env, void* data);
napi_value generate_keys_complete(napi_env env, void* data);

}  // namespace napi
}  // namespace ecdh_gm_generate_keys
}  // namespace pace

#endif
