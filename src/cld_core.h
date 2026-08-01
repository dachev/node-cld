#ifndef NODE_CLD_CORE_H
#define NODE_CLD_CORE_H

#include <memory>
#include <string>

#include "compact_lang_det.h"

namespace NodeCld {
  struct CLDInput {
    std::string bytes,
      languageHint,
      encodingHint,
      tldHint,
      httpHint;
    int numBytes;
    bool isPlainText;
    bool bestEffort;
  };

  struct CLDOutput {
    CLD2::Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    CLD2::ResultChunkVector resultChunkVector;
    int textBytesFound;
    bool isReliable;
  };

  std::unique_ptr<CLDOutput> DetectLanguage(const CLDInput &input);
}

#endif
