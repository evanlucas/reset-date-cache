#include <nan.h>
#include <node.h>
#include <v8.h>
#include <time.h>

#include "monitor.h"

using v8::Date;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

TimeZoneMonitor *_monitor;
Nan::Callback *_hook = NULL;

void TimeZoneMonitor::Notify() {
  Nan::HandleScope scope;

  Isolate *isolate = Isolate::GetCurrent();
#ifdef _WIN32
  _tzset();
#else
  tzset();
#endif
  Date::DateTimeConfigurationChangeNotification(isolate);
  if(_hook) {
      _hook->Call(0, NULL);
  }
}

NAN_METHOD(Reset) {
  _monitor->Notify();
}

NAN_METHOD(SetHook) {
  if(_hook)
    delete _hook;

  if(info[0]->IsFunction())
    _hook = new Nan::Callback(v8::Local<v8::Function>::Cast(info[0]));
  else
    _hook = NULL;
}

NAN_MODULE_INIT(Init) {
  _monitor = TimeZoneMonitor::Create();
  Set(target, New<String>("reset").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Reset)).ToLocalChecked());
  Set(target, New<String>("setHook").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(SetHook)).ToLocalChecked());
}

NODE_MODULE(resetdatecache, Init)
