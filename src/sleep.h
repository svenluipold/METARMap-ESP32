#pragma once

#include <Arduino.h>
#include <time.h>
#include <WiFi.h>

#include "settings.h"


bool isSleepTime(const tm& t);
uint32_t secondsUntilNext0800(const tm& now);
bool getTimeOrFail();
void checkforEnteringNightSleep(const std::function<void(void)>& beforeSleep);
static bool isTimeSet();