#ifndef PACE_hpp
#define PACE_hpp

#include <napi.h>

namespace PACE {

Napi::Object Init(Napi::Env env, Napi::Object exports);

Napi::Value Step1EncryptNonce(const Napi::CallbackInfo &info);
Napi::Value Step2DecryptNonce(const Napi::CallbackInfo &info);
Napi::Value Step3AGenerateMappingData(const Napi::CallbackInfo &info);
Napi::Value Step3AMapGenerator(const Napi::CallbackInfo &info);
Napi::Value Step3BGenerateEphemeralKey(const Napi::CallbackInfo &info);
Napi::Value Step3BComputeSharedSecret(const Napi::CallbackInfo &info);
Napi::Value Step3CDeriveKeys(const Napi::CallbackInfo &info);
Napi::Value Step3DComputeAuthenticationToken(const Napi::CallbackInfo &info);
Napi::Value Step3DVerifyAuthenticationToken(const Napi::CallbackInfo &info);

}

#endif /* PACE_hpp */
