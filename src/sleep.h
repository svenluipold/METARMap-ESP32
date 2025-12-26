#pragma one

#include <Arduino.h>
#include <time.h>
#include <WiFi.h>

#include "settings.h"


bool isNightMode(const tm& t);
uint32_t secondsUntilNext0800(const tm& now);
bool syncTimeOrFail();
void checkforEnteringNightSleep(const std::function<void(void)>& beforeSleep);