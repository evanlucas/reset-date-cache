
#ifndef _TZD_MONITOR_H
#define _TZD_MONITOR_H
#include <uv.h>

class TimeZoneMonitor {
 public:
  // Returns a new TimeZoneMonitor object specific to the platform.
  static TimeZoneMonitor *Create();
  void Notify();
 protected:
  TimeZoneMonitor(); //Creation can only occur through create.
  void DebouncedNotify();
  void ThreadedNotify();
  virtual void ThreadedCallback() {}
 private:
  static void tzDelayExpired(uv_timer_t* handle);
  static void _asyncWorkCb(uv_async_t *work);
  uv_async_t _asyncWork;
  uv_timer_t _debouncer;
};

#endif