#include "../napi.h"
#include "ecdh_gm.hpp"
#include "ecdh_gm_generate_keys_napi.hpp"

#include <openssl/ec.h>
#include <openssl/objects.h>

namespace pace {
namespace ecdh_gm_generate_keys {
namespace napi {

napi_value generate_keys(napi_env env, napi_callback_info args) {
  size_t argc = 2;
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

  size_t datal;
  unsigned char* data;
  std::vector<unsigned char> generator;

  NAPI_CALL(env, napi_get_buffer_info(env, argv[0], (void**)&data, &datal));
  generator = std::vector<unsigned char>(data, data + datal);

  size_t curve_namel = 0xff;
  char* curve_name = new char[curve_namel];
  NAPI_CALL(env, napi_get_value_string_utf8(env, argv[1], curve_name,
                                            curve_namel, &datal));

  delete[] argv;

  generate_keys_data* worker_data = new generate_keys_data;
  worker_data->generator = generator;
  worker_data->curve_name = std::string(curve_name, curve_namel);

  generate_keys_execute(env, worker_data);
  napi_value out = generate_keys_complete(env, worker_data);

  delete worker_data;

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

  if (generator != nullptr) {
    EC_POINT_free(generator);
  }

  if ((ret != 1 || status != 0) && worker_data->error.message.empty()) {
    worker_data->error.capture();
  }
}
napi_value generate_keys_complete(napi_env env, void* data) {
  generate_keys_data* worker_data = static_cast<generate_keys_data*>(data);

  if (!worker_data->error.message.empty()) {
    napi_value error;
    NAPI_CALL(env, worker_data->error.to_napi_error(env, &error));

    return error;
  }

  napi_value public_key;
  napi_value private_key;

  NAPI_CALL(env, napi_create_buffer_copy(env, worker_data->public_key.size(),
                                         worker_data->public_key.data(),
                                         nullptr, &public_key));

  NAPI_CALL(env, napi_create_buffer_copy(env, worker_data->private_key.size(),
                                         worker_data->private_key.data(),
                                         nullptr, &private_key));

  napi_value key_pair;
  NAPI_CALL(env, napi_create_array(env, &key_pair));
  NAPI_CALL(env, napi_set_element(env, key_pair, 0, public_key));
  NAPI_CALL(env, napi_set_element(env, key_pair, 1, private_key));

  return key_pair;
}

}  // namespace napi
}  // namespace ecdh_gm_generate_keys
}  // namespace pace
