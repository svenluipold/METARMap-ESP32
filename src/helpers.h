#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <vector>

#include "metar.h"
#include "settings.h"


bool parseMetars(const String& payload, std::vector<Metar>& out);
String calcFlightCategory(float visibility, int ceiling);
float parseVisibility(JsonVariant floatString);

CRGB getLedColorByMetarAndWindCycle(Metar metar, bool windCycle);
CRGB resolveColor(String flightCategory, bool windCycle, bool windy, bool highWinds, bool lightningConditions);
CRGB baseColorForCat(String cat);
CRGB fadeColorForCat(String cat);
