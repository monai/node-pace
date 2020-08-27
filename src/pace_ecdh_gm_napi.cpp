#include "napi.h"
#include "pace_ecdh_gm.hpp"
#include "pace_ecdh_gm_napi.hpp"

#include <openssl/obj_mac.h>

namespace pace {
namespace ecdh_gm {
namespace napi {

napi_value build_do83(napi_env env, napi_callback_info args) {
  size_t argc = 5;
  napi_value argv[argc];
  NAPI_CALL(env, napi_get_cb_info(env, args, &argc, argv, nullptr, nullptr));

  bool is_buffer;
  char message[0xff];
  std::vector<std::string> order = {"first", "second", "third", "fourth"};
  for (size_t i = 0; i < order.size(); i++) {
    NAPI_CALL(env, napi_is_buffer(env, argv[i], &is_buffer));

    if (!is_buffer) {
      snprintf(message, sizeof(message),
               "The %s argument must be an instance of Buffer",
               order[i].c_str());

      napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE", message);
      return nullptr;
    }
  }

  napi_valuetype callback_type;
  NAPI_CALL(env, napi_typeof(env, argv[4], &callback_type));

  if (callback_type != napi_function) {
    napi_throw_type_error(env, "ERR_INVALID_ARG_TYPE",
                          "The fifth argument must be a function");
    return nullptr;
  }

  size_t datal;
  unsigned char* data;
  std::vector<unsigned char> nonce;
  std::vector<unsigned char> pcd_public_key;
  std::vector<unsigned char> pcd_private_key;
  std::vector<unsigned char> ic_public_key;

  NAPI_CALL(env, napi_get_buffer_info(env, argv[0], (void**)&data, &datal));
  nonce = std::vector<unsigned char>(data, data + datal);

  NAPI_CALL(env, napi_get_buffer_info(env, argv[1], (void**)&data, &datal));
  pcd_public_key = std::vector<unsigned char>(data, data + datal);

  NAPI_CALL(env, napi_get_buffer_info(env, argv[2], (void**)&data, &datal));
  pcd_private_key = std::vector<unsigned char>(data, data + datal);

  NAPI_CALL(env, napi_get_buffer_info(env, argv[3], (void**)&data, &datal));
  ic_public_key = std::vector<unsigned char>(data, data + datal);

  napi_ref callback;
  NAPI_CALL(env, napi_create_reference(env, argv[4], 1, &callback));

  napi_value resource_name;
  NAPI_CALL(env, napi_create_string_utf8(env, "build_do83", NAPI_AUTO_LENGTH,
                                         &resource_name));

  do83_data* do_data = new do83_data;
  do_data->nonce = nonce;
  do_data->pcd_public_key = pcd_public_key;
  do_data->pcd_private_key = pcd_private_key;
  do_data->ic_public_key = ic_public_key;
  do_data->callback = callback;

  NAPI_CALL(env, napi_create_async_work(env, nullptr, resource_name,
                                        build_do83_execute, build_do83_complete,
                                        do_data, &do_data->work));

  NAPI_CALL(env, napi_queue_async_work(env, do_data->work));

  napi_value out;
  NAPI_CALL(env, napi_get_undefined(env, &out));

  return out;
}

void build_do83_execute(napi_env env, void* data) {
  do83_data* do_data = static_cast<do83_data*>(data);

  EC_KEY* pcd_key_pair = nullptr;
  EC_KEY* ephemeral_key_pair = EC_KEY_new();
  const EC_GROUP* group;
  EC_POINT* ic_public_key = nullptr;
  unsigned char* buffer;

  int ret = 1;
  int status = 0;
  size_t len;

  //  ec_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
  pcd_key_pair = EC_KEY_new_by_curve_name(NID_brainpoolP256r1);
  if (pcd_key_pair == nullptr) {
    status = 1;
  }

  if (status == 0) {
    ret = EC_KEY_oct2key(pcd_key_pair, do_data->pcd_public_key.data(),
                         do_data->pcd_public_key.size(), nullptr);
  }

  if (ret == 1 && status == 0) {
    ret = EC_KEY_oct2priv(pcd_key_pair, do_data->pcd_private_key.data(),
                          do_data->pcd_private_key.size());
  }

  if (ret == 1 && status == 0) {
    group = EC_KEY_get0_group(pcd_key_pair);
    if (group == nullptr) {
      status = 1;
    }
  }

  if (ret == 1 && status == 0) {
    ic_public_key = EC_POINT_new(group);
    if (ic_public_key == nullptr) {
      status = 2;
    }
  }

  if (ret == 1 && status == 0) {
    ret =
        EC_POINT_oct2point(group, ic_public_key, do_data->ic_public_key.data(),
                           do_data->ic_public_key.size(), nullptr);
  }

  if (ret == 1 && status == 0) {
    ret = calculate_ephemeral_key(pcd_key_pair, ic_public_key, do_data->nonce,
                                  ephemeral_key_pair);
  }

  if (ret == 1 && status == 0) {
    len = EC_KEY_key2buf(ephemeral_key_pair, POINT_CONVERSION_UNCOMPRESSED,
                         &buffer, nullptr);
    if (len == 0) {
      status = 3;
    }
  }

  if (ret == 1 && status == 0) {
    do_data->pcd_ephemeral_public_key =
        std::vector<unsigned char>(buffer, buffer + len);
    free(buffer);
  }

  if (ret == 1 && status == 0) {
    len = EC_KEY_priv2buf(ephemeral_key_pair, &buffer);
    if (len == 0) {
      status = 4;
    }
  }

  if (ret == 1 && status == 0) {
    do_data->pcd_ephemeral_private_key =
        std::vector<unsigned char>(buffer, buffer + len);
    free(buffer);
  }

  if (pcd_key_pair != nullptr) {
    EC_KEY_free(pcd_key_pair);
  }
  if (ephemeral_key_pair != nullptr) {
    EC_KEY_free(ephemeral_key_pair);
  }
  if (ic_public_key != nullptr) {
    EC_POINT_free(ic_public_key);
  }

  if (ret != 1 && status != 0) {
    do_data->error = crypto_error();
  }
}

void build_do83_complete(napi_env env, napi_status status, void* data) {
  do83_data* do_data = static_cast<do83_data*>(data);

  if (!do_data->error_code.empty() || !do_data->error_message.empty()) {
    napi_throw_error(env, do_data->error_code.c_str(),
                     do_data->error_message.c_str());
  }

  napi_value global;
  NAPI_CALL_RETURN_VOID(env, napi_get_global(env, &global));

  napi_value pcd_ephemeral_public_key;
  napi_value pcd_ephemeral_private_key;

  NAPI_CALL_RETURN_VOID(env, napi_create_buffer_copy(
                                 env, do_data->pcd_ephemeral_public_key.size(),
                                 do_data->pcd_ephemeral_public_key.data(),
                                 nullptr, &pcd_ephemeral_public_key));

  NAPI_CALL_RETURN_VOID(env, napi_create_buffer_copy(
                                 env, do_data->pcd_ephemeral_private_key.size(),
                                 do_data->pcd_ephemeral_private_key.data(),
                                 nullptr, &pcd_ephemeral_private_key));

  napi_value callback;
  NAPI_CALL_RETURN_VOID(
      env, napi_get_reference_value(env, do_data->callback, &callback));

  std::vector<napi_value> argv = std::vector<napi_value>();
  if (do_data->error.err != 0) {
    napi_value error;
    NAPI_CALL_RETURN_VOID(env, do_data->error.to_napi_error(env, &error));

    argv.push_back(error);
  } else {
    napi_value undefined;
    NAPI_CALL_RETURN_VOID(env, napi_get_undefined(env, &undefined));

    argv.push_back(undefined);
    argv.push_back(pcd_ephemeral_public_key);
    argv.push_back(pcd_ephemeral_private_key);
  }

  delete do_data;

  napi_value argva[argv.size()];
  std::copy(argv.begin(), argv.end(), argva);

  NAPI_CALL_RETURN_VOID(env, napi_call_function(env, global, callback,
                                                argv.size(), argva, nullptr));
}

}  // namespace napi
}  // namespace ecdh_gm
}  // namespace pace
