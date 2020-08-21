#include <node_api.h>

#include "pace_ecdh_gm_napi.hpp"

namespace addon {

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value paceEcdhBuildDo83;

  status = napi_create_function(env, "paceEcdhGmBuildDo83", NAPI_AUTO_LENGTH,
                                pace::ecdh_gm::napi::BuildDo83, nullptr, &paceEcdhBuildDo83);
  if (status != napi_ok) {
    return nullptr;
  }

  status = napi_set_named_property(env, exports, "paceEcdhBuildDo83", paceEcdhBuildDo83);
  if (status != napi_ok)
    return nullptr;
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace addon
