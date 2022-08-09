#include "SharedSecret.hpp"

#include <eac/pace.h>
#include <napi.h>

Napi::Object SharedSecret::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "SharedSecret", {});

  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("SharedSecret", func);
  return exports;
}

SharedSecret::SharedSecret(const Napi::CallbackInfo& info): Napi::ObjectWrap<SharedSecret>(info) {
  Napi::Env env = info.Env();

  Napi::Buffer<uint8_t> data = info[0].As<Napi::Buffer<uint8_t>>();
  Napi::Number passwordType = info[1].As<Napi::Number>();

  s_type type_arg = static_cast<s_type>(passwordType.Int32Value());

  _sec = PACE_SEC_new((char *)data.Data(), data.Length(), type_arg);
  if (_sec == nullptr) {
    Napi::Error::New(env, "Failed to create shared secret").ThrowAsJavaScriptException();
  }
}

SharedSecret::~SharedSecret() {
  PACE_SEC_clear_free(_sec);
}

PACE_SEC* SharedSecret::sec() {
  return _sec;
}

