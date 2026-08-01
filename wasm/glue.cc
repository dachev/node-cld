#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

#include <emscripten.h>

#include "cld_core.h"
#include "constants.h"

namespace {
  // Only CLD2's own ASCII language/encoding names ever end up as string
  // values here (the detected input text itself is never echoed back), but
  // escape defensively anyway since this is a JSON boundary.
  void appendEscapedJSON(std::ostringstream &out, const char *s) {
    out << '"';
    for (const char *p = s; *p; ++p) {
      unsigned char c = static_cast<unsigned char>(*p);
      switch (c) {
        case '"':  out << "\\\""; break;
        case '\\': out << "\\\\"; break;
        case '\n': out << "\\n"; break;
        case '\r': out << "\\r"; break;
        case '\t': out << "\\t"; break;
        default:
          if (c < 0x20) {
            char buf[8];
            snprintf(buf, sizeof(buf), "\\u%04x", c);
            out << buf;
          } else {
            out << static_cast<char>(c);
          }
      }
    }
    out << '"';
  }

  std::string serialize(const NodeCld::CLDOutput &output) {
    std::ostringstream out;
    // default ostream precision is 6 significant digits, which would
    // silently truncate score (a double) for any value with more digits
    // than that; 17 is enough to round-trip any double exactly, matching
    // what Napi::Number::New does natively on the native addon side.
    out.precision(17);
    out << "{\"reliable\":" << (output.isReliable ? "true" : "false")
        << ",\"textBytes\":" << output.textBytesFound
        << ",\"languages\":[";

    bool first = true;
    for (size_t i = 0; i < 3; i++) {
      CLD2::Language lang = output.language3[i];
      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }
      if (!first) out << ",";
      first = false;
      out << "{\"name\":";
      appendEscapedJSON(out, NodeCld::Constants::getInstance().getLanguageName(lang));
      out << ",\"code\":";
      appendEscapedJSON(out, NodeCld::Constants::getInstance().getLanguageCode(lang));
      out << ",\"percent\":" << output.percent3[i]
          << ",\"score\":" << output.normalized_score3[i] << "}";
    }
    out << "],\"chunks\":[";

    first = true;
    for (size_t i = 0; i < output.resultChunkVector.size(); i++) {
      const CLD2::ResultChunk &chunk = output.resultChunkVector.at(i);
      CLD2::Language lang = static_cast<CLD2::Language>(chunk.lang1);
      if (lang == CLD2::UNKNOWN_LANGUAGE) {
        continue;
      }
      if (!first) out << ",";
      first = false;
      out << "{\"name\":";
      appendEscapedJSON(out, NodeCld::Constants::getInstance().getLanguageName(lang));
      out << ",\"code\":";
      appendEscapedJSON(out, NodeCld::Constants::getInstance().getLanguageCode(lang));
      out << ",\"offset\":" << chunk.offset
          << ",\"bytes\":" << chunk.bytes << "}";
    }
    out << "]}";

    return out.str();
  }
}

extern "C" {
  // Returns a heap-allocated, NUL-terminated JSON string. Callers must use
  // ccall(..., 'number', ...) (NOT the 'string' convenience return type,
  // which decodes the string but never frees the underlying buffer) and
  // pass the returned pointer to cld_free() once done reading it.
  EMSCRIPTEN_KEEPALIVE
  char* cld_detect(
    const char* bytes, int numBytes,
    int isPlainText,
    const char* languageHint,
    const char* encodingHint,
    const char* tldHint,
    const char* httpHint,
    int bestEffort
  ) {
    NodeCld::CLDInput input;
    input.bytes = std::string(bytes, numBytes);
    input.numBytes = numBytes;
    input.isPlainText = isPlainText != 0;
    input.languageHint = languageHint ? languageHint : "";
    input.encodingHint = encodingHint ? encodingHint : "";
    input.tldHint = tldHint ? tldHint : "";
    input.httpHint = httpHint ? httpHint : "";
    input.bestEffort = bestEffort != 0;

    auto output = NodeCld::DetectLanguage(input);
    std::string json = serialize(*output);

    char *result = static_cast<char*>(malloc(json.size() + 1));
    memcpy(result, json.c_str(), json.size() + 1);
    return result;
  }

  EMSCRIPTEN_KEEPALIVE
  void cld_free(char* ptr) {
    free(ptr);
  }
}
