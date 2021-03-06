#include "../napi.h"
#include "ecdh_gm.hpp"
#include "ecdh_gm_map_napi.hpp"

#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/objects.h>

namespace pace {
namespace ecdh_gm_map {
namespace napi {

napi_value map(napi_env env, napi_callback_info args) {
  size_t argc = 4;
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

  NAPI_CALL(env, napi_is_buffer(env, argv[1], &is_buffer));
  if (!is_buffer) {
    napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE",
                          "The second argument must be an instance of Buffer");
    return nullptr;
  }

  NAPI_CALL(env, napi_is_buffer(env, argv[2], &is_buffer));
  if (!is_buffer) {
    napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE",
                          "The third argument must be an instance of Buffer");
    return nullptr;
  }

  NAPI_CALL(env, napi_typeof(env, argv[3], &value_type));
  if (value_type != napi_string) {
    napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE",
                          "The fourth argument must be a string");
    return nullptr;
  }

  size_t datal;
  unsigned char* data;
  std::vector<unsigned char> pcd_private_key;
  std::vector<unsigned char> ic_public_key;
  std::vector<unsigned char> nonce;

  NAPI_CALL(env, napi_get_buffer_info(env, argv[0], (void**)&data, &datal));
  pcd_private_key = std::vector<unsigned char>(data, data + datal);

  NAPI_CALL(env, napi_get_buffer_info(env, argv[1], (void**)&data, &datal));
  ic_public_key = std::vector<unsigned char>(data, data + datal);

  NAPI_CALL(env, napi_get_buffer_info(env, argv[2], (void**)&data, &datal));
  nonce = std::vector<unsigned char>(data, data + datal);

  size_t curve_namel = 0xff;
  char* curve_name = new char[curve_namel];
  NAPI_CALL(env, napi_get_value_string_utf8(env, argv[3], curve_name,
                                            curve_namel, &datal));

  delete[] argv;

  map_data* worker_data = new map_data;
  worker_data->pcd_private_key = pcd_private_key;
  worker_data->ic_public_key = ic_public_key;
  worker_data->nonce = nonce;
  worker_data->curve_name = std::string(curve_name, curve_namel);

  map_execute(env, worker_data);
  napi_value out = map_complete(env, worker_data);

  delete worker_data;

  return out;
}

// G~ = [s]G + H
void map_execute(napi_env env, void* data) {
  map_data* worker_data = static_cast<map_data*>(data);

  EC_KEY* key_pair = nullptr;
  const EC_GROUP* group;
  EC_POINT* ic_public_key = nullptr;

  // H
  EC_POINT* shared_secret = nullptr;
  // G~
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

  if (status == 0) {
    ret = EC_KEY_oct2priv(key_pair, worker_data->pcd_private_key.data(),
                          worker_data->pcd_private_key.size());
  }

  if (ret == 1 && status == 0) {
    group = EC_KEY_get0_group(key_pair);
    if (group == nullptr) {
      status = 3;
    }
  }

  if (ret == 1 && status == 0) {
    ic_public_key = EC_POINT_new(group);
    if (ic_public_key == nullptr) {
      status = 4;
    }
  }

  if (ret == 1 && status == 0) {
    ret = EC_POINT_oct2point(group, ic_public_key,
                             worker_data->ic_public_key.data(),
                             worker_data->ic_public_key.size(), nullptr);
  }

  if (ret == 1 && status == 0) {
    shared_secret = EC_POINT_new(group);
    if (shared_secret == nullptr) {
      status = 5;
    }
  }

  if (ret == 1 && status == 0) {
    ret = ecdh_gm::calculate_shared_secret(key_pair, ic_public_key,
                                           shared_secret);
  }

  if (ret == 1 && status == 0) {
    generator = EC_POINT_new(group);
    if (generator == nullptr) {
      status = 6;
    }
  }

  if (ret == 1 && status == 0) {
    ret = ecdh_gm::map_nonce_to_generator(group, worker_data->nonce,
                                          shared_secret, generator);
  }

  unsigned char* buffer;
  if (ret == 1 && status == 0) {
    len = EC_POINT_point2buf(group, generator, POINT_CONVERSION_UNCOMPRESSED,
                             &buffer, nullptr);
    if (len == 0) {
      status = 7;
    }
  }

  if (ret == 1 && status == 0) {
    worker_data->generator = std::vector<unsigned char>(buffer, buffer + len);
    free(buffer);
  }

  if (key_pair != nullptr) {
    EC_KEY_free(key_pair);
  }
  if (ic_public_key != nullptr) {
    EC_POINT_free(ic_public_key);
  }
  if (shared_secret != nullptr) {
    EC_POINT_free(shared_secret);
  }
  if (generator != nullptr) {
    EC_POINT_free(generator);
  }

  if ((ret != 1 || status != 0) && worker_data->error.message.empty()) {
    worker_data->error.capture();
  }
}
napi_value map_complete(napi_env env, void* data) {
  map_data* worker_data = static_cast<map_data*>(data);

  if (!worker_data->error.message.empty()) {
    napi_value error;
    NAPI_CALL(env, worker_data->error.to_napi_error(env, &error));

    return error;
  }

  napi_value generator;
  NAPI_CALL(env, napi_create_buffer_copy(env, worker_data->generator.size(),
                                         worker_data->generator.data(), nullptr,
                                         &generator));

  return generator;
}

}  // namespace napi
}  // namespace ecdh_gm_map
}  // namespace pace
