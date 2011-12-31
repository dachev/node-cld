#!/usr/bin/env node

var cld    = require('../build/default/cld');
var data   = require('./data');
var assert = require('assert');

function runOne(expectedLangName, text, shouldBeReliable) {
  var detectionResult = cld.detect(text);
  
  assert.equal(expectedLangName, detectionResult.name);
  
  if (shouldBeReliable == true) {
    assert.equal(detectionResult.reliable, true, detectionResult.name);
  }
}

function runAll(tests) {
  for (var i = 0; i < tests.length; i++) {
    var test = tests[i];
    runOne(test.name, test.text, test.reliable);
  }
}

var tests  = [
  {name:'AFRIKAANS', text:data.kTeststr_af_Latn, reliable:true},
  {name:'AMHARIC', text:data.kTeststr_am_Ethi, reliable:true},
  {name:'ARABIC', text:data.kTeststr_ar_Arab, reliable:false},
  {name:'AZERBAIJANI', text:data.kTeststr_az_Latn, reliable:true},
  {name:'BELARUSIAN', text:data.kTeststr_be_Cyrl, reliable:true},
  {name:'BULGARIAN', text:data.kTeststr_bg_Cyrl, reliable:true},
  {name:'BENGALI', text:data.kTeststr_bn_Beng, reliable:true},
  {name:'TIBETAN', text:data.kTeststr_bo_Tibt, reliable:true},
  {name:'SERBIAN', text:data.kTeststr_bs_Cyrl, reliable:true},
  //{name:'CROATIAN', text:data.kTeststr_bs_Latn, reliable:true},
  {name:'CATALAN', text:data.kTeststr_ca_Latn, reliable:true},
  {name:'CHEROKEE', text:data.kTeststr_chr_Cher, reliable:true},
  {name:'CZECH', text:data.kTeststr_cs_Latn, reliable:true},
  {name:'WELSH', text:data.kTeststr_cy_Latn, reliable:true},
  {name:'DANISH', text:data.kTeststr_da_Latn, reliable:true},
  {name:'GERMAN', text:data.kTeststr_de_Latn, reliable:true},
  {name:'DHIVEHI', text:data.kTeststr_dv_Thaa, reliable:true},
  {name:'GREEK', text:data.kTeststr_el_Grek, reliable:true},
  {name:'ENGLISH', text:data.kTeststr_en_Latn, reliable:true},
  {name:'ENGLISH', text:data.kTeststr_en, reliable:true},
  {name:'SPANISH', text:data.kTeststr_es_Latn, reliable:true},
  {name:'ESTONIAN', text:data.kTeststr_et_Latn, reliable:true},
  {name:'BASQUE', text:data.kTeststr_eu_Latn, reliable:true},
  {name:'PERSIAN', text:data.kTeststr_fa_Arab, reliable:true},
  {name:'FINNISH', text:data.kTeststr_fi_Latn, reliable:true},
  {name:'FRENCH', text:data.kTeststr_fr_Latn, reliable:true},
  {name:'IRISH', text:data.kTeststr_ga_Latn, reliable:true},
  {name:'GALICIAN', text:data.kTeststr_gl_Latn2, reliable:true},
  {name:'GUJARATI', text:data.kTeststr_gu_Gujr, reliable:true},
  {name:'HINDI', text:data.kTeststr_hi_Deva, reliable:true},
  {name:'HINDI', text:data.kTeststr_ks, reliable:true},
  {name:'CROATIAN', text:data.kTeststr_hr_Latn, reliable:false},
  {name:'HAITIAN_CREOLE', text:data.kTeststr_ht_Latn, reliable:true},
  {name:'HUNGARIAN', text:data.kTeststr_hu_Latn, reliable:true},
  {name:'ARMENIAN', text:data.kTeststr_hy_Armn, reliable:true},
  {name:'MALAY', text:data.kTeststr_id_Latn, reliable:true},
  {name:'ICELANDIC', text:data.kTeststr_is_Latn, reliable:true},
  {name:'ITALIAN', text:data.kTeststr_it_Latn, reliable:true},
  {name:'INUKTITUT', text:data.kTeststr_iu_Cans, reliable:true},
  {name:'HEBREW', text:data.kTeststr_iw_Hebr, reliable:true},
  {name:'Japanese', text:data.kTeststr_ja_Hani, reliable:true},
  {name:'GEORGIAN', text:data.kTeststr_ka_Geor, reliable:true},
  {name:'KHMER', text:data.kTeststr_km_Khmr, reliable:true},
  {name:'KANNADA', text:data.kTeststr_kn_Knda, reliable:true},
  {name:'Korean', text:data.kTeststr_ko_Hani, reliable:true},
  {name:'LAOTHIAN', text:data.kTeststr_lo_Laoo, reliable:true},
  {name:'LITHUANIAN', text:data.kTeststr_lt_Latn, reliable:true},
  {name:'LATVIAN', text:data.kTeststr_lv_Latn, reliable:true},
  {name:'MACEDONIAN', text:data.kTeststr_mk_Cyrl, reliable:true},
  {name:'MALAYALAM', text:data.kTeststr_ml_Mlym, reliable:true},
  {name:'MALAY', text:data.kTeststr_ms_Latn, reliable:true},
  {name:'MALAY', text:data.kTeststr_ms_Latn2, reliable:true},
  {name:'MALAY', text:data.kTeststr_ms_Latn3, reliable:true},
  {name:'MALTESE', text:data.kTeststr_mt_Latn, reliable:true},
  {name:'BURMESE', text:data.kTeststr_my_Mymr, reliable:true},
  {name:'DUTCH', text:data.kTeststr_nl_Latn, reliable:true},
  {name:'NORWEGIAN', text:data.kTeststr_no_Latn, reliable:true},
  {name:'ORIYA', text:data.kTeststr_or_Orya, reliable:true},
  {name:'PUNJABI', text:data.kTeststr_pa_Guru, reliable:true},
  {name:'POLISH', text:data.kTeststr_pl_Latn, reliable:true},
  {name:'PORTUGUESE', text:data.kTeststr_pt_BR, reliable:true},
  {name:'ROMANIAN', text:data.kTeststr_ro_Latn, reliable:true},
  {name:'RUSSIAN', text:data.kTeststr_ru_Cyrl, reliable:true},
  {name:'SINHALESE', text:data.kTeststr_si_Sinh, reliable:true},
  {name:'SLOVAK', text:data.kTeststr_sk_Latn, reliable:true},
  {name:'SLOVENIAN', text:data.kTeststr_sl_Latn, reliable:false},
  {name:'ALBANIAN', text:data.kTeststr_sq_Latn, reliable:true},
  {name:'SERBIAN', text:data.kTeststr_sr_Cyrl, reliable:true},
  {name:'CROATIAN', text:data.kTeststr_sr_Latn, reliable:true},
  {name:'CROATIAN', text:data.kTeststr_sr_ME_Latn, reliable:true},
  {name:'SWEDISH', text:data.kTeststr_sv_Latn, reliable:true},
  {name:'SWAHILI', text:data.kTeststr_sw_Latn, reliable:true},
  {name:'SYRIAC', text:data.kTeststr_syr_Syrc, reliable:true},
  {name:'TAMIL', text:data.kTeststr_ta_Taml, reliable:true},
  {name:'TELUGU', text:data.kTeststr_te_Telu, reliable:true},
  {name:'THAI', text:data.kTeststr_th_Thai, reliable:true},
  {name:'TAGALOG', text:data.kTeststr_tl_Latn, reliable:true},
  {name:'TURKISH', text:data.kTeststr_tr_Latn, reliable:true},
  {name:'UKRAINIAN', text:data.kTeststr_uk_Cyrl, reliable:true},
  {name:'URDU', text:data.kTeststr_ur_Arab, reliable:true},
  {name:'VIETNAMESE', text:data.kTeststr_vi_Latn, reliable:true},
  {name:'YIDDISH', text:data.kTeststr_yi_Hebr, reliable:true},
  {name:'Chinese', text:data.kTeststr_zh_Hani, reliable:true},
  {name:'ChineseT', text:data.kTeststr_zh_TW, reliable:true},
  {name:'INDONESIAN', text:data.kTeststr_id, reliable:true}
];

runAll(tests);


