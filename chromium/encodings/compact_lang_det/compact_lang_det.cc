// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "encodings/compact_lang_det/compact_lang_det.h"
#include "encodings/compact_lang_det/compact_lang_det_impl.h"
#include "encodings/compact_lang_det/win/cld_basictypes.h"

// String is "code_version - data_scrape_date"
static const char* kDetectLanguageVersion = "V1.6 - 20081121";

// Large-table version for all ~160 languages (all Tiers)

Language CompactLangDet::DetectLanguage(
                        const DetectionTables* tables,
                        const char* buffer,
                        int buffer_length,
                        bool is_plain_text,
                        bool do_allow_extended_languages,
                        bool do_pick_summary_language,
                        bool do_remove_weak_matches,
                        const char* tld_hint,       // "id" boosts Indonesian
                        int encoding_hint,          // SJS boosts Japanese
                        Language language_hint,     // ITALIAN boosts it
                        Language* language3,
                        int* percent3,
                        double* normalized_score3,
                        int* text_bytes,
                        bool* is_reliable) {
  int flags = 0;
  Language plus_one = UNKNOWN_LANGUAGE;

  Language lang = CompactLangDetImpl::DetectLanguageSummaryV25(
                          tables,
                          buffer,
                          buffer_length,
                          is_plain_text,
                          do_pick_summary_language,
                          do_remove_weak_matches,
                          tld_hint,               // "id" boosts Indonesian
                          encoding_hint,          // SJS boosts Japanese
                          language_hint,          // ITALIAN boosts it
                          do_allow_extended_languages,
                          flags,
                          plus_one,
                          language3,
                          percent3,
                          normalized_score3,
                          text_bytes,
                          is_reliable);
  // Do not default to English
  return lang;
  }



// Return version text string
// String is "code_version - data_scrape_date"
const char* CompactLangDet::DetectLanguageVersion() {
  return kDetectLanguageVersion;
}

