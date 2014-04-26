#include "compact_lang_det.h"
#include "encodings.h"
#include "constants.h"
#include "cld.h"

namespace NodeCld {
  static v8::Persistent<v8::FunctionTemplate> detect;
  static v8::Persistent<v8::Array> detected;
  static v8::Persistent<v8::Object> languages;
  static v8::Persistent<v8::Array> encodings;
  
  void
  Cld::Init(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;
    
    detected  = v8::Persistent<v8::Array>(v8::Array::New());
    languages = v8::Persistent<v8::Object>(v8::Object::New());
    encodings = v8::Persistent<v8::Array>(v8::Array::New());

    // set detected languages
    vector<NodeCldDetected>* rawDetected = Constants::getInstance().getDetected();
    for(vector<NodeCldDetected>::size_type i = 0; i < rawDetected->size(); i++) {
      NodeCldDetected rawLanguage = rawDetected->at(i);
      
      detected->Set(v8::Integer::New(i), v8::String::New(rawLanguage.name));
    }
    target->Set(v8::String::NewSymbol("DETECTED_LANGUAGES"), detected);

    // set all languages
    vector<NodeCldLanguage>* rawLanguages = Constants::getInstance().getLanguages();
    for(vector<NodeCldLanguage>::size_type i = 0; i < rawLanguages->size(); i++) {
      NodeCldLanguage rawLanguage = rawLanguages->at(i);
      
      languages->Set(v8::String::New(rawLanguage.name), v8::String::New(rawLanguage.code));
    }
    target->Set(v8::String::NewSymbol("LANGUAGES"), languages);
    
    // set encodings
    vector<NodeCldEncoding>* rawEncodings = Constants::getInstance().getEncodings();
    for(vector<NodeCldEncoding>::size_type i = 0; i < rawEncodings->size(); i++) {
      NodeCldEncoding rawEncoding = rawEncodings->at(i);
      
      encodings->Set(v8::Integer::New(i), v8::String::New(rawEncoding.name));
    }
    target->Set(v8::String::NewSymbol("ENCODINGS"), encodings);
    
    v8::Local<v8::FunctionTemplate> func = v8::FunctionTemplate::New(Detect);
    detect = v8::Persistent<v8::FunctionTemplate>::New(func);
    target->Set(v8::String::NewSymbol("detect"), detect->GetFunction());
  }
  
  v8::Handle<v8::Value>
  Cld::Detect(const v8::Arguments& args) {
    v8::HandleScope scope;
    
    v8::Local<v8::Object> results = v8::Object::New();
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

    v8::Local<v8::Array> languages = v8::Local<v8::Array>(v8::Array::New());
    for(int idx = 0; idx < 3; idx++) {
      CLD2::Language lang = language3[idx];
      
      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }
      
      v8::Local<v8::Object> item = v8::Object::New();
      item->Set(v8::String::New("name"), v8::String::New(Constants::getInstance().getLanguageName(lang)));
      item->Set(v8::String::New("code"), v8::String::New(Constants::getInstance().getLanguageCode(lang)));
      item->Set(v8::String::New("percent"), v8::Number::New(percent3[idx]));
      item->Set(v8::String::New("score"), v8::Number::New(normalized_score3[idx]));
      
      languages->Set(v8::Integer::New(idx), item);
    }

    v8::Local<v8::Array> chunks = v8::Local<v8::Array>(v8::Array::New());
    for(unsigned int idx = 0; idx < resultChunkVector.size(); idx++) {
      CLD2::ResultChunk chunk = resultChunkVector.at(idx);
      CLD2::Language lang = static_cast<CLD2::Language>(chunk.lang1);

      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }

      v8::Local<v8::Object> item = v8::Object::New();
      item->Set(v8::String::New("name"), v8::String::New(Constants::getInstance().getLanguageName(lang)));
      item->Set(v8::String::New("code"), v8::String::New(Constants::getInstance().getLanguageCode(lang)));
      item->Set(v8::String::New("offset"), v8::Number::New(chunk.offset));
      item->Set(v8::String::New("bytes"), v8::Number::New(chunk.bytes));

      chunks->Set(v8::Integer::New(idx), item);
    }

    results->Set(v8::String::New("reliable"), v8::Boolean::New(isReliable));
    results->Set(v8::String::New("textBytes"), v8::Number::New(textBytesFound));
    results->Set(v8::String::New("languages"), languages);
    results->Set(v8::String::New("chunks"), chunks);
    
    return scope.Close(results);
  }

  extern "C" void init (v8::Handle<v8::Object> target) {
    Cld::Init(target);
  }

  NODE_MODULE(cld, init);
}

