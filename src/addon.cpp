#include "napi.h"
#include "pace/ecdh_gm_generate_keys_napi.hpp"
#include "pace/ecdh_gm_map_napi.hpp"

#include <node_api.h>

namespace addon {

napi_value Init(napi_env env, napi_value exports) {
  napi_value ecdh;
  napi_value gm;

  NAPI_CALL(env, napi_create_object(env, &ecdh));
  NAPI_CALL(env, napi_create_object(env, &gm));

  NAPI_CALL(env, napi_set_named_property(env, exports, "ecdh", ecdh));
  NAPI_CALL(env, napi_set_named_property(env, ecdh, "gm", gm));

  napi_value gm_generate_keys_fn;
  NAPI_CALL(env, napi_create_function(env, "generateKeys", NAPI_AUTO_LENGTH,
                                      pace::ecdh_gm_generate_keys::napi::generate_keys, nullptr,
                                      &gm_generate_keys_fn));
  NAPI_CALL(env, napi_set_named_property(env, gm, "generateKeys", gm_generate_keys_fn));

  napi_value gm_map_fn;
  NAPI_CALL(env, napi_create_function(env, "map", NAPI_AUTO_LENGTH,
                                      pace::ecdh_gm_map::napi::map, nullptr,
                                      &gm_map_fn));
  NAPI_CALL(env, napi_set_named_property(env, gm, "map", gm_map_fn));

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);

}  // namespace addon
