#include "monitor.h"

static const DWORD dwFilter = REG_NOTIFY_CHANGE_LAST_SET;

class TimeZoneMonitorWindows : public TimeZoneMonitor {
 public:
  TimeZoneMonitorWindows();
 protected:
  void ThreadedCallback();
 private:
  void Notify();
  void SetUp(void);
  static void TimeZoneChangeThreadedCallback(PVOID context, BOOLEAN timeout);
  HANDLE hTZEvent;
  HKEY   hRegKey;
};


void TimeZoneMonitorWindows::ThreadedCallback() {
    //Setup only causes notify to be called once
    SetUp();
}

void TimeZoneMonitorWindows::SetUp() {
   ResetEvent(hTZEvent);
    // Watch the registry key for a change of value, must be run on main thread
   RegNotifyChangeKeyValue(hRegKey, TRUE,
        dwFilter, hTZEvent, TRUE);
}

TimeZoneMonitorWindows::TimeZoneMonitorWindows() {

   HANDLE hWait;
   LONG   lErrorCode;
   lErrorCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation", 0, KEY_NOTIFY, &hRegKey);
   if (lErrorCode != ERROR_SUCCESS) return;

   // Create an event.
   hTZEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (hTZEvent == NULL) return;

   SetUp();

   //Invoke the TimeZoneChangeThreadedCallback upon a change
   RegisterWaitForSingleObject(
      &hWait, hTZEvent, TimeZoneChangeThreadedCallback,
      this, INFINITE, WT_EXECUTEINWAITTHREAD
    );
}

//static
void TimeZoneMonitorWindows::TimeZoneChangeThreadedCallback(PVOID context, BOOLEAN timeout) {
    TimeZoneMonitorWindows *monitor = static_cast<TimeZoneMonitorWindows *>(context);
    monitor->ThreadedNotify(); //First enter the main eventloop, then debounce the notify
}

// static
TimeZoneMonitor *TimeZoneMonitor::Create() {
  return new TimeZoneMonitorWindows();
}