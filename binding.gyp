{
  "targets": [
    {
      "target_name": "cld",
      "dependencies": [
        "chromium/chromium.gyp:chromium"
      ],
      "include_dirs": [
        ".",
        "chromium"
      ],
      "sources": [ "src/cld.cc" ],
      "defines": [
        "CLD_WINDOWS"
      ]
    }
  ]
}
