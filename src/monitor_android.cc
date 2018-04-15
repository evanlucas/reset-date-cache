
//TODO: We should catch the TIMEZONE_CHANGED intent and fire ThreadedNotify
//Unfortunatly, it looks like this part requires some jni magic which may not work from inside a module
//Lets disable the notifications for now...

// static
TimeZoneMonitor *TimeZoneMonitor::Create() {
  return new TimeZoneMonitor();
}