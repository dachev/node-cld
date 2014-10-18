#include <cstring>
#include <vector>
#include "constants.h"

#ifdef _WIN32
  #include <string.h>
  #define strcasecmp _stricmp
#else
  #include <strings.h>
#endif

namespace CLD2 {
  extern const int kNameToLanguageSize;
  extern const CharIntPair kNameToLanguage[];
}

using namespace std;

namespace NodeCld
{
  Constants::Constants() {
    init();
  }

  vector<NodeCldLanguage>*
  Constants::getLanguages() {
    return languages;
  }

  vector<NodeCldDetected>*
  Constants::getDetected() {
    return detected;
  }

  vector<NodeCldEncoding>*
  Constants::getEncodings() {
    return encodings;
  }
  
  CLD2::Language
  Constants::getLanguageFromName(const char* name) {
    return CLD2::GetLanguageFromName(name);
  }

  int
  Constants::getEncodingFromName(const char* name) {
    vector<NodeCldEncoding>* encodings = Constants::getInstance().getEncodings();
    for(vector<NodeCldEncoding>::size_type i = 0; i < encodings->size(); i++) {
      NodeCldEncoding encoding = encodings->at(i);
      if (!strcasecmp(encoding.name, name)) {
        return encoding.encoding;
      }
    }

    return CLD2::UNKNOWN_ENCODING;
  }

  const char*
  Constants::getLanguageName(CLD2::Language lang) {
    return CLD2::LanguageName(lang);
  }

  const char*
  Constants::getLanguageCode(CLD2::Language lang) {
    return CLD2::LanguageCode(lang);
  }

  void
  Constants::init() {
    initLanguages();
    initDetected();
    initEncodings();
  }

  void
  Constants::initLanguages() {
    languages = new vector<NodeCldLanguage>();

    for(int i=0; i<CLD2::kNameToLanguageSize; i++) {
      const char *name = CLD2::kNameToLanguage[i].s;
      if (!strcmp(name, "Unknown")) {
        continue;
      }

      CLD2::Language lang = CLD2::GetLanguageFromName(name);
      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }

      languages->insert(languages->end(), NodeCldLanguage(LanguageName(lang), LanguageCode(lang)));
    }
  }

  void
  Constants::initDetected() {
    detected = new vector<NodeCldDetected>();
    
    detected->insert(detected->end(), NodeCldDetected("ABKHAZIAN"));
    detected->insert(detected->end(), NodeCldDetected("AFAR"));
    detected->insert(detected->end(), NodeCldDetected("AFRIKAANS"));
    detected->insert(detected->end(), NodeCldDetected("AKAN"));
    detected->insert(detected->end(), NodeCldDetected("ALBANIAN"));
    detected->insert(detected->end(), NodeCldDetected("AMHARIC"));
    detected->insert(detected->end(), NodeCldDetected("ARABIC"));
    detected->insert(detected->end(), NodeCldDetected("ARMENIAN"));
    detected->insert(detected->end(), NodeCldDetected("ASSAMESE"));
    detected->insert(detected->end(), NodeCldDetected("AYMARA"));
    detected->insert(detected->end(), NodeCldDetected("AZERBAIJANI"));
    detected->insert(detected->end(), NodeCldDetected("BASHKIR"));
    detected->insert(detected->end(), NodeCldDetected("BASQUE"));
    detected->insert(detected->end(), NodeCldDetected("BELARUSIAN"));
    detected->insert(detected->end(), NodeCldDetected("BENGALI"));
    detected->insert(detected->end(), NodeCldDetected("BIHARI"));
    detected->insert(detected->end(), NodeCldDetected("BISLAMA"));
    detected->insert(detected->end(), NodeCldDetected("BOSNIAN"));
    detected->insert(detected->end(), NodeCldDetected("BRETON"));
    detected->insert(detected->end(), NodeCldDetected("BULGARIAN"));
    detected->insert(detected->end(), NodeCldDetected("BURMESE"));
    detected->insert(detected->end(), NodeCldDetected("CATALAN"));
    detected->insert(detected->end(), NodeCldDetected("CEBUANO"));
    detected->insert(detected->end(), NodeCldDetected("CHEROKEE"));
    detected->insert(detected->end(), NodeCldDetected("CORSICAN"));
    detected->insert(detected->end(), NodeCldDetected("CROATIAN"));
    detected->insert(detected->end(), NodeCldDetected("CZECH"));
    detected->insert(detected->end(), NodeCldDetected("Chinese"));
    detected->insert(detected->end(), NodeCldDetected("ChineseT"));
    detected->insert(detected->end(), NodeCldDetected("DANISH"));
    detected->insert(detected->end(), NodeCldDetected("DHIVEHI"));
    detected->insert(detected->end(), NodeCldDetected("DUTCH"));
    detected->insert(detected->end(), NodeCldDetected("DZONGKHA"));
    detected->insert(detected->end(), NodeCldDetected("ENGLISH"));
    detected->insert(detected->end(), NodeCldDetected("ESPERANTO"));
    detected->insert(detected->end(), NodeCldDetected("ESTONIAN"));
    detected->insert(detected->end(), NodeCldDetected("FAROESE"));
    detected->insert(detected->end(), NodeCldDetected("FIJIAN"));
    detected->insert(detected->end(), NodeCldDetected("FINNISH"));
    detected->insert(detected->end(), NodeCldDetected("FRENCH"));
    detected->insert(detected->end(), NodeCldDetected("FRISIAN"));
    detected->insert(detected->end(), NodeCldDetected("GALICIAN"));
    detected->insert(detected->end(), NodeCldDetected("GANDA"));
    detected->insert(detected->end(), NodeCldDetected("GEORGIAN"));
    detected->insert(detected->end(), NodeCldDetected("GERMAN"));
    detected->insert(detected->end(), NodeCldDetected("GREEK"));
    detected->insert(detected->end(), NodeCldDetected("GREENLANDIC"));
    detected->insert(detected->end(), NodeCldDetected("GUARANI"));
    detected->insert(detected->end(), NodeCldDetected("GUJARATI"));
    detected->insert(detected->end(), NodeCldDetected("HAITIAN_CREOLE"));
    detected->insert(detected->end(), NodeCldDetected("HAUSA"));
    detected->insert(detected->end(), NodeCldDetected("HAWAIIAN"));
    detected->insert(detected->end(), NodeCldDetected("HEBREW"));
    detected->insert(detected->end(), NodeCldDetected("HINDI"));
    detected->insert(detected->end(), NodeCldDetected("HMONG"));
    detected->insert(detected->end(), NodeCldDetected("HUNGARIAN"));
    detected->insert(detected->end(), NodeCldDetected("ICELANDIC"));
    detected->insert(detected->end(), NodeCldDetected("IGBO"));
    detected->insert(detected->end(), NodeCldDetected("INDONESIAN"));
    detected->insert(detected->end(), NodeCldDetected("INTERLINGUA"));
    detected->insert(detected->end(), NodeCldDetected("INTERLINGUE"));
    detected->insert(detected->end(), NodeCldDetected("INUKTITUT"));
    detected->insert(detected->end(), NodeCldDetected("INUPIAK"));
    detected->insert(detected->end(), NodeCldDetected("IRISH"));
    detected->insert(detected->end(), NodeCldDetected("ITALIAN"));
    detected->insert(detected->end(), NodeCldDetected("JAVANESE"));
    detected->insert(detected->end(), NodeCldDetected("Japanese"));
    detected->insert(detected->end(), NodeCldDetected("KANNADA"));
    detected->insert(detected->end(), NodeCldDetected("KASHMIRI"));
    detected->insert(detected->end(), NodeCldDetected("KAZAKH"));
    detected->insert(detected->end(), NodeCldDetected("KHASI"));
    detected->insert(detected->end(), NodeCldDetected("KHMER"));
    detected->insert(detected->end(), NodeCldDetected("KINYARWANDA"));
    detected->insert(detected->end(), NodeCldDetected("KURDISH"));
    detected->insert(detected->end(), NodeCldDetected("KYRGYZ"));
    detected->insert(detected->end(), NodeCldDetected("Korean"));
    detected->insert(detected->end(), NodeCldDetected("LAOTHIAN"));
    detected->insert(detected->end(), NodeCldDetected("LATIN"));
    detected->insert(detected->end(), NodeCldDetected("LATVIAN"));
    detected->insert(detected->end(), NodeCldDetected("LIMBU"));
    detected->insert(detected->end(), NodeCldDetected("LINGALA"));
    detected->insert(detected->end(), NodeCldDetected("LITHUANIAN"));
    detected->insert(detected->end(), NodeCldDetected("LUXEMBOURGISH"));
    detected->insert(detected->end(), NodeCldDetected("MACEDONIAN"));
    detected->insert(detected->end(), NodeCldDetected("MALAGASY"));
    detected->insert(detected->end(), NodeCldDetected("MALAY"));
    detected->insert(detected->end(), NodeCldDetected("MALAYALAM"));
    detected->insert(detected->end(), NodeCldDetected("MALTESE"));
    detected->insert(detected->end(), NodeCldDetected("MANX"));
    detected->insert(detected->end(), NodeCldDetected("MAORI"));
    detected->insert(detected->end(), NodeCldDetected("MARATHI"));
    detected->insert(detected->end(), NodeCldDetected("MAURITIAN_CREOLE"));
    detected->insert(detected->end(), NodeCldDetected("MONGOLIAN"));
    detected->insert(detected->end(), NodeCldDetected("NAURU"));
    detected->insert(detected->end(), NodeCldDetected("NDEBELE"));
    detected->insert(detected->end(), NodeCldDetected("NEPALI"));
    detected->insert(detected->end(), NodeCldDetected("NORWEGIAN"));
    detected->insert(detected->end(), NodeCldDetected("NORWEGIAN_N"));
    detected->insert(detected->end(), NodeCldDetected("NYANJA"));
    detected->insert(detected->end(), NodeCldDetected("OCCITAN"));
    detected->insert(detected->end(), NodeCldDetected("ORIYA"));
    detected->insert(detected->end(), NodeCldDetected("OROMO"));
    detected->insert(detected->end(), NodeCldDetected("PASHTO"));
    detected->insert(detected->end(), NodeCldDetected("PEDI"));
    detected->insert(detected->end(), NodeCldDetected("PERSIAN"));
    detected->insert(detected->end(), NodeCldDetected("POLISH"));
    detected->insert(detected->end(), NodeCldDetected("PORTUGUESE"));
    detected->insert(detected->end(), NodeCldDetected("PUNJABI"));
    detected->insert(detected->end(), NodeCldDetected("QUECHUA"));
    detected->insert(detected->end(), NodeCldDetected("RHAETO_ROMANCE"));
    detected->insert(detected->end(), NodeCldDetected("ROMANIAN"));
    detected->insert(detected->end(), NodeCldDetected("RUNDI"));
    detected->insert(detected->end(), NodeCldDetected("RUSSIAN"));
    detected->insert(detected->end(), NodeCldDetected("SAMOAN"));
    detected->insert(detected->end(), NodeCldDetected("SANGO"));
    detected->insert(detected->end(), NodeCldDetected("SANSKRIT"));
    detected->insert(detected->end(), NodeCldDetected("SCOTS"));
    detected->insert(detected->end(), NodeCldDetected("SCOTS_GAELIC"));
    detected->insert(detected->end(), NodeCldDetected("SERBIAN"));
    detected->insert(detected->end(), NodeCldDetected("SESELWA"));
    detected->insert(detected->end(), NodeCldDetected("SESOTHO"));
    detected->insert(detected->end(), NodeCldDetected("SHONA"));
    detected->insert(detected->end(), NodeCldDetected("SINDHI"));
    detected->insert(detected->end(), NodeCldDetected("SINHALESE"));
    detected->insert(detected->end(), NodeCldDetected("SISWANT"));
    detected->insert(detected->end(), NodeCldDetected("SLOVAK"));
    detected->insert(detected->end(), NodeCldDetected("SLOVENIAN"));
    detected->insert(detected->end(), NodeCldDetected("SOMALI"));
    detected->insert(detected->end(), NodeCldDetected("SPANISH"));
    detected->insert(detected->end(), NodeCldDetected("SUNDANESE"));
    detected->insert(detected->end(), NodeCldDetected("SWAHILI"));
    detected->insert(detected->end(), NodeCldDetected("SWEDISH"));
    detected->insert(detected->end(), NodeCldDetected("SYRIAC"));
    detected->insert(detected->end(), NodeCldDetected("TAGALOG"));
    detected->insert(detected->end(), NodeCldDetected("TAJIK"));
    detected->insert(detected->end(), NodeCldDetected("TAMIL"));
    detected->insert(detected->end(), NodeCldDetected("TATAR"));
    detected->insert(detected->end(), NodeCldDetected("TELUGU"));
    detected->insert(detected->end(), NodeCldDetected("THAI"));
    detected->insert(detected->end(), NodeCldDetected("TIBETAN"));
    detected->insert(detected->end(), NodeCldDetected("TIGRINYA"));
    detected->insert(detected->end(), NodeCldDetected("TONGA"));
    detected->insert(detected->end(), NodeCldDetected("TSONGA"));
    detected->insert(detected->end(), NodeCldDetected("TSWANA"));
    detected->insert(detected->end(), NodeCldDetected("TURKISH"));
    detected->insert(detected->end(), NodeCldDetected("TURKMEN"));
    detected->insert(detected->end(), NodeCldDetected("UIGHUR"));
    detected->insert(detected->end(), NodeCldDetected("UKRAINIAN"));
    detected->insert(detected->end(), NodeCldDetected("URDU"));
    detected->insert(detected->end(), NodeCldDetected("UZBEK"));
    detected->insert(detected->end(), NodeCldDetected("VENDA"));
    detected->insert(detected->end(), NodeCldDetected("VIETNAMESE"));
    detected->insert(detected->end(), NodeCldDetected("VOLAPUK"));
    detected->insert(detected->end(), NodeCldDetected("WARAY_PHILIPPINES"));
    detected->insert(detected->end(), NodeCldDetected("WELSH"));
    detected->insert(detected->end(), NodeCldDetected("WOLOF"));
    detected->insert(detected->end(), NodeCldDetected("XHOSA"));
    detected->insert(detected->end(), NodeCldDetected("X_Buginese"));
    detected->insert(detected->end(), NodeCldDetected("X_Gothic"));
    detected->insert(detected->end(), NodeCldDetected("X_KLINGON"));
    detected->insert(detected->end(), NodeCldDetected("X_PIG_LATIN"));
    detected->insert(detected->end(), NodeCldDetected("YIDDISH"));
    detected->insert(detected->end(), NodeCldDetected("YORUBA"));
    detected->insert(detected->end(), NodeCldDetected("ZHUANG"));
    detected->insert(detected->end(), NodeCldDetected("ZULU"));
  }

  void
  Constants::initEncodings() {
    encodings = new vector<NodeCldEncoding>();

    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_1", CLD2::ISO_8859_1));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_2", CLD2::ISO_8859_2));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_3", CLD2::ISO_8859_3));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_4", CLD2::ISO_8859_4));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_5", CLD2::ISO_8859_5));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_6", CLD2::ISO_8859_6));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_7", CLD2::ISO_8859_7));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_8", CLD2::ISO_8859_8));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_9", CLD2::ISO_8859_9));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_10", CLD2::ISO_8859_10));
    encodings->insert(encodings->end(), NodeCldEncoding("JAPANESE_EUC_JP", CLD2::JAPANESE_EUC_JP));
    encodings->insert(encodings->end(), NodeCldEncoding("JAPANESE_SHIFT_JIS", CLD2::JAPANESE_SHIFT_JIS));
    encodings->insert(encodings->end(), NodeCldEncoding("JAPANESE_JIS", CLD2::JAPANESE_JIS));
    encodings->insert(encodings->end(), NodeCldEncoding("CHINESE_BIG5", CLD2::CHINESE_BIG5));
    encodings->insert(encodings->end(), NodeCldEncoding("CHINESE_GB", CLD2::CHINESE_GB));
    encodings->insert(encodings->end(), NodeCldEncoding("CHINESE_EUC_CN", CLD2::CHINESE_EUC_CN));
    encodings->insert(encodings->end(), NodeCldEncoding("KOREAN_EUC_KR", CLD2::KOREAN_EUC_KR));
    encodings->insert(encodings->end(), NodeCldEncoding("UNICODE_UNUSED", CLD2::UNICODE_UNUSED));
    encodings->insert(encodings->end(), NodeCldEncoding("CHINESE_EUC_DEC", CLD2::CHINESE_EUC_DEC));
    encodings->insert(encodings->end(), NodeCldEncoding("CHINESE_CNS", CLD2::CHINESE_CNS));
    encodings->insert(encodings->end(), NodeCldEncoding("CHINESE_BIG5_CP950", CLD2::CHINESE_BIG5_CP950));
    encodings->insert(encodings->end(), NodeCldEncoding("JAPANESE_CP932", CLD2::JAPANESE_CP932));
    encodings->insert(encodings->end(), NodeCldEncoding("UTF8", CLD2::UTF8));
    encodings->insert(encodings->end(), NodeCldEncoding("ASCII_7BIT", CLD2::ASCII_7BIT));
    encodings->insert(encodings->end(), NodeCldEncoding("RUSSIAN_KOI8_R", CLD2::RUSSIAN_KOI8_R));
    encodings->insert(encodings->end(), NodeCldEncoding("RUSSIAN_CP1251", CLD2::RUSSIAN_CP1251));
    encodings->insert(encodings->end(), NodeCldEncoding("MSFT_CP1252", CLD2::MSFT_CP1252));
    encodings->insert(encodings->end(), NodeCldEncoding("RUSSIAN_KOI8_RU", CLD2::RUSSIAN_KOI8_RU));
    encodings->insert(encodings->end(), NodeCldEncoding("MSFT_CP1250", CLD2::MSFT_CP1250));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_15", CLD2::ISO_8859_15));
    encodings->insert(encodings->end(), NodeCldEncoding("MSFT_CP1254", CLD2::MSFT_CP1254));
    encodings->insert(encodings->end(), NodeCldEncoding("MSFT_CP1257", CLD2::MSFT_CP1257));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_11", CLD2::ISO_8859_11));
    encodings->insert(encodings->end(), NodeCldEncoding("MSFT_CP874", CLD2::MSFT_CP874));
    encodings->insert(encodings->end(), NodeCldEncoding("MSFT_CP1256", CLD2::MSFT_CP1256));
    encodings->insert(encodings->end(), NodeCldEncoding("MSFT_CP1255", CLD2::MSFT_CP1255));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_8_I", CLD2::ISO_8859_8_I));
    encodings->insert(encodings->end(), NodeCldEncoding("HEBREW_VISUAL", CLD2::HEBREW_VISUAL));
    encodings->insert(encodings->end(), NodeCldEncoding("CZECH_CP852", CLD2::CZECH_CP852));
    encodings->insert(encodings->end(), NodeCldEncoding("CZECH_CSN_369103", CLD2::CZECH_CSN_369103));
    encodings->insert(encodings->end(), NodeCldEncoding("MSFT_CP1253", CLD2::MSFT_CP1253));
    encodings->insert(encodings->end(), NodeCldEncoding("RUSSIAN_CP866", CLD2::RUSSIAN_CP866));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_8859_13", CLD2::ISO_8859_13));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_2022_KR", CLD2::ISO_2022_KR));
    encodings->insert(encodings->end(), NodeCldEncoding("GBK", CLD2::GBK));
    encodings->insert(encodings->end(), NodeCldEncoding("GB18030", CLD2::GB18030));
    encodings->insert(encodings->end(), NodeCldEncoding("BIG5_HKSCS", CLD2::BIG5_HKSCS));
    encodings->insert(encodings->end(), NodeCldEncoding("ISO_2022_CN", CLD2::ISO_2022_CN));
    encodings->insert(encodings->end(), NodeCldEncoding("TSCII", CLD2::TSCII));
    encodings->insert(encodings->end(), NodeCldEncoding("TAMIL_MONO", CLD2::TAMIL_MONO));
    encodings->insert(encodings->end(), NodeCldEncoding("TAMIL_BI", CLD2::TAMIL_BI));
    encodings->insert(encodings->end(), NodeCldEncoding("JAGRAN", CLD2::JAGRAN));
    encodings->insert(encodings->end(), NodeCldEncoding("MACINTOSH_ROMAN", CLD2::MACINTOSH_ROMAN));
    encodings->insert(encodings->end(), NodeCldEncoding("UTF7", CLD2::UTF7));
    encodings->insert(encodings->end(), NodeCldEncoding("BHASKAR", CLD2::BHASKAR));
    encodings->insert(encodings->end(), NodeCldEncoding("HTCHANAKYA", CLD2::HTCHANAKYA));
    encodings->insert(encodings->end(), NodeCldEncoding("UTF16BE", CLD2::UTF16BE));
    encodings->insert(encodings->end(), NodeCldEncoding("UTF16LE", CLD2::UTF16LE));
    encodings->insert(encodings->end(), NodeCldEncoding("UTF32BE", CLD2::UTF32BE));
    encodings->insert(encodings->end(), NodeCldEncoding("UTF32LE", CLD2::UTF32LE));
    encodings->insert(encodings->end(), NodeCldEncoding("BINARYENC", CLD2::BINARYENC));
    encodings->insert(encodings->end(), NodeCldEncoding("HZ_GB_2312", CLD2::HZ_GB_2312));
    encodings->insert(encodings->end(), NodeCldEncoding("UTF8UTF8", CLD2::UTF8UTF8));
    encodings->insert(encodings->end(), NodeCldEncoding("TAM_ELANGO", CLD2::TAM_ELANGO));
    encodings->insert(encodings->end(), NodeCldEncoding("TAM_LTTMBARANI", CLD2::TAM_LTTMBARANI));
    encodings->insert(encodings->end(), NodeCldEncoding("TAM_SHREE", CLD2::TAM_SHREE));
    encodings->insert(encodings->end(), NodeCldEncoding("TAM_TBOOMIS", CLD2::TAM_TBOOMIS));
    encodings->insert(encodings->end(), NodeCldEncoding("TAM_TMNEWS", CLD2::TAM_TMNEWS));
    encodings->insert(encodings->end(), NodeCldEncoding("TAM_WEBTAMIL", CLD2::TAM_WEBTAMIL));
    encodings->insert(encodings->end(), NodeCldEncoding("KDDI_SHIFT_JIS", CLD2::KDDI_SHIFT_JIS));
    encodings->insert(encodings->end(), NodeCldEncoding("DOCOMO_SHIFT_JIS", CLD2::DOCOMO_SHIFT_JIS));
    encodings->insert(encodings->end(), NodeCldEncoding("SOFTBANK_SHIFT_JIS", CLD2::SOFTBANK_SHIFT_JIS));
    encodings->insert(encodings->end(), NodeCldEncoding("KDDI_ISO_2022_JP", CLD2::KDDI_ISO_2022_JP));
    encodings->insert(encodings->end(), NodeCldEncoding("SOFTBANK_ISO_2022_JP", CLD2::SOFTBANK_ISO_2022_JP));
  }
}
