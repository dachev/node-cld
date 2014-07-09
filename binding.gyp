{
  "targets": [
    {
      "target_name": "cld",
      "dependencies": [
        "cld/binding.gyp:cld"
      ],
      "include_dirs": [
        "cld/public",
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
