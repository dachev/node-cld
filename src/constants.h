#ifndef NODE_CONSTANTS_H
#define NODE_CONSTANTS_H

#include "compact_lang_det.h"
#include "encodings.h"

using namespace std;

namespace NodeCld
{
  struct NodeCldLanguage {
    const char *name;
    const char *code;

	NodeCldLanguage(const char *name, const char *code) :
    name(name), code(code) {}
  };

  struct NodeCldDetected {
    const char *name;

	NodeCldDetected(const char *name) :
    name(name) {}
  };

  struct NodeCldEncoding {
    const char *name;
    CLD2::Encoding encoding;

	NodeCldEncoding(const char *name, CLD2::Encoding encoding) :
    name(name), encoding(encoding) {}
  };

  class Constants {
    public:
      static Constants& getInstance() {
        static Constants instance;
        
        return instance;
      }
      vector<NodeCldLanguage>* getLanguages();
      vector<NodeCldDetected>* getDetected();
      vector<NodeCldEncoding>* getEncodings();
      CLD2::Language getLanguageFromName(const char* name);
      int getEncodingFromName(const char* name);
      const char* getLanguageName(CLD2::Language lang);
      const char* getLanguageCode(CLD2::Language lang);

    private:
      vector<NodeCldLanguage> *languages;
      vector<NodeCldDetected> *detected;
      vector<NodeCldEncoding> *encodings;

      Constants();
      Constants(Constants const&);
      void operator=(Constants const&);

      void init();
      void initLanguages();
      void initDetected();
      void initEncodings();
  };
}

#endif
