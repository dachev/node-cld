#!/usr/bin/env bash
# Builds the WASM fallback backend from the same CLD2 sources used by
# binding.gyp's native build. Requires the Emscripten SDK on PATH (built and
# verified with emsdk 6.0.5 / emcc 6.0.5) -- see https://emscripten.org/docs/getting_started/downloads.html
# or run: source /path/to/emsdk/emsdk_env.sh
set -euo pipefail

cd "$(dirname "$0")/.."

if ! command -v emcc >/dev/null 2>&1; then
  echo "error: emcc not found on PATH. Install/activate the Emscripten SDK first." >&2
  exit 1
fi

CLD_INTERNAL=deps/cld/internal

# Mirrors deps/cld/binding.gyp's cld-c target source list exactly -- keep
# this in sync if that file's list ever changes.
CLD_SOURCES=(
  "$CLD_INTERNAL/cldutil.cc"
  "$CLD_INTERNAL/cldutil_shared.cc"
  "$CLD_INTERNAL/compact_lang_det.cc"
  "$CLD_INTERNAL/compact_lang_det_hint_code.cc"
  "$CLD_INTERNAL/compact_lang_det_impl.cc"
  "$CLD_INTERNAL/debug.cc"
  "$CLD_INTERNAL/fixunicodevalue.cc"
  "$CLD_INTERNAL/generated_entities.cc"
  "$CLD_INTERNAL/generated_language.cc"
  "$CLD_INTERNAL/generated_ulscript.cc"
  "$CLD_INTERNAL/getonescriptspan.cc"
  "$CLD_INTERNAL/lang_script.cc"
  "$CLD_INTERNAL/offsetmap.cc"
  "$CLD_INTERNAL/scoreonescriptspan.cc"
  "$CLD_INTERNAL/tote.cc"
  "$CLD_INTERNAL/utf8statetable.cc"
  "$CLD_INTERNAL/cld_generated_cjk_uni_prop_80.cc"
  "$CLD_INTERNAL/cld2_generated_cjk_compatible.cc"
  "$CLD_INTERNAL/cld_generated_cjk_delta_bi_32.cc"
  "$CLD_INTERNAL/generated_distinct_bi_0.cc"
  "$CLD_INTERNAL/cld2_generated_quad0122.cc"
  "$CLD_INTERNAL/cld2_generated_deltaocta0122.cc"
  "$CLD_INTERNAL/cld2_generated_deltaoctachrome.cc"
  "$CLD_INTERNAL/cld2_generated_distinctocta0122.cc"
  "$CLD_INTERNAL/cld2_generated_distinctoctachrome.cc"
  "$CLD_INTERNAL/cld2_generated_quadchrome_16.cc"
  "$CLD_INTERNAL/cld2_generated_quadchrome_2.cc"
  "$CLD_INTERNAL/cld_generated_score_quad_octa_0122.cc"
  "$CLD_INTERNAL/cld_generated_score_quad_octa_2.cc"
)

GLUE_SOURCES=(
  src/constants.cc
  src/cld_core.cc
  wasm/glue.cc
)

COMMON_FLAGS=(
  -O3
  -std=gnu++98 -w
  -fno-exceptions
  -I deps/cld/public
  -I "$CLD_INTERNAL"
  -I src
  -s MODULARIZE=1
  -s EXPORTED_FUNCTIONS=_cld_detect,_cld_free,_malloc,_free
  -s EXPORTED_RUNTIME_METHODS=ccall,UTF8ToString
  -s ALLOW_MEMORY_GROWTH=1
  -s WASM_ASYNC_COMPILATION=1
  # deps/cld's generated data-table files intentionally define overlapping
  # symbols across translation units (the native build tolerates this via
  # binding.gyp's "-z muldefs" ldflag) -- wasm-ld's equivalent:
  -Wl,--allow-multiple-definition
)

mkdir -p wasm/dist

echo "Building Node target -> wasm/dist/cld.node.js"
emcc \
  "${COMMON_FLAGS[@]}" \
  -s ENVIRONMENT=node \
  -s EXPORT_NAME=createCldModule \
  "${CLD_SOURCES[@]}" "${GLUE_SOURCES[@]}" \
  -o wasm/dist/cld.node.js

echo "Building browser target -> wasm/dist/cld.web.mjs"
emcc \
  "${COMMON_FLAGS[@]}" \
  -s ENVIRONMENT=web \
  -s EXPORT_ES6=1 \
  -s EXPORT_NAME=createCldModule \
  "${CLD_SOURCES[@]}" "${GLUE_SOURCES[@]}" \
  -o wasm/dist/cld.web.mjs

echo "Done."
