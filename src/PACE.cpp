#include "PACE.hpp"
#include "Context.hpp"
#include "SharedSecret.hpp"
//
#include <openssl/bio.h>
#include <napi.h>

namespace PACE {

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Napi::Object pace = Napi::Object::New(env);

  pace.Set("step1EncryptNonce", Napi::Function::New(env, PACE::Step1EncryptNonce));
  pace.Set("step2DecryptNonce", Napi::Function::New(env, PACE::Step2DecryptNonce));
  pace.Set("step3AGenerateMappingData", Napi::Function::New(env, PACE::Step3AGenerateMappingData));
  pace.Set("step3AMapGenerator", Napi::Function::New(env, PACE::Step3AMapGenerator));
  pace.Set("step3BGenerateEphemeralKey", Napi::Function::New(env, PACE::Step3BGenerateEphemeralKey));
  pace.Set("step3BComputeSharedSecret", Napi::Function::New(env, PACE::Step3BComputeSharedSecret));
  pace.Set("step3CDeriveKeys", Napi::Function::New(env, PACE::Step3CDeriveKeys));
  pace.Set("step3DComputeAuthenticationToken", Napi::Function::New(env, PACE::Step3DComputeAuthenticationToken));
  pace.Set("step3DVerifyAuthenticationToken", Napi::Function::New(env, PACE::Step3DVerifyAuthenticationToken));

  exports.Set("PACE", pace);
  return exports;
}

Napi::Value Step1EncryptNonce(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  SharedSecret* secret = Napi::ObjectWrap<SharedSecret>::Unwrap(info[1].As<Napi::Object>());

  BUF_MEM* nonce = PACE_STEP1_enc_nonce(context->ctx(), secret->sec());

  if (nonce == nullptr) {
    Napi::Error::New(env, "Failed to encrypt nonce").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> nonceObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)nonce->data, nonce->length);
  BUF_MEM_free(nonce);

  return nonceObj;
}

Napi::Value Step2DecryptNonce(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  SharedSecret* secret = Napi::ObjectWrap<SharedSecret>::Unwrap(info[1].As<Napi::Object>());
  Napi::Buffer<uint8_t> nonce = info[2].As<Napi::Buffer<uint8_t>>();

  BIO* nonce_bio = BIO_new_mem_buf(nonce.Data(), (int)nonce.Length());
  BUF_MEM* nonce_buf;
  BIO_get_mem_ptr(nonce_bio, &nonce_buf);

  int status = PACE_STEP2_dec_nonce(context->ctx(), secret->sec(), nonce_buf);
  BIO_free(nonce_bio);

  if (status == 0) {
    Napi::Error::New(env, "Failed to decrypt nonce").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Step3AGenerateMappingData(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());

  BUF_MEM* mappingData = PACE_STEP3A_generate_mapping_data(context->ctx());

  if (mappingData == nullptr) {
    Napi::Error::New(env, "Failed to generate mapping data").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> mappingDataObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)mappingData->data, mappingData->length);
  BUF_MEM_free(mappingData);

  return mappingDataObj;
}

Napi::Value Step3AMapGenerator(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> mappingData = info[1].As<Napi::Buffer<uint8_t>>();

  BIO* mappingData_bio = BIO_new_mem_buf(mappingData.Data(), (int)mappingData.Length());
  BUF_MEM* mappingData_buf;
  BIO_get_mem_ptr(mappingData_bio, &mappingData_buf);

  int status = PACE_STEP3A_map_generator(context->ctx(), mappingData_buf);
  BIO_free(mappingData_bio);

  if (status == 0) {
    Napi::Error::New(env, "Failed to map generator").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Step3BGenerateEphemeralKey(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());

  BUF_MEM* ephemeralKey = PACE_STEP3B_generate_ephemeral_key(context->ctx());

  if (ephemeralKey == nullptr) {
    Napi::Error::New(env, "Failed to generate ephemeral key").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> ephemeralKeyObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)ephemeralKey->data, ephemeralKey->length);
  BUF_MEM_free(ephemeralKey);

  return ephemeralKeyObj;
}

Napi::Value Step3BComputeSharedSecret(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> publicKey = info[1].As<Napi::Buffer<uint8_t>>();

  BIO* publicKey_bio = BIO_new_mem_buf(publicKey.Data(), (int)publicKey.Length());
  BUF_MEM* publicKey_buf;
  BIO_get_mem_ptr(publicKey_bio, &publicKey_buf);

  int status = PACE_STEP3B_compute_shared_secret(context->ctx(), publicKey_buf);
  BIO_free(publicKey_bio);

  if (status == 0) {
    Napi::Error::New(env, "Failed to compute shared secret").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Step3CDeriveKeys(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());

  int status = PACE_STEP3C_derive_keys(context->ctx());
  if (status == 0) {
    Napi::Error::New(env, "Failed to derive keys").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

Napi::Value Step3DComputeAuthenticationToken(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> publicKey = info[1].As<Napi::Buffer<uint8_t>>();

  BIO* publicKey_bio = BIO_new_mem_buf(publicKey.Data(), (int)publicKey.Length());
  BUF_MEM* publicKey_buf;
  BIO_get_mem_ptr(publicKey_bio, &publicKey_buf);

  BUF_MEM* authenticationToken = PACE_STEP3D_compute_authentication_token(context->ctx(), publicKey_buf);
  BIO_free(publicKey_bio);

  if (authenticationToken == nullptr) {
    Napi::Error::New(env, "Failed to compute authentication token").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> authenticationTokenObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)authenticationToken->data, authenticationToken->length);
  BUF_MEM_free(authenticationToken);

  return authenticationTokenObj;
}

Napi::Value Step3DVerifyAuthenticationToken(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> authenticationToken = info[1].As<Napi::Buffer<uint8_t>>();

  BIO* authenticationToken_bio = BIO_new_mem_buf(authenticationToken.Data(), (int)authenticationToken.Length());
  BUF_MEM* authenticationToken_buf;
  BIO_get_mem_ptr(authenticationToken_bio, &authenticationToken_buf);

  int status = PACE_STEP3D_verify_authentication_token(context->ctx(), authenticationToken_buf);
  BIO_free(authenticationToken_bio);

  if (status == 0) {
    Napi::Error::New(env, "Failed to verify authentication token").ThrowAsJavaScriptException();
  }

  return Napi::Value();
}

}
