#include "napi.h"
#include "pace_ecdh_gm_napi.hpp"
#include "pace_ecdh_gm_map_napi.hpp"

#include <node_api.h>

namespace addon {

napi_value Init(napi_env env, napi_value exports) {
  napi_value gm;
  napi_value gm_build_do83_fn;
  napi_value gm_map_fn;

  NAPI_CALL(env, napi_create_object(env, &gm));

  NAPI_CALL(env, napi_create_function(env, "buildDo83", NAPI_AUTO_LENGTH,
                                      pace::ecdh_gm::napi::build_do83, nullptr,
                                      &gm_build_do83_fn));
  NAPI_CALL(env,
            napi_set_named_property(env, gm, "buildDo83", gm_build_do83_fn));

  NAPI_CALL(env, napi_create_function(env, "map", NAPI_AUTO_LENGTH,
                                      pace::ecdh_gm_map::napi::map, nullptr,
                                      &gm_map_fn));
  NAPI_CALL(env, napi_set_named_property(env, gm, "map", gm_map_fn));

  NAPI_CALL(env, napi_set_named_property(env, exports, "gm", gm));

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);

}  // namespace addon
