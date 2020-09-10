#include "napi.h"
#include "pace/ecdh_gm_map_napi.hpp"
#include "pace/ecdh_gm_generate_keys_napi.hpp"
#include "pace/ecdh_gm_key_agreement_napi.hpp"

#include <node_api.h>

namespace addon {

napi_value Init(napi_env env, napi_value exports) {
  napi_value gm;

  NAPI_CALL(env, napi_create_object(env, &gm));

  napi_value gm_map_fn;
  NAPI_CALL(env, napi_create_function(env, "map", NAPI_AUTO_LENGTH,
                                      pace::ecdh_gm_map::napi::map, nullptr,
                                      &gm_map_fn));
  NAPI_CALL(env, napi_set_named_property(env, gm, "map", gm_map_fn));

  napi_value gm_generate_keys_fn;
  NAPI_CALL(env, napi_create_function(
                     env, "generateKeys", NAPI_AUTO_LENGTH,
                     pace::ecdh_gm_generate_keys::napi::generate_keys, nullptr,
                     &gm_generate_keys_fn));
  NAPI_CALL(env, napi_set_named_property(env, gm, "generateKeys",
                                         gm_generate_keys_fn));

  napi_value gm_key_agreement_fn;
  NAPI_CALL(env, napi_create_function(
                     env, "keyAgreement", NAPI_AUTO_LENGTH,
                     pace::ecdh_gm_key_agreement::napi::key_agreement, nullptr,
                     &gm_key_agreement_fn));
  NAPI_CALL(env, napi_set_named_property(env, gm, "keyAgreement",
                                         gm_key_agreement_fn));

  NAPI_CALL(env, napi_set_named_property(env, exports, "gm", gm));

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);

}  // namespace addon
