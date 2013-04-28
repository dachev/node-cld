#include "chromium/encodings/compact_lang_det/compact_lang_det.h"
#include "chromium/encodings/compact_lang_det/ext_lang_enc.h"
#include "chromium/base/string_util.h"
#include "chromium/cld_encodings.h"
#include "cld.h"

namespace cld {
  static v8::Persistent<v8::FunctionTemplate> detect;
  static v8::Persistent<v8::Array> encodings;
  static v8::Persistent<v8::Array> baseLanguages;
  static v8::Persistent<v8::Array> extraLanguages;
  static v8::Persistent<v8::Array> detectedLanguages;
  
  void
  Cld::Init(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;
  
    v8::Local<v8::FunctionTemplate> func = v8::FunctionTemplate::New(Detect);
    detect = v8::Persistent<v8::FunctionTemplate>::New(func);
    target->Set(v8::String::NewSymbol("detect"), detect->GetFunction());
    
    // set encodings
    encodings = v8::Persistent<v8::Array>(v8::Array::New());
    for(int i = 0; i < NUM_ENCODINGS; i++) {
      const char *name = cld_encoding_info[i].name;
      
      encodings->Set(v8::Integer::New(i), v8::String::New(name));
    }
    target->Set(v8::String::NewSymbol("ENCODINGS"), encodings);
    
    // set all languages
    baseLanguages = v8::Persistent<v8::Array>(v8::Array::New());
    for(int i = 0; i < NUM_LANGUAGES; i++) {
      const char *name = LanguageName((Language) i);
      
      baseLanguages->Set(v8::Integer::New(i), v8::String::New(name));
    }
    target->Set(v8::String::NewSymbol("LANGUAGES"), baseLanguages);
    
    // set extra languages
    extraLanguages = v8::Persistent<v8::Array>(v8::Array::New());
    for(int i = EXT_LANGUAGE_BASE; i < EXT_NUM_LANGUAGES; i++) {
      const char *name = ExtLanguageName((Language) i);
      
      extraLanguages->Set(v8::Integer::New(i-EXT_LANGUAGE_BASE), v8::String::New(name));
    }
    target->Set(v8::String::NewSymbol("EXTRA_LANGUAGES"), extraLanguages);
    
    // set detected languages
    detectedLanguages = v8::Persistent<v8::Array>(v8::Array::New());
    detectedLanguages->Set(v8::Integer::New(0), v8::String::New("AFRIKAANS"));
    detectedLanguages->Set(v8::Integer::New(1), v8::String::New("ALBANIAN"));
    detectedLanguages->Set(v8::Integer::New(2), v8::String::New("AMHARIC"));
    detectedLanguages->Set(v8::Integer::New(3), v8::String::New("ARABIC"));
    detectedLanguages->Set(v8::Integer::New(4), v8::String::New("ARMENIAN"));
    detectedLanguages->Set(v8::Integer::New(5), v8::String::New("AZERBAIJANI"));
    detectedLanguages->Set(v8::Integer::New(6), v8::String::New("BASQUE"));
    detectedLanguages->Set(v8::Integer::New(7), v8::String::New("BELARUSIAN"));
    detectedLanguages->Set(v8::Integer::New(8), v8::String::New("BENGALI"));
    detectedLanguages->Set(v8::Integer::New(9), v8::String::New("BULGARIAN"));
    detectedLanguages->Set(v8::Integer::New(10), v8::String::New("BURMESE"));
    detectedLanguages->Set(v8::Integer::New(11), v8::String::New("CATALAN"));
    detectedLanguages->Set(v8::Integer::New(12), v8::String::New("CHEROKEE"));
    detectedLanguages->Set(v8::Integer::New(13), v8::String::New("CROATIAN"));
    detectedLanguages->Set(v8::Integer::New(14), v8::String::New("CZECH"));
    detectedLanguages->Set(v8::Integer::New(15), v8::String::New("Chinese"));
    detectedLanguages->Set(v8::Integer::New(16), v8::String::New("ChineseT"));
    detectedLanguages->Set(v8::Integer::New(17), v8::String::New("DANISH"));
    detectedLanguages->Set(v8::Integer::New(18), v8::String::New("DHIVEHI"));
    detectedLanguages->Set(v8::Integer::New(19), v8::String::New("DUTCH"));
    detectedLanguages->Set(v8::Integer::New(20), v8::String::New("ENGLISH"));
    detectedLanguages->Set(v8::Integer::New(21), v8::String::New("ESTONIAN"));
    detectedLanguages->Set(v8::Integer::New(22), v8::String::New("FINNISH"));
    detectedLanguages->Set(v8::Integer::New(23), v8::String::New("FRENCH"));
    detectedLanguages->Set(v8::Integer::New(24), v8::String::New("GALICIAN"));
    detectedLanguages->Set(v8::Integer::New(25), v8::String::New("GEORGIAN"));
    detectedLanguages->Set(v8::Integer::New(26), v8::String::New("GERMAN"));
    detectedLanguages->Set(v8::Integer::New(27), v8::String::New("GREEK"));
    detectedLanguages->Set(v8::Integer::New(28), v8::String::New("GUJARATI"));
    detectedLanguages->Set(v8::Integer::New(29), v8::String::New("HAITIAN_CREOLE"));
    detectedLanguages->Set(v8::Integer::New(30), v8::String::New("HEBREW"));
    detectedLanguages->Set(v8::Integer::New(31), v8::String::New("HINDI"));
    detectedLanguages->Set(v8::Integer::New(32), v8::String::New("HUNGARIAN"));
    detectedLanguages->Set(v8::Integer::New(33), v8::String::New("ICELANDIC"));
    detectedLanguages->Set(v8::Integer::New(34), v8::String::New("INDONESIAN"));
    detectedLanguages->Set(v8::Integer::New(35), v8::String::New("INUKTITUT"));
    detectedLanguages->Set(v8::Integer::New(36), v8::String::New("IRISH"));
    detectedLanguages->Set(v8::Integer::New(37), v8::String::New("ITALIAN"));
    detectedLanguages->Set(v8::Integer::New(38), v8::String::New("Japanese"));
    detectedLanguages->Set(v8::Integer::New(39), v8::String::New("KANNADA"));
    detectedLanguages->Set(v8::Integer::New(40), v8::String::New("KHMER"));
    detectedLanguages->Set(v8::Integer::New(41), v8::String::New("Korean"));
    detectedLanguages->Set(v8::Integer::New(42), v8::String::New("LAOTHIAN"));
    detectedLanguages->Set(v8::Integer::New(43), v8::String::New("LATVIAN"));
    detectedLanguages->Set(v8::Integer::New(44), v8::String::New("LITHUANIAN"));
    detectedLanguages->Set(v8::Integer::New(45), v8::String::New("MACEDONIAN"));
    detectedLanguages->Set(v8::Integer::New(46), v8::String::New("MALAY"));
    detectedLanguages->Set(v8::Integer::New(47), v8::String::New("MALAYALAM"));
    detectedLanguages->Set(v8::Integer::New(48), v8::String::New("MALTESE"));
    detectedLanguages->Set(v8::Integer::New(49), v8::String::New("NORWEGIAN"));
    detectedLanguages->Set(v8::Integer::New(50), v8::String::New("ORIYA"));
    detectedLanguages->Set(v8::Integer::New(51), v8::String::New("PERSIAN"));
    detectedLanguages->Set(v8::Integer::New(52), v8::String::New("POLISH"));
    detectedLanguages->Set(v8::Integer::New(53), v8::String::New("PORTUGUESE"));
    detectedLanguages->Set(v8::Integer::New(54), v8::String::New("PUNJABI"));
    detectedLanguages->Set(v8::Integer::New(55), v8::String::New("ROMANIAN"));
    detectedLanguages->Set(v8::Integer::New(56), v8::String::New("RUSSIAN"));
    detectedLanguages->Set(v8::Integer::New(57), v8::String::New("SERBIAN"));
    detectedLanguages->Set(v8::Integer::New(58), v8::String::New("SINHALESE"));
    detectedLanguages->Set(v8::Integer::New(59), v8::String::New("SLOVAK"));
    detectedLanguages->Set(v8::Integer::New(60), v8::String::New("SLOVENIAN"));
    detectedLanguages->Set(v8::Integer::New(61), v8::String::New("SPANISH"));
    detectedLanguages->Set(v8::Integer::New(62), v8::String::New("SWAHILI"));
    detectedLanguages->Set(v8::Integer::New(63), v8::String::New("SWEDISH"));
    detectedLanguages->Set(v8::Integer::New(64), v8::String::New("SYRIAC"));
    detectedLanguages->Set(v8::Integer::New(65), v8::String::New("TAGALOG"));
    detectedLanguages->Set(v8::Integer::New(66), v8::String::New("TAMIL"));
    detectedLanguages->Set(v8::Integer::New(67), v8::String::New("TELUGU"));
    detectedLanguages->Set(v8::Integer::New(68), v8::String::New("THAI"));
    detectedLanguages->Set(v8::Integer::New(69), v8::String::New("TIBETAN"));
    detectedLanguages->Set(v8::Integer::New(70), v8::String::New("TURKISH"));
    detectedLanguages->Set(v8::Integer::New(71), v8::String::New("UKRAINIAN"));
    detectedLanguages->Set(v8::Integer::New(72), v8::String::New("URDU"));
    detectedLanguages->Set(v8::Integer::New(73), v8::String::New("VIETNAMESE"));
    detectedLanguages->Set(v8::Integer::New(74), v8::String::New("WELSH"));
    detectedLanguages->Set(v8::Integer::New(75), v8::String::New("YIDDISH"));
    
    target->Set(v8::String::NewSymbol("DETECTED_LANGUAGES"), detectedLanguages);
  }
  
  v8::Handle<v8::Value>
  Cld::Detect(const v8::Arguments& args) {
    v8::HandleScope scope;
    
    v8::Local<v8::Object> results = v8::Object::New();
    results->Set(v8::String::New("name"), v8::String::New(ExtLanguageName(UNKNOWN_LANGUAGE)));
    results->Set(v8::String::New("code"), v8::String::New(ExtLanguageCode(UNKNOWN_LANGUAGE)));
    results->Set(v8::String::New("reliable"), v8::Boolean::New(false));
    results->Set(v8::String::New("details"), v8::Array::New());
    
    if (args.Length() < 1 || args[0]->IsString() == false) {
      return scope.Close(results);
    }
    v8::String::Utf8Value text(args[0]->ToString());
    
    char *bytes                   = *text;
    int numBytes                  = text.length();
    bool isPlainText              = true;
    bool pickSummaryLanguage      = false;
    bool removeWeakMatches        = true;
    bool includeExtendedLanguages = true;
    
    const char* hintTopLevelDomain = NULL; // "id" boosts Indonesian;
    const char* hintLanguageCode   = NULL; // ITALIAN boosts it
    const char* hintEncoding       = NULL; // SJS boosts Japanese
    
    Encoding hintEncodingEnum = UNKNOWN_ENCODING;
    Language hintLanguageEnum = UNKNOWN_LANGUAGE;
    Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    int textBytesFound;
    bool isReliable;
    
    if (args.Length() > 1 && args[1]->IsObject()) {
      // TODO: allow passing options
    }
    
    Language sumLang = CompactLangDet::DetectLanguage(0,
                         bytes, numBytes,
                         isPlainText,
                         includeExtendedLanguages,
                         pickSummaryLanguage,
                         removeWeakMatches,
                         hintTopLevelDomain,
                         hintEncodingEnum,
                         hintLanguageEnum,
                         language3,
                         percent3,
                         normalized_score3,
                         &textBytesFound,
                         &isReliable);
    
    v8::Local<v8::Array> details = v8::Local<v8::Array>(v8::Array::New());
    for(int idx = 0; idx < 3; idx++) {
      Language lang = language3[idx];
      if (lang == UNKNOWN_LANGUAGE) {
        break;
      }
      
      v8::Local<v8::Object> detail = v8::Object::New();
      detail->Set(v8::String::New("name"), v8::String::New(ExtLanguageName(lang)));
      detail->Set(v8::String::New("code"), v8::String::New(ExtLanguageCode(lang)));
      detail->Set(v8::String::New("percent"), v8::Number::New(percent3[idx]));
      detail->Set(v8::String::New("score"), v8::Number::New(normalized_score3[idx]));
      
      details->Set(v8::Integer::New(idx), detail);
    }
    
    results->Set(v8::String::New("name"), v8::String::New(ExtLanguageName(sumLang)));
    results->Set(v8::String::New("code"), v8::String::New(ExtLanguageCode(sumLang)));
    results->Set(v8::String::New("reliable"), v8::Boolean::New(isReliable));
    results->Set(v8::String::New("details"), details);
    
    return scope.Close(results);
 }

  extern "C" void init (v8::Handle<v8::Object> target) {
    Cld::Init(target);
  }

  NODE_MODULE(cld, init);
}

