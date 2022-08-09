#ifndef SM_hpp
#define SM_hpp

#include <napi.h>

namespace SM {

Napi::Object Init(Napi::Env env, Napi::Object exports);

Napi::Value SetSSC(const Napi::CallbackInfo &info);
Napi::Value ResetSSC(const Napi::CallbackInfo &info);
Napi::Value IncrementSSC(const Napi::CallbackInfo &info);
Napi::Value Encrypt(const Napi::CallbackInfo &info);
Napi::Value Decrypt(const Napi::CallbackInfo &info);
Napi::Value Authenticate(const Napi::CallbackInfo &info);
Napi::Boolean Verify(const Napi::CallbackInfo &info);
Napi::Value AddPadding(const Napi::CallbackInfo &info);
Napi::Value RemovePadding(const Napi::CallbackInfo &info);

}

#endif /* SM_hpp */
