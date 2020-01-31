{
  "targets": [
    {
      "target_name": "cld",
      "dependencies": [
        "deps/cld/binding.gyp:cld-c"
      ],
      "include_dirs": [
        "deps/cld/public",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "sources": ["src/constants.cc", "src/cld.cc"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-Wall", "-fno-exceptions"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7",
        "OTHER_CFLAGS": ["-Wall"]
      },
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1 }
      },
      "conditions": [
        ['OS=="mac"', {
          "cflags+": ["-fvisibility=hidden"],
          "xcode_settings": {
            "GCC_SYMBOLS_PRIVATE_EXTERN": "YES"
          }
        }]
      ]
    }
  ]
}
