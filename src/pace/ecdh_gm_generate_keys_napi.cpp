#include "../napi.h"
#include "ecdh_gm.hpp"
#include "ecdh_gm_generate_keys_napi.hpp"

#include <openssl/ec.h>
#include <openssl/objects.h>

namespace pace {
namespace ecdh_gm_generate_keys {
namespace napi {

napi_value generate_keys(napi_env env, napi_callback_info args) {
  size_t argc = 3;
  napi_value* argv = new napi_value[argc];
  NAPI_CALL(env, napi_get_cb_info(env, args, &argc, argv, nullptr, nullptr));

  bool is_buffer;
  napi_valuetype value_type;

  NAPI_CALL(env, napi_is_buffer(env, argv[0], &is_buffer));
  if (!is_buffer) {
    napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE",
                          "The first argument must be an instance of Buffer");
    return nullptr;
  }

  NAPI_CALL(env, napi_typeof(env, argv[1], &value_type));
  if (value_type != napi_string) {
    napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE",
                          "The second argument must be a string");
    return nullptr;
  }

  NAPI_CALL(env, napi_typeof(env, argv[2], &value_type));
  if (value_type != napi_function) {
    napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE",
                          "The third argument must be a function");
    return nullptr;
  }

  size_t datal;
  unsigned char* data;
  std::vector<unsigned char> generator;
  size_t curve_namel = 0xff;
  char* curve_name = new char[curve_namel];

  NAPI_CALL(env, napi_get_buffer_info(env, argv[0], (void**)&data, &datal));
  generator = std::vector<unsigned char>(data, data + datal);

  curve_name = new char[0xff];
  NAPI_CALL(env, napi_get_value_string_utf8(env, argv[1], curve_name,
                                            curve_namel, &datal));

  napi_ref callback;
  NAPI_CALL(env, napi_create_reference(env, argv[2], 1, &callback));

  napi_value resource_name;
  NAPI_CALL(env, napi_create_string_utf8(env, "generateKeys", NAPI_AUTO_LENGTH,
                                         &resource_name));

  generate_keys_data* worker_data = new generate_keys_data;
  worker_data->generator = generator;
  worker_data->curve_name = std::string(curve_name, curve_namel);
  worker_data->callback = callback;

  NAPI_CALL(env, napi_create_async_work(
                     env, nullptr, resource_name, generate_keys_execute,
                     generate_keys_complete, worker_data, &worker_data->work));

  NAPI_CALL(env, napi_queue_async_work(env, worker_data->work));

  napi_value out;
  NAPI_CALL(env, napi_get_undefined(env, &out));

  delete[] argv;

  return out;
}

void generate_keys_execute(napi_env env, void* data) {
  generate_keys_data* worker_data = static_cast<generate_keys_data*>(data);

  EC_KEY* key_pair = nullptr;
  const EC_GROUP* group;
  EC_POINT* generator = nullptr;

  int ret = 1;
  int status = 0;
  size_t len;

  int nid;
  nid = OBJ_sn2nid(worker_data->curve_name.c_str());
  if (nid == OBJ_undef) {
    status = 1;
    worker_data->error.capture("Unknown curve name");
  }

  if (status == 0) {
    key_pair = EC_KEY_new_by_curve_name(nid);
    if (key_pair == nullptr) {
      status = 2;
    }
  }

  if (ret == 1 && status == 0) {
    group = EC_KEY_get0_group(key_pair);
    if (group == nullptr) {
      status = 3;
    }
  }

  if (ret == 1 && status == 0) {
    generator = EC_POINT_new(group);
    if (generator == nullptr) {
      status = 4;
    }
  }

  if (ret == 1 && status == 0) {
    ret = EC_POINT_oct2point(group, generator, worker_data->generator.data(),
                             worker_data->generator.size(), nullptr);
  }

  if (ret == 1 && status == 0) {
    ret = ecdh_gm::generate_key_pair(group, generator, key_pair);
  }

  unsigned char* buffer;
  if (ret == 1 && status == 0) {
    len = EC_KEY_key2buf(key_pair, POINT_CONVERSION_UNCOMPRESSED, &buffer,
                         nullptr);
    if (len == 0) {
      status = 5;
    }
  }

  if (ret == 1 && status == 0) {
    worker_data->public_key = std::vector<unsigned char>(buffer, buffer + len);
    free(buffer);
  }

  if (ret == 1 && status == 0) {
    len = EC_KEY_priv2buf(key_pair, &buffer);
    if (len == 0) {
      status = 6;
    }
  }

  if (ret == 1 && status == 0) {
    worker_data->private_key = std::vector<unsigned char>(buffer, buffer + len);
    free(buffer);
  }

  if (key_pair != nullptr) {
    EC_KEY_free(key_pair);
  }

  if (key_pair != nullptr) {
    EC_POINT_free(generator);
  }

  if ((ret != 1 || status != 0) && worker_data->error.message.empty()) {
    worker_data->error.capture();
  }
}
void generate_keys_complete(napi_env env, napi_status status, void* data) {
  generate_keys_data* worker_data = static_cast<generate_keys_data*>(data);

  napi_value global;
  NAPI_CALL_RETURN_VOID(env, napi_get_global(env, &global));

  napi_value public_key;
  napi_value private_key;

  NAPI_CALL_RETURN_VOID(
      env, napi_create_buffer_copy(env, worker_data->public_key.size(),
                                   worker_data->public_key.data(), nullptr,
                                   &public_key));

  NAPI_CALL_RETURN_VOID(
      env, napi_create_buffer_copy(env, worker_data->private_key.size(),
                                   worker_data->private_key.data(), nullptr,
                                   &private_key));

  napi_value callback;
  NAPI_CALL_RETURN_VOID(
      env, napi_get_reference_value(env, worker_data->callback, &callback));

  std::vector<napi_value> argv = std::vector<napi_value>();
  if (!worker_data->error.message.empty()) {
    napi_value error;
    NAPI_CALL_RETURN_VOID(env, worker_data->error.to_napi_error(env, &error));

    argv.push_back(error);
  } else {
    napi_value undefined;
    NAPI_CALL_RETURN_VOID(env, napi_get_undefined(env, &undefined));

    napi_value key_pair;
    NAPI_CALL_RETURN_VOID(env, napi_create_array(env, &key_pair));
    NAPI_CALL_RETURN_VOID(env, napi_set_element(env, key_pair, 0, public_key));
    NAPI_CALL_RETURN_VOID(env, napi_set_element(env, key_pair, 1, private_key));

    argv.push_back(undefined);
    argv.push_back(key_pair);
  }

  delete worker_data;

  napi_value* argva = new napi_value[argv.size()];
  std::copy(argv.begin(), argv.end(), argva);

  NAPI_CALL_RETURN_VOID(env, napi_call_function(env, global, callback,
                                                argv.size(), argva, nullptr));
}

}  // namespace napi
}  // namespace ecdh_gm_generate_keys
}  // namespace pace
