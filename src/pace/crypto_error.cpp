#include "../napi.h"
#include "crypto_error.hpp"

#include <openssl/err.h>

namespace pace {

void crypto_error::capture() {
  capture(std::string());
}

void crypto_error::capture(std::string message) {
  this->message = message;

  err = ERR_get_error();
  if (err > 0 && message.empty()) {
    char buf[0xff];
    ERR_error_string_n(err, buf, sizeof(buf));
    message = std::string(buf);
  }

  clear();
  while (auto err = ERR_get_error()) {
    char buf[0xff];
    ERR_error_string_n(err, buf, sizeof(buf));
    push_back(buf);
  }
  std::reverse(begin(), end());
}

napi_status crypto_error::library_string(napi_env env, napi_value* result) {
  const char* ls = ERR_lib_error_string(err);
  if (ls == nullptr) {
    return napi_generic_failure;
  }

  return napi_create_string_utf8(env, ls, NAPI_AUTO_LENGTH, result);
}

napi_status crypto_error::function_string(napi_env env, napi_value* result) {
  const char* ls = ERR_func_error_string(err);
  if (ls == nullptr) {
    return napi_generic_failure;
  }

  return napi_create_string_utf8(env, ls, NAPI_AUTO_LENGTH, result);
}

napi_status crypto_error::reason_string(napi_env env, napi_value* result) {
  const char* ls = ERR_reason_error_string(err);
  if (ls == nullptr) {
    return napi_generic_failure;
  }

  return napi_create_string_utf8(env, ls, NAPI_AUTO_LENGTH, result);
}

napi_status crypto_error::to_napi_error(napi_env env, napi_value* error) {
  napi_value msg;
  NAPI_CALL_RETURN(
      napi_create_string_utf8(env, message.c_str(), message.size(), &msg));
  NAPI_CALL_RETURN(napi_create_error(env, nullptr, msg, error));

  if (err == 0) {
    return napi_ok;
  }

  napi_value library;
  napi_value function;
  napi_value reason;

  NAPI_CALL_RETURN(library_string(env, &library));
  NAPI_CALL_RETURN(function_string(env, &function));
  NAPI_CALL_RETURN(reason_string(env, &reason));

  NAPI_CALL_RETURN(napi_set_named_property(env, *error, "library", library));
  NAPI_CALL_RETURN(napi_set_named_property(env, *error, "function", function));
  NAPI_CALL_RETURN(napi_set_named_property(env, *error, "reason", reason));

  napi_value openssl_error_stack;
  NAPI_CALL_RETURN(
      napi_create_array_with_length(env, size(), &openssl_error_stack));

  std::string cur_msg;
  for (size_type i = 0; i < size(); ++i) {
    cur_msg = at(i);
    NAPI_CALL_RETURN(
        napi_create_string_utf8(env, cur_msg.c_str(), cur_msg.size(), &msg));
    NAPI_CALL_RETURN(
        napi_set_element(env, *error, static_cast<uint32_t>(i), msg));
  }

  return napi_ok;
}

}  // namespace pace
