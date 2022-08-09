#include "Context.hpp"

#include <eac/eac.h>
#include <napi.h>

Napi::Object Context::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Context", {
    InstanceMethod<&Context::InitPACE>("initPACE"),
    InstanceMethod<&Context::InitTA>("initTA"),
    InstanceMethod<&Context::InitCA>("initCA"),
    InstanceMethod<&Context::InitRI>("initRI"),
    InstanceMethod<&Context::InitEFCardAccess>("initEFCardAccess"),
    InstanceMethod<&Context::InitEFCardSecurity>("initEFCardSecurity"),
    InstanceMethod<&Context::SetSMContext>("setSMContext"),
  });

  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

  exports.Set("Context", func);
  return exports;
}

Context::Context(const Napi::CallbackInfo& info): Napi::ObjectWrap<Context>(info) {
  Napi::Env env = info.Env();

  _ctx = EAC_CTX_new();
  if (_ctx == nullptr) {
    Napi::Error::New(env, "Failed to create EAC context").ThrowAsJavaScriptException();
  }
}

Context::~Context() {
  EAC_CTX_clear_free(_ctx);
}

EAC_CTX* Context::ctx() {
  return _ctx;
}

Napi::Value Context::InitPACE(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Napi::Number protocol = info[0].As<Napi::Number>();
  Napi::Number standardizedDomainParameter = info[1].As<Napi::Number>();

  int status = EAC_CTX_init_pace(ctx(), protocol.Int32Value(), standardizedDomainParameter.Int32Value());
  if (status == 0) {
    Napi::Error::New(env, "Failed to initialize an EAC context for PACE").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Context::InitTA(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  const unsigned char* privkey_arg = nullptr;
  size_t privkey_len = 0;
  const unsigned char* cvca_arg = nullptr;
  size_t cvca_len = 0;

  Napi::Buffer<uint8_t> privkey;
  Napi::Buffer<uint8_t> cvca;

  if (info[0].IsBuffer()) {
    privkey = info[0].As<Napi::Buffer<uint8_t>>();
    privkey_arg = privkey.Data();
    privkey_len = privkey.Length();
  }

  if (info[1].IsBuffer()) {
    cvca = info[1].As<Napi::Buffer<uint8_t>>();
    cvca_arg = cvca.Data();
    cvca_len = cvca.Length();
  }

  int status = EAC_CTX_init_ta(ctx(), privkey_arg, privkey_len, cvca_arg, cvca_len);
  if (status == 0) {
    Napi::Error::New(env, "Failed to initialize an EAC context for TA").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Context::InitCA(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Napi::Number protocol = info[0].As<Napi::Number>();
  Napi::Number curve = info[1].As<Napi::Number>();

  int status = EAC_CTX_init_ca(ctx(), protocol.Int32Value(), curve.Int32Value());
  if (status == 0) {
    Napi::Error::New(env, "Failed to initialize an EAC context for CA").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Context::InitRI(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Napi::Number protocol = info[0].As<Napi::Number>();
  Napi::Number sdp = info[1].As<Napi::Number>();

  int status = EAC_CTX_init_ri(ctx(), protocol.Int32Value(), sdp.Int32Value());
  if (status == 0) {
    Napi::Error::New(env, "Failed to initialize an EAC context for RI").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Context::InitEFCardAccess(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Napi::Buffer<uint8_t> data = info[0].As<Napi::Buffer<uint8_t>>();

  int status = EAC_CTX_init_ef_cardaccess(data.Data(), data.Length(), ctx());
  if (status == 0) {
    Napi::Error::New(env, "Failed to initialize an EAC context from EF.CardAccess").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Context::InitEFCardSecurity(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Napi::Buffer<uint8_t> data = info[0].As<Napi::Buffer<uint8_t>>();

  int status = EAC_CTX_init_ef_cardsecurity(data.Data(), data.Length(), ctx());
  if (status == 0) {
    Napi::Error::New(env, "Failed to initialize an EAC context from EF.CardSecurity").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Context::SetSMContext(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  Napi::Number encryptionContext = info[0].As<Napi::Number>();
  int status = EAC_CTX_set_encryption_ctx(ctx(), encryptionContext.Int32Value());
  if (status == 0) {
    Napi::Error::New(env, "Failed to set secure messaging context").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}
