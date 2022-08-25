#ifndef Context_hpp
#define Context_hpp

#include <eac/eac.h>
#include <napi.h>

class Context: public Napi::ObjectWrap<Context> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Context(const Napi::CallbackInfo& info);
  ~Context();
  EAC_CTX* ctx();

  Napi::Value InitPACE(const Napi::CallbackInfo& info);
  Napi::Value InitTA(const Napi::CallbackInfo& info);
  Napi::Value InitCA(const Napi::CallbackInfo& info);
  Napi::Value InitRI(const Napi::CallbackInfo& info);
  Napi::Value InitEFCardAccess(const Napi::CallbackInfo& info);
  Napi::Value InitEFCardSecurity(const Napi::CallbackInfo& info);
  Napi::Value SetSMContext(const Napi::CallbackInfo& info);

private:
  EAC_CTX* _ctx;
};

#endif /* Context_hpp */
