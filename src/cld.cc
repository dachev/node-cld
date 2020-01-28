#include "compact_lang_det.h"
#include "encodings.h"
#include "constants.h"

using std::terminate_handler;
using std::unexpected_handler;

#include <napi.h>

namespace NodeCld {
  Napi::Object Detect(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    std::string text = info[0].ToString().Utf8Value();
    const char *bytes = text.c_str();
    int numBytes = text.length();
    bool isPlainText = info[1].ToBoolean();

    CLD2::CLDHints hints;
    hints.tld_hint = 0;
    hints.content_language_hint = 0;
    hints.language_hint = CLD2::UNKNOWN_LANGUAGE;
    hints.encoding_hint = CLD2::UNKNOWN_ENCODING;


    if (info[2].IsString()) {
      std::string languageHint = info[2].ToString().Utf8Value();
      if (languageHint.length() > 0) {
        hints.language_hint = Constants::getInstance().getLanguageFromName(languageHint.c_str());
      }
    }

    if (info[3].IsString()) {
      std::string encodingHint = info[3].ToString().Utf8Value();
      if (encodingHint.length() > 0) {
        hints.encoding_hint = Constants::getInstance().getEncodingFromName(encodingHint.c_str());
      }
    }

    if (info[4].IsString()) {
      std::string tldHint = info[4].ToString().Utf8Value();
      if (tldHint.length() > 0) {
        hints.tld_hint = tldHint.c_str();
      }
    }

    if (info[5].IsString()) {
      std::string httpHint = info[5].ToString().Utf8Value();
      if (httpHint.length() > 0) {
        hints.content_language_hint = httpHint.c_str();
      }
    }

    CLD2::Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    CLD2::ResultChunkVector resultChunkVector;
    int textBytesFound;
    bool isReliable;

    CLD2::ExtDetectLanguageSummary(
      bytes, numBytes,
      isPlainText,
      &hints,
      0,
      language3,
      percent3,
      normalized_score3,
      &resultChunkVector,
      &textBytesFound,
      &isReliable
    );

    size_t languageIdx = 0;
    auto languages = Napi::Array::New(env);
    for (size_t resultIdx = 0; resultIdx < 3; resultIdx++) {
      CLD2::Language lang = language3[resultIdx];

      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }

      auto item = Napi::Object::New(env);
      item["name"] = Napi::String::New(env, Constants::getInstance().getLanguageName(lang));
      item["code"] = Napi::String::New(env, Constants::getInstance().getLanguageCode(lang));
      item["percent"] = Napi::Number::New(env, percent3[resultIdx]);
      item["score"] = Napi::Number::New(env, normalized_score3[resultIdx]);

      languages[languageIdx++] = item;
    }

    size_t chunkIdx = 0;
    auto chunks = Napi::Array::New(env);
    for (size_t resultIdx = 0; resultIdx < resultChunkVector.size(); resultIdx++) {
      CLD2::ResultChunk chunk = resultChunkVector.at(resultIdx);
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
    results["reliable"] = Napi::Boolean::New(env, isReliable);
    results["textBytes"] = Napi::Number::New(env, textBytesFound);
    results["languages"] = languages;
    results["chunks"] = chunks;

    return results;
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

    return exports;
  }

  NODE_API_MODULE(cld, Init);
}
