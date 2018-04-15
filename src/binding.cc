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

void TimeZoneMonitor::Notify() {
  Nan::HandleScope scope;

  Isolate *isolate = Isolate::GetCurrent();
#ifdef _WIN32
  _tzset();
#else
  tzset();
#endif
  Date::DateTimeConfigurationChangeNotification(isolate);
}

NAN_METHOD(Reset) {
  _monitor->Notify();
}

NAN_MODULE_INIT(Init) {
  _monitor = TimeZoneMonitor::Create();
  Set(target, New<String>("reset").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Reset)).ToLocalChecked());
}

NODE_MODULE(resetdatecache, Init)
