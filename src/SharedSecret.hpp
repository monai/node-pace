#ifndef PACESecret_hpp
#define PACESecret_hpp

#include <eac/pace.h>
#include <napi.h>

class SharedSecret: public Napi::ObjectWrap<SharedSecret> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  SharedSecret(const Napi::CallbackInfo& info);
  ~SharedSecret();
  PACE_SEC* sec();

private:
  PACE_SEC* _sec;
};


#endif /* PACESecret_hpp */
