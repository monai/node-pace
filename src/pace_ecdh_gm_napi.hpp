#ifndef pace_ecdh_gm_napi_hpp
#define pace_ecdh_gm_napi_hpp

#include "pace_crypto_error.hpp"

#include <node_api.h>

#include <string>
#include <vector>

namespace pace {
namespace ecdh_gm {
namespace napi {

struct do83_data {
  std::vector<unsigned char> nonce;
  std::vector<unsigned char> pcd_public_key;
  std::vector<unsigned char> pcd_private_key;
  std::vector<unsigned char> ic_public_key;
  std::vector<unsigned char> pcd_ephemeral_public_key;
  std::vector<unsigned char> pcd_ephemeral_private_key;

  std::string error_code;
  std::string error_message;
  crypto_error error;

  napi_ref callback;
  napi_async_work work;
};

napi_value build_do83(napi_env env, napi_callback_info args);
void build_do83_execute(napi_env env, void* data);
void build_do83_complete(napi_env env, napi_status status, void* data);

}  // namespace napi
}  // namespace ecdh_gm
}  // namespace pace

#endif
