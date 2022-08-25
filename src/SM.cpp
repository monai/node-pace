#include "SM.hpp"
#include "Context.hpp"

namespace SM {

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Napi::Object sm = Napi::Object::New(env);

  sm.Set("setSSC", Napi::Function::New(env, SM::SetSSC));
  sm.Set("resetSSC", Napi::Function::New(env, SM::ResetSSC));
  sm.Set("incrementSSC", Napi::Function::New(env, SM::IncrementSSC));
  sm.Set("encrypt", Napi::Function::New(env, SM::Encrypt));
  sm.Set("decrypt", Napi::Function::New(env, SM::Decrypt));
  sm.Set("authenticate", Napi::Function::New(env, SM::Authenticate));
  sm.Set("verify", Napi::Function::New(env, SM::Verify));
  sm.Set("addPadding", Napi::Function::New(env, SM::AddPadding));
  sm.Set("removePadding", Napi::Function::New(env, SM::RemovePadding));

  exports.Set("SM", sm);

  return exports;
}

Napi::Value SetSSC(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Number ssc = info[1].As<Napi::Number>();

  int status = EAC_set_ssc(context->ctx(), ssc.Int64Value());
  if (status == 0) {
    Napi::Error::New(env, "Failed to set SSC").ThrowAsJavaScriptException();
  }

  return env.Undefined();
}

Napi::Value ResetSSC(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());

  int status = EAC_reset_ssc(context->ctx());
  if (status == 0) {
    Napi::Error::New(env, "Failed to reset SSC").ThrowAsJavaScriptException();
  }

  return env.Undefined();
}

Napi::Value IncrementSSC(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());

  int status = EAC_increment_ssc(context->ctx());
  if (status == 0) {
    Napi::Error::New(env, "Failed to increment SSC").ThrowAsJavaScriptException();
  }

  return env.Undefined();
}

Napi::Value Encrypt(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> plaintext = info[1].As<Napi::Buffer<uint8_t>>();

  BIO* plaintext_bio = BIO_new_mem_buf(plaintext.Data(), (int)plaintext.Length());
  BUF_MEM* plaintext_buf;
  BIO_get_mem_ptr(plaintext_bio, &plaintext_buf);

  BUF_MEM* ciphertext = EAC_encrypt(context->ctx(), plaintext_buf);
  BIO_free(plaintext_bio);

  if (ciphertext == nullptr) {
    Napi::Error::New(env, "Failed to encrypt").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> ciphertextObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)ciphertext->data, ciphertext->length);
  BUF_MEM_free(ciphertext);

  return ciphertextObj;
}

Napi::Value Decrypt(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> ciphertext = info[1].As<Napi::Buffer<uint8_t>>();

  BIO* ciphertext_bio = BIO_new_mem_buf(ciphertext.Data(), (int)ciphertext.Length());
  BUF_MEM* ciphertext_buf;
  BIO_get_mem_ptr(ciphertext_bio, &ciphertext_buf);

  BUF_MEM* plaintext = EAC_decrypt(context->ctx(), ciphertext_buf);
  BIO_free(ciphertext_bio);

  if (plaintext == nullptr) {
    Napi::Error::New(env, "Failed to decrypt").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> plaintextObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)plaintext->data, plaintext->length);
  BUF_MEM_free(plaintext);

  return plaintextObj;
}

Napi::Value Authenticate(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> data = info[1].As<Napi::Buffer<uint8_t>>();

  BIO* data_bio = BIO_new_mem_buf(data.Data(), (int)data.Length());
  BUF_MEM* data_buf;
  BIO_get_mem_ptr(data_bio, &data_buf);

  BUF_MEM* mac = EAC_authenticate(context->ctx(), data_buf);
  BIO_free(data_bio);

  if (mac == nullptr) {
    Napi::Error::New(env, "Failed to authenticate").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> macObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)mac->data, mac->length);
  BUF_MEM_free(mac);

  return macObj;
}

Napi::Boolean Verify(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> data = info[1].As<Napi::Buffer<uint8_t>>();
  Napi::Buffer<uint8_t> mac = info[2].As<Napi::Buffer<uint8_t>>();

  BIO* data_bio = BIO_new_mem_buf(data.Data(), (int)data.Length());
  BUF_MEM* data_buf;
  BIO_get_mem_ptr(data_bio, &data_buf);

  BIO* mac_bio = BIO_new_mem_buf(mac.Data(), (int)mac.Length());
  BUF_MEM* mac_buf;
  BIO_get_mem_ptr(mac_bio, &mac_buf);

  int status = EAC_verify_authentication(context->ctx(), data_buf, mac_buf);
  BIO_free(data_bio);
  BIO_free(mac_bio);

  return Napi::Boolean::New(env, status == 1);
}

Napi::Value AddPadding(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Context* context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
  Napi::Buffer<uint8_t> data = info[1].As<Napi::Buffer<uint8_t>>();

  BIO* data_bio = BIO_new_mem_buf(data.Data(), (int)data.Length());
  BUF_MEM* data_buf;
  BIO_get_mem_ptr(data_bio, &data_buf);

  BUF_MEM* paddedData = EAC_add_iso_pad(context->ctx(), data_buf);
  BIO_free(data_bio);

  if (paddedData == nullptr) {
    Napi::Error::New(env, "Failed to add padding").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> paddedDataObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)paddedData->data, paddedData->length);
  BUF_MEM_free(paddedData);

  return paddedDataObj;
}

Napi::Value RemovePadding(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Napi::Buffer<uint8_t> paddedData = info[0].As<Napi::Buffer<uint8_t>>();

  BIO* paddedData_bio = BIO_new_mem_buf(paddedData.Data(), (int)paddedData.Length());
  BUF_MEM* paddedData_buf;
  BIO_get_mem_ptr(paddedData_bio, &paddedData_buf);

  BUF_MEM* data = EAC_remove_iso_pad(paddedData_buf);
  BIO_free(paddedData_bio);

  if (data == nullptr) {
    Napi::Error::New(env, "Failed to remove padding").ThrowAsJavaScriptException();
    return Napi::Value();
  }

  Napi::Buffer<uint8_t> dataObj = Napi::Buffer<uint8_t>::Copy(env, (unsigned char *)data->data, data->length);
  BUF_MEM_free(data);

  return dataObj;
}

}
