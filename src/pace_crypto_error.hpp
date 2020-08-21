#ifndef pace_crypto_error_hpp
#define pace_crypto_error_hpp

#include <node_api.h>

#include <string>
#include <vector>

namespace pace {

class crypto_error : public std::vector<std::string> {
 public:
  unsigned long err;
  std::string message;

  crypto_error();
  napi_status to_napi_error(napi_env env, napi_value* error);

 private:
  void capture();
  napi_status library_string(napi_env env, napi_value* result);
  napi_status function_string(napi_env env, napi_value* result);
  napi_status reason_string(napi_env env, napi_value* result);
};

}  // namespace pace

#endif
