#include "monitor.h"

static const uint64_t kDebounceDelay = 500;

void TimeZoneMonitor::_asyncWorkCb(uv_async_t *work) {
  TimeZoneMonitor *monitor = static_cast<TimeZoneMonitor *>(work->data);
  monitor->ThreadedCallback();
  monitor->DebouncedNotify();
}

void TimeZoneMonitor::tzDelayExpired(uv_timer_t* handle) {
    TimeZoneMonitor *monitor = static_cast<TimeZoneMonitor *>(handle->data);
    monitor->Notify();
}

TimeZoneMonitor::TimeZoneMonitor() {
    uv_async_init(uv_default_loop(), &_asyncWork, _asyncWorkCb);
    uv_timer_init(uv_default_loop(), &_debouncer);
    _asyncWork.data = this;
    _debouncer.data = this;

    uv_unref((uv_handle_t*)&_asyncWork);
    uv_unref((uv_handle_t*)&_debouncer);
}

void TimeZoneMonitor::ThreadedNotify() {
    uv_async_send(&_asyncWork);
}

void TimeZoneMonitor::DebouncedNotify() {
    //Some platforms write the file instead of symlinking it, add a small delay just to be sure
    uv_timer_start(&_debouncer, TimeZoneMonitor::tzDelayExpired, kDebounceDelay, 0);
}

#ifdef _WIN32
# include "monitor_windows.cc"
#elif __ANDROID__
# include "monitor_android.cc"
#elif __APPLE__
# include "monitor_unix.cc"
#elif __unix__
# include "monitor_unix.cc"
#else
// static
TimeZoneMonitor *TimeZoneMonitor::Create() {
  return new TimeZoneMonitor();
}
#endif