#include "cld_core.h"
#include "encodings.h"
#include "constants.h"

namespace NodeCld {
  std::unique_ptr<CLDOutput> DetectLanguage(const CLDInput &input) {
    std::unique_ptr<CLDOutput> output(new CLDOutput);
    CLD2::CLDHints hints;
    hints.tld_hint = 0;
    hints.content_language_hint = 0;
    hints.language_hint = CLD2::UNKNOWN_LANGUAGE;
    hints.encoding_hint = CLD2::UNKNOWN_ENCODING;

    if (input.languageHint.length() > 0) {
      hints.language_hint = Constants::getInstance().getLanguageFromName(input.languageHint.c_str());
    }

    if (input.encodingHint.length() > 0) {
      hints.encoding_hint = Constants::getInstance().getEncodingFromName(input.encodingHint.c_str());
    }

    if (input.tldHint.length() > 0) {
      hints.tld_hint = input.tldHint.c_str();
    }

    if (input.httpHint.length() > 0) {
      hints.content_language_hint = input.httpHint.c_str();
    }
    int flags = 0;
    if (input.bestEffort) {
      flags |= CLD2::kCLDFlagBestEffort;
    }

    CLD2::ExtDetectLanguageSummary(
      input.bytes.c_str(),
      input.numBytes,
      input.isPlainText,
      &hints,
      flags,
      output->language3,
      output->percent3,
      output->normalized_score3,
      &output->resultChunkVector,
      &output->textBytesFound,
      &output->isReliable
    );

    return output;
  }
}
