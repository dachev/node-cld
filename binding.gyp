{
  "targets": [
    {
      "target_name": "cld",
      "dependencies": [
        "cld/binding.gyp:cld"
      ],
      "include_dirs": [
        "cld/public"
      ],
      "sources": ["src/constants.cc", "src/cld.cc"],
      "defines": []
    }
  ]
}
