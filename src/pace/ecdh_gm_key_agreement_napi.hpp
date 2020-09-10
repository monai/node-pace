#ifndef pace_ecdh_gm_key_agreement_napi_hpp
#define pace_ecdh_gm_key_agreement_napi_hpp

#include "crypto_error.hpp"

#include <node_api.h>

#include <string>
#include <vector>

namespace pace {
namespace ecdh_gm_key_agreement {
namespace napi {

struct key_agreement_data {
  std::vector<unsigned char> pcd_private_key;
  std::vector<unsigned char> ic_public_key;
  std::string curve_name;

  std::vector<unsigned char> shared_secret;

  crypto_error error;
  napi_ref callback;
  napi_async_work work;
};

napi_value key_agreement(napi_env env, napi_callback_info args);
void key_agreement_execute(napi_env env, void* data);
void key_agreement_complete(napi_env env, napi_status status, void* data);

}  // namespace napi
}  // namespace ecdh_gm_key_agreement
}  // namespace pace

#endif
