#include "Constants.hpp"
#include "Context.hpp"
#include "SharedSecret.hpp"
#include "PACE.hpp"
#include "SM.hpp"

#include <napi.h>

Napi::Value EACCleanup(const Napi::CallbackInfo &info) {
  EAC_cleanup();

  return Napi::Value();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  EAC_init();
  exports.Set("cleanup", Napi::Function::New(env, EACCleanup));

  Constants::Init(env, exports);
  Context::Init(env, exports);
  SharedSecret::Init(env, exports);
  PACE::Init(env, exports);
  SM::Init(env, exports);

  return exports;
}

NODE_API_MODULE(eac, Init)
