#pragma once

#include <Arduino.h>


struct Metar {
  String icao;
  String fltCat;
  int wspd = -1;
  int wgst = -1;
};