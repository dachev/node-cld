{
  "targets": [
    {
      "target_name": "cld",
      "dependencies": [
        "deps/cld/binding.gyp:cld-c"
      ],
      "include_dirs": [
        "deps/cld/public",
        "<!(node -e \"require('nan')\")"
      ],
      "sources": ["src/constants.cc", "src/cld.cc"],
      "defines": [],
      "cflags_cc": ["-Wall"],
      "xcode_settings": {
        "OTHER_CFLAGS": ["-Wall"]
      }
    }
  ]
}
