#include <memory>
#include <string>

#include "compact_lang_det.h"
#include "encodings.h"
#include "constants.h"

using std::terminate_handler;
using std::unexpected_handler;

#define NAPI_VERSION 4
#include <napi.h>

namespace NodeCld {
  struct CLDInput {
    std::string bytes,
      languageHint,
      encodingHint,
      tldHint,
      httpHint;
    int numBytes;
    bool isPlainText;
  };

  struct CLDOutput {
    CLD2::Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    CLD2::ResultChunkVector resultChunkVector;
    int textBytesFound;
    bool isReliable;
  };

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

    return input;
  }

  std::unique_ptr<CLDOutput> DetectLanguage(std::unique_ptr<CLDInput> input) {
    std::unique_ptr<CLDOutput> output(new CLDOutput);
    CLD2::CLDHints hints;
    hints.tld_hint = 0;
    hints.content_language_hint = 0;
    hints.language_hint = CLD2::UNKNOWN_LANGUAGE;
    hints.encoding_hint = CLD2::UNKNOWN_ENCODING;

    if (input->languageHint.length() > 0) {
      hints.language_hint = Constants::getInstance().getLanguageFromName(input->languageHint.c_str());
    }

    if (input->encodingHint.length() > 0) {
      hints.encoding_hint = Constants::getInstance().getEncodingFromName(input->encodingHint.c_str());
    }

    if (input->tldHint.length() > 0) {
      hints.tld_hint = input->tldHint.c_str();
    }

    if (input->httpHint.length() > 0) {
      hints.content_language_hint = input->httpHint.c_str();
    }

    CLD2::ExtDetectLanguageSummary(
      input->bytes.c_str(),
      input->numBytes,
      input->isPlainText,
      &hints,
      0,
      output->language3,
      output->percent3,
      output->normalized_score3,
      &output->resultChunkVector,
      &output->textBytesFound,
      &output->isReliable
    );

    return output;
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
        mOutput = DetectLanguage(std::move(mInput));
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
    auto output = DetectLanguage(std::move(input));
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

  NODE_API_MODULE(cld, Init);
}
