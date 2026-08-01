#include <memory>
#include <string>

#include "compact_lang_det.h"
#include "encodings.h"
#include "constants.h"
#include "cld_core.h"

using std::terminate_handler;

#define NAPI_VERSION 4
#include <napi.h>

namespace NodeCld {
  std::unique_ptr<CLDInput> UnpackInputFromJSArgs(const Napi::CallbackInfo &info) {
    std::unique_ptr<CLDInput> input(new CLDInput);

    input->bytes = info[0].ToString().Utf8Value();
    input->numBytes = input->bytes.length();
    input->isPlainText = info[1].ToBoolean();

    if (info[2].IsString()) {
      input->languageHint = info[2].ToString().Utf8Value();
    }

    if (info[3].IsString()) {
      input->encodingHint = info[3].ToString().Utf8Value();
    }

    if (info[4].IsString()) {
      input->tldHint = info[4].ToString().Utf8Value();
    }

    if (info[5].IsString()) {
      input->httpHint = info[5].ToString().Utf8Value();
    }
    input->bestEffort = info[6].ToBoolean();

    return input;
  }

  Napi::Object UnpackOutputToJS(const Napi::Env env, std::unique_ptr<CLDOutput> output) {
    size_t languageIdx = 0;
    auto languages = Napi::Array::New(env);
    for (size_t resultIdx = 0; resultIdx < 3; resultIdx++) {
      CLD2::Language lang = output->language3[resultIdx];

      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }

      auto item = Napi::Object::New(env);
      item["name"] = Napi::String::New(env, Constants::getInstance().getLanguageName(lang));
      item["code"] = Napi::String::New(env, Constants::getInstance().getLanguageCode(lang));
      item["percent"] = Napi::Number::New(env, output->percent3[resultIdx]);
      item["score"] = Napi::Number::New(env, output->normalized_score3[resultIdx]);

      languages[languageIdx++] = item;
    }

    size_t chunkIdx = 0;
    auto chunks = Napi::Array::New(env);
    for (size_t resultIdx = 0; resultIdx < output->resultChunkVector.size(); resultIdx++) {
      CLD2::ResultChunk chunk = output->resultChunkVector.at(resultIdx);
      CLD2::Language lang = static_cast<CLD2::Language>(chunk.lang1);

      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }

      auto item = Napi::Object::New(env);
      item["name"] = Napi::String::New(env, Constants::getInstance().getLanguageName(lang));
      item["code"] = Napi::String::New(env, Constants::getInstance().getLanguageCode(lang));
      item["offset"] = Napi::Number::New(env, chunk.offset);
      item["bytes"] = Napi::Number::New(env, chunk.bytes);

      chunks[chunkIdx++] = item;
    }

    auto results = Napi::Object::New(env);
    results["reliable"] = Napi::Boolean::New(env, output->isReliable);
    results["textBytes"] = Napi::Number::New(env, output->textBytesFound);
    results["languages"] = languages;
    results["chunks"] = chunks;

    return results;
  }

  class DetectAsyncWorker : public Napi::AsyncWorker {
    public:
      DetectAsyncWorker(const Napi::CallbackInfo &info):
        Napi::AsyncWorker(info.Env()),
        deferred(Napi::Promise::Deferred::New(info.Env())),
        mInput(UnpackInputFromJSArgs(info))
      {}

      void Execute() {
        mOutput = DetectLanguage(*mInput);
      }

      void OnOK() {
        deferred.Resolve(UnpackOutputToJS(Env(), std::move(mOutput)));
      }

      Napi::Promise Promise() {
        this->Queue();
        return deferred.Promise();
      }

    private:
      Napi::Promise::Deferred deferred;
      std::unique_ptr<CLDInput> mInput;
      std::unique_ptr<CLDOutput> mOutput;
  };

  Napi::Object Detect(const Napi::CallbackInfo &info) {
    auto input = UnpackInputFromJSArgs(info);
    auto output = DetectLanguage(*input);
    return UnpackOutputToJS(info.Env(), std::move(output));
  }

  Napi::Promise DetectAsync(const Napi::CallbackInfo &info) {
    return (new DetectAsyncWorker(info))->Promise();
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports) {
    auto rawDetected = Constants::getInstance().getDetected();
    auto numDetected = rawDetected->size();
    auto detected = Napi::Array::New(env, numDetected);
    for (size_t i = 0; i < rawDetected->size(); i++) {
      auto rawLanguage = rawDetected->at(i);
      detected[i] = Napi::String::New(env, rawLanguage.name);
    }
    exports["DETECTED_LANGUAGES"] = detected;

    auto languages = Napi::Object::New(env);
    auto rawLanguages = Constants::getInstance().getLanguages();
    for (size_t i = 0; i < rawLanguages->size(); i++) {
      auto rawLanguage = rawLanguages->at(i);
      languages[rawLanguage.name] = Napi::String::New(env, rawLanguage.code);
    }
    exports["LANGUAGES"] = languages;

    auto rawEncodings = Constants::getInstance().getEncodings();
    auto numEncodings = rawEncodings->size();
    auto encodings = Napi::Array::New(env, numEncodings);
    for (size_t i = 0; i < numEncodings; i++) {
      auto rawEncoding = rawEncodings->at(i);
      encodings[i] = Napi::String::New(env, rawEncoding.name);
    }
    exports["ENCODINGS"] = encodings;

    exports["detect"] = Napi::Function::New(env, Detect);
    exports["detectAsync"] = Napi::Function::New(env, DetectAsync);
    return exports;
  }
}

using NodeCld::Init;
NODE_API_MODULE(cld, Init);
