#include "compact_lang_det.h"
#include "encodings.h"
#include "constants.h"
#include "nan.h"

namespace NodeCld {

  NAN_METHOD(Detect) {
    NanScope();
    
    v8::Local<v8::Object> results = NanNew<v8::Object>();
    v8::String::Utf8Value text(args[0]->ToString());

    char *bytes      = *text;
    int numBytes     = text.length();
    bool isPlainText = args[1]->ToBoolean()->Value();

    CLD2::CLDHints hints;
    hints.tld_hint = 0;
    hints.content_language_hint = 0;
    hints.language_hint = CLD2::UNKNOWN_LANGUAGE;
    hints.encoding_hint = CLD2::UNKNOWN_ENCODING;

    v8::String::Utf8Value languageHint(args[2]->ToString());
    v8::String::Utf8Value encodingHint(args[3]->ToString());
    v8::String::Utf8Value tldHint(args[4]->ToString());
    v8::String::Utf8Value httpHint(args[5]->ToString());

    if (tldHint.length() > 0) {
      hints.tld_hint = *tldHint;
    }
    if (httpHint.length() > 0) {
      hints.content_language_hint = *httpHint;
    }
    if (languageHint.length() > 0) {
      hints.language_hint = Constants::getInstance().getLanguageFromName(*languageHint);
    }
    if (encodingHint.length() > 0) {
      hints.encoding_hint = Constants::getInstance().getEncodingFromName(*encodingHint);
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

    v8::Local<v8::Array> languages = v8::Local<v8::Array>(NanNew<v8::Array>());
    for(int idx = 0; idx < 3; idx++) {
      CLD2::Language lang = language3[idx];
      
      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }
      
      v8::Local<v8::Object> item = NanNew<v8::Object>();
      item->Set(NanNew<v8::String>("name"), NanNew<v8::String>(Constants::getInstance().getLanguageName(lang)));
      item->Set(NanNew<v8::String>("code"), NanNew<v8::String>(Constants::getInstance().getLanguageCode(lang)));
      item->Set(NanNew<v8::String>("percent"), NanNew<v8::Number>(percent3[idx]));
      item->Set(NanNew<v8::String>("score"), NanNew<v8::Number>(normalized_score3[idx]));
      
      languages->Set(NanNew<v8::Integer>(idx), item);
    }

    unsigned int chunkIdx = 0;
    v8::Local<v8::Array> chunks = v8::Local<v8::Array>(NanNew<v8::Array>());
    for(unsigned int resultIdx = 0; resultIdx < resultChunkVector.size(); resultIdx++) {
      CLD2::ResultChunk chunk = resultChunkVector.at(resultIdx);
      CLD2::Language lang = static_cast<CLD2::Language>(chunk.lang1);

      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }

      v8::Local<v8::Object> item = NanNew<v8::Object>();
      item->Set(NanNew<v8::String>("name"), NanNew<v8::String>(Constants::getInstance().getLanguageName(lang)));
      item->Set(NanNew<v8::String>("code"), NanNew<v8::String>(Constants::getInstance().getLanguageCode(lang)));
      item->Set(NanNew<v8::String>("offset"), NanNew<v8::Number>(chunk.offset));
      item->Set(NanNew<v8::String>("bytes"), NanNew<v8::Number>(chunk.bytes));

      chunks->Set(NanNew<v8::Integer>(chunkIdx), item);
      chunkIdx++;
    }

    results->Set(NanNew<v8::String>("reliable"), NanNew<v8::Boolean>(isReliable));
    results->Set(NanNew<v8::String>("textBytes"), NanNew<v8::Number>(textBytesFound));
    results->Set(NanNew<v8::String>("languages"), languages);
    results->Set(NanNew<v8::String>("chunks"), chunks);
    
    NanReturnValue(results);
  }

  extern "C" void init (v8::Handle<v8::Object> target) {
    NanScope();

    // set detected languages
    v8::Local<v8::Array> detected = NanNew<v8::Array>();
    vector<NodeCldDetected>* rawDetected = Constants::getInstance().getDetected();
    for(vector<NodeCldDetected>::size_type i = 0; i < rawDetected->size(); i++) {
      NodeCldDetected rawLanguage = rawDetected->at(i);
      detected->Set(static_cast<uint32_t>(i), NanNew<v8::String>(rawLanguage.name));
    }
    target->Set(NanNew<v8::String>("DETECTED_LANGUAGES"), detected);

    // set all languages
    v8::Local<v8::Object> languages = NanNew<v8::Object>();
    vector<NodeCldLanguage>* rawLanguages = Constants::getInstance().getLanguages();
    for(vector<NodeCldLanguage>::size_type i = 0; i < rawLanguages->size(); i++) {
      NodeCldLanguage rawLanguage = rawLanguages->at(i);
      languages->Set(NanNew<v8::String>(rawLanguage.name), NanNew<v8::String>(rawLanguage.code));
    }
    target->Set(NanNew<v8::String>("LANGUAGES"), languages);

    // set encodings
    v8::Local<v8::Array> encodings = NanNew<v8::Array>();
    vector<NodeCldEncoding>* rawEncodings = Constants::getInstance().getEncodings();
    for(vector<NodeCldEncoding>::size_type i = 0; i < rawEncodings->size(); i++) {
      NodeCldEncoding rawEncoding = rawEncodings->at(i);
      encodings->Set(static_cast<uint32_t>(i), NanNew<v8::String>(rawEncoding.name));
    }
    target->Set(NanNew<v8::String>("ENCODINGS"), encodings);

    NODE_SET_METHOD(target, "detect", Detect);
  }

  NODE_MODULE(cld, init);
}
