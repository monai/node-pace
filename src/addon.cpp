#include <node_api.h>

#include "pace_ecdh_gm_napi.hpp"

namespace addon {

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value build_do83_fn;

  status = napi_create_function(env, "buildDo83", NAPI_AUTO_LENGTH,
                                pace::ecdh_gm::napi::build_do83, nullptr, &build_do83_fn);
  if (status != napi_ok) {
    return nullptr;
  }

  status = napi_set_named_property(env, exports, "buildDo83", build_do83_fn);
  if (status != napi_ok)
    return nullptr;
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);

}  // namespace addon
