#include "Constants.hpp"

#include <eac/eac.h>
#include <eac/pace.h>

Napi::Object Constants::Init(Napi::Env env, Napi::Object exports) {
  int bufferLength = 1024;
  char buffer[bufferLength];
  int len;

//  MARK: PACE

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_DH_GM_3DES_CBC_CBC), 1);
  exports.Set("id-PACE-DH-GM-3DES-CBC-CBC", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-DH-GM-3DES-CBC-CBC", Napi::Number::New(env, NID_id_PACE_DH_GM_3DES_CBC_CBC));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_DH_GM_AES_CBC_CMAC_128), 1);
  exports.Set("id-PACE-DH-GM-AES-CBC-CMAC-128", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-DH-GM-AES-CBC-CMAC-128", Napi::Number::New(env, NID_id_PACE_DH_GM_AES_CBC_CMAC_128));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_DH_GM_AES_CBC_CMAC_192), 1);
  exports.Set("id-PACE-DH-GM-AES-CBC-CMAC-192", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-DH-GM-AES-CBC-CMAC-192", Napi::Number::New(env, NID_id_PACE_DH_GM_AES_CBC_CMAC_192));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_DH_GM_AES_CBC_CMAC_256), 1);
  exports.Set("id-PACE-DH-GM-AES-CBC-CMAC-256", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-DH-GM-AES-CBC-CMAC-256", Napi::Number::New(env, NID_id_PACE_DH_GM_AES_CBC_CMAC_256));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_ECDH_GM_3DES_CBC_CBC), 1);
  exports.Set("id-PACE-ECDH-GM-3DES-CBC-CBC", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-ECDH-GM-3DES-CBC-CBC", Napi::Number::New(env, NID_id_PACE_ECDH_GM_3DES_CBC_CBC));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_ECDH_GM_AES_CBC_CMAC_128), 1);
  exports.Set("id-PACE-ECDH-GM-AES-CBC-CMAC-128", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-ECDH-GM-AES-CBC-CMAC-128", Napi::Number::New(env, NID_id_PACE_ECDH_GM_AES_CBC_CMAC_128));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_ECDH_GM_AES_CBC_CMAC_192), 1);
  exports.Set("id-PACE-ECDH-GM-AES-CBC-CMAC-192", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-ECDH-GM-AES-CBC-CMAC-192", Napi::Number::New(env, NID_id_PACE_ECDH_GM_AES_CBC_CMAC_192));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_ECDH_GM_AES_CBC_CMAC_256), 1);
  exports.Set("id-PACE-ECDH-GM-AES-CBC-CMAC-256", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-ECDH-GM-AES-CBC-CMAC-256", Napi::Number::New(env, NID_id_PACE_ECDH_GM_AES_CBC_CMAC_256));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_DH_IM_3DES_CBC_CBC), 1);
  exports.Set("id-PACE-DH-IM-3DES-CBC-CBC", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-DH-IM-3DES-CBC-CBC", Napi::Number::New(env, NID_id_PACE_DH_IM_3DES_CBC_CBC));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_DH_IM_AES_CBC_CMAC_128), 1);
  exports.Set("id-PACE-DH-IM-AES-CBC-CMAC-128", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-DH-IM-AES-CBC-CMAC-128", Napi::Number::New(env, NID_id_PACE_DH_IM_AES_CBC_CMAC_128));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_DH_IM_AES_CBC_CMAC_192), 1);
  exports.Set("id-PACE-DH-IM-AES-CBC-CMAC-192", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-DH-IM-AES-CBC-CMAC-192", Napi::Number::New(env, NID_id_PACE_DH_IM_AES_CBC_CMAC_192));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_DH_IM_AES_CBC_CMAC_256), 1);
  exports.Set("id-PACE-DH-IM-AES-CBC-CMAC-256", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-DH-IM-AES-CBC-CMAC-256", Napi::Number::New(env, NID_id_PACE_DH_IM_AES_CBC_CMAC_256));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_ECDH_IM_3DES_CBC_CBC), 1);
  exports.Set("id-PACE-ECDH-IM-3DES-CBC-CBC", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-ECDH-IM-3DES-CBC-CBC", Napi::Number::New(env, NID_id_PACE_ECDH_IM_3DES_CBC_CBC));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_ECDH_IM_AES_CBC_CMAC_128), 1);
  exports.Set("id-PACE-ECDH-IM-AES-CBC-CMAC-128", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-ECDH-IM-AES-CBC-CMAC-128", Napi::Number::New(env, NID_id_PACE_ECDH_IM_AES_CBC_CMAC_128));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_ECDH_IM_AES_CBC_CMAC_192), 1);
  exports.Set("id-PACE-ECDH-IM-AES-CBC-CMAC-192", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-ECDH-IM-AES-CBC-CMAC-192", Napi::Number::New(env, NID_id_PACE_ECDH_IM_AES_CBC_CMAC_192));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_PACE_ECDH_IM_AES_CBC_CMAC_256), 1);
  exports.Set("id-PACE-ECDH-IM-AES-CBC-CMAC-256", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PACE-ECDH-IM-AES-CBC-CMAC-256", Napi::Number::New(env, NID_id_PACE_ECDH_IM_AES_CBC_CMAC_256));

//  MARK: CA

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_CA_DH_3DES_CBC_CBC), 1);
  exports.Set("id-CA-DH-3DES-CBC-CBC", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CA-DH-3DES-CBC-CBC", Napi::Number::New(env, NID_id_CA_DH_3DES_CBC_CBC));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_CA_DH_AES_CBC_CMAC_128), 1);
  exports.Set("id-CA-DH-AES-CBC-CMAC-128", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CA-DH-AES-CBC-CMAC-128", Napi::Number::New(env, NID_id_CA_DH_AES_CBC_CMAC_128));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_CA_DH_AES_CBC_CMAC_192), 1);
  exports.Set("id-CA-DH-AES-CBC-CMAC-192", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CA-DH-AES-CBC-CMAC-192", Napi::Number::New(env, NID_id_CA_DH_AES_CBC_CMAC_192));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_CA_DH_AES_CBC_CMAC_256), 1);
  exports.Set("id-CA-DH-AES-CBC-CMAC-256", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CA-DH-AES-CBC-CMAC-256", Napi::Number::New(env, NID_id_CA_DH_AES_CBC_CMAC_256));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_CA_ECDH_3DES_CBC_CBC), 1);
  exports.Set("id-CA-ECDH-3DES-CBC-CBC", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CA-ECDH-3DES-CBC-CBC", Napi::Number::New(env, NID_id_CA_ECDH_3DES_CBC_CBC));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_CA_ECDH_AES_CBC_CMAC_128), 1);
  exports.Set("id-CA-ECDH-AES-CBC-CMAC-128", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CA-ECDH-AES-CBC-CMAC-128", Napi::Number::New(env, NID_id_CA_ECDH_AES_CBC_CMAC_128));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_CA_ECDH_AES_CBC_CMAC_192), 1);
  exports.Set("id-CA-ECDH-AES-CBC-CMAC-192", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CA-ECDH-AES-CBC-CMAC-192", Napi::Number::New(env, NID_id_CA_ECDH_AES_CBC_CMAC_192));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_CA_ECDH_AES_CBC_CMAC_256), 1);
  exports.Set("id-CA-ECDH-AES-CBC-CMAC-256", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CA-ECDH-AES-CBC-CMAC-256", Napi::Number::New(env, NID_id_CA_ECDH_AES_CBC_CMAC_256));

//  MARK: RI

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_DH_SHA_1), 1);
  exports.Set("id-RI-DH-SHA-1", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-DH-SHA-1", Napi::Number::New(env, NID_id_RI_DH_SHA_1));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_DH_SHA_224), 1);
  exports.Set("id-RI-DH-SHA-224", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-DH-SHA-224", Napi::Number::New(env, NID_id_RI_DH_SHA_224));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_DH_SHA_256), 1);
  exports.Set("id-RI-DH-SHA-256", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-DH-SHA-256", Napi::Number::New(env, NID_id_RI_DH_SHA_256));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_DH_SHA_384), 1);
  exports.Set("id-RI-DH-SHA-384", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-DH-SHA-384", Napi::Number::New(env, NID_id_RI_DH_SHA_384));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_DH_SHA_512), 1);
  exports.Set("id-RI-DH-SHA-512", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-DH-SHA-512", Napi::Number::New(env, NID_id_RI_DH_SHA_512));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_ECDH_SHA_1), 1);
  exports.Set("id-RI-ECDH-SHA-1", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-ECDH-SHA-1", Napi::Number::New(env, NID_id_RI_ECDH_SHA_1));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_ECDH_SHA_224), 1);
  exports.Set("id-RI-ECDH-SHA-224", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-ECDH-SHA-224", Napi::Number::New(env, NID_id_RI_ECDH_SHA_224));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_ECDH_SHA_256), 1);
  exports.Set("id-RI-ECDH-SHA-256", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-ECDH-SHA-256", Napi::Number::New(env, NID_id_RI_ECDH_SHA_256));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_ECDH_SHA_384), 1);
  exports.Set("id-RI-ECDH-SHA-384", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-ECDH-SHA-384", Napi::Number::New(env, NID_id_RI_ECDH_SHA_384));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(NID_id_RI_ECDH_SHA_512), 1);
  exports.Set("id-RI-ECDH-SHA-512", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RI-ECDH-SHA-512", Napi::Number::New(env, NID_id_RI_ECDH_SHA_512));

//  MARK: Password Types

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(PACE_MRZ), 1);
  exports.Set("id-MRZ", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-MRZ", Napi::Number::New(env, PACE_MRZ));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(PACE_CAN), 1);
  exports.Set("id-CAN", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-CAN", Napi::Number::New(env, PACE_CAN));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(PACE_PIN), 1);
  exports.Set("id-PIN", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PIN", Napi::Number::New(env, PACE_PIN));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(PACE_PUK), 1);
  exports.Set("id-PUK", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-PUK", Napi::Number::New(env, PACE_PUK));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(PACE_RAW), 1);
  exports.Set("id-RAW", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-RAW", Napi::Number::New(env, PACE_RAW));

  len = OBJ_obj2txt(buffer, bufferLength, EAC_OBJ_nid2obj(PACE_SEC_UNDEF), 1);
  exports.Set("id-SEC-UNDEF", Napi::String::New(env, buffer, len));
  exports.Set("NID_id-SEC-UNDEF", Napi::Number::New(env, PACE_SEC_UNDEF));

//  MARK: Encryption Context

  exports.Set("EAC_ID_PACE", Napi::Number::New(env, EAC_ID_PACE));
  exports.Set("EAC_ID_CA", Napi::Number::New(env, EAC_ID_CA));
  exports.Set("EAC_ID_EAC", Napi::Number::New(env, EAC_ID_EAC));

  return exports;
}
