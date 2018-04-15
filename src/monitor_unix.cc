#include <uv.h>
#include <string.h>
#include "monitor.h"


// There is no true standard for where time zone information is actually
// stored. glibc uses /etc/localtime, uClibc uses /etc/TZ, and some older
// systems store the name of the time zone file within /usr/share/zoneinfo
// in /etc/timezone. Different libraries and custom builds may mean that
// still more paths are used. Just watch all three of these paths, because
// false positives are harmless, assuming the false positive rate is
// reasonable.
static const char* kFilesToWatch[] = {
  "localtime",
  "timezone",
  "TZ",
};
// libuv seems to have some troubles when monitoring symlinks.
//It does however work if we monitor the parent folder non-recursively
static const char* kWatchFolder = "/etc/";

class TimeZoneMonitorUnix : public TimeZoneMonitor {
 public:
  TimeZoneMonitorUnix();
 private:
  static void tzChange(uv_fs_event_t *handle, const char *filename, int events, int status);
  uv_fs_event_t fileWatcher;
};


void TimeZoneMonitorUnix::tzChange(uv_fs_event_t *handle, const char *filename, int events, int status) {
    TimeZoneMonitorUnix *monitor = static_cast<TimeZoneMonitorUnix *>(handle->data);
    for (size_t i = 0; i < sizeof(kFilesToWatch)/sizeof(kFilesToWatch[0]); i++) {
        if(strcmp(kFilesToWatch[i], filename) == 0) {
            monitor->DebouncedNotify(); //Debounce the notification (sometimes multiple files change)
            return;
        }
    }
}

TimeZoneMonitorUnix::TimeZoneMonitorUnix()
{
    uv_fs_event_init(uv_default_loop(), &fileWatcher);
    fileWatcher.data = this;
    uv_unref((uv_handle_t*)&fileWatcher);

    uv_fs_event_start(&fileWatcher, TimeZoneMonitorUnix::tzChange, kWatchFolder, 0);
}

// static
TimeZoneMonitor *TimeZoneMonitor::Create() {
    return new TimeZoneMonitorUnix();
}