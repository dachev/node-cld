#ifndef CLD_H
#define CLD_H

#include <v8.h>
#include <node.h>

namespace cld {
  class Cld {
    public:
      static void Init(v8::Handle<v8::Object> target);
      static v8::Handle<v8::Value> Detect(const v8::Arguments& args);

    protected:
      Cld() {}
      ~Cld() {}
  };
}

#endif