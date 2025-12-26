#include "sleep.h"


static const char* NTP1 = "pool.ntp.org";
static const char* NTP2 = "time.nist.gov";

// Europe/Berlin incl. Sommerzeit (DST)
static const char* TZ_INFO = "CET-1CEST,M3.5.0/2,M10.5.0/3";

bool isNightMode(const tm& t) {
  int nowMin = t.tm_hour * 60 + t.tm_min;
  int start  = NIGHT_SLEEP_START_HOUR * 60 + NIGHT_SLEEP_START_MIN;
  int end    = NIGHT_SLEEP_END_HOUR   * 60 + NIGHT_SLEEP_END_MIN;
  // If a night is past midnight (day must be increased)
  return (nowMin >= start) || (nowMin < end);
}


uint32_t secondsUntilNextWake(const tm& now) {
  tm target = now;
  target.tm_hour = NIGHT_SLEEP_END_HOUR;
  target.tm_min  = NIGHT_SLEEP_END_MIN;
  target.tm_sec  = 0;

  time_t nowEpoch = mktime(const_cast<tm*>(&now));
  time_t targetEpoch = mktime(&target);

  if (targetEpoch <= nowEpoch) {
    target.tm_mday += 1;
    targetEpoch = mktime(&target);
  }
  return (uint32_t)difftime(targetEpoch, nowEpoch);
}


bool syncTimeOrFail() {
  int timeoutMilis = 8000;
  configTzTime(TZ_INFO, NTP1, NTP2);

  const uint32_t start = millis();
  time_t now = 0;
  while (millis() - start < timeoutMilis) {
    time(&now);
    if (now > 1700000000) return true; // grob: Zeit ist gesetzt
    delay(200);
  }
  return false;
}

// Callback to turn off leds 
void checkforEnteringNightSleep(const std::function<void(void)>& beforeSleep) {
  time_t nowEpoch;
  time(&nowEpoch);
  tm local;
  localtime_r(&nowEpoch, &local);

  if (!isNightMode(local)) return;

  uint32_t seconds = secondsUntilNextWake(local);

  // Make callbacl to main.cpp --> turn off leds
  if (beforeSleep) beforeSleep();

  esp_sleep_enable_timer_wakeup((uint64_t)seconds * 1000000ULL);
  esp_deep_sleep_start();
}