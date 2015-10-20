#include <nan.h>
#include <node.h>
#include <v8.h>

using v8::Date;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_METHOD(Reset) {
  Isolate *isolate = Isolate::GetCurrent();
  Date::DateTimeConfigurationChangeNotification(isolate);
}

NAN_MODULE_INIT(Init) {
  Set(target, New<String>("reset").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Reset)).ToLocalChecked());
}

NODE_MODULE(resetdatecache, Init)
