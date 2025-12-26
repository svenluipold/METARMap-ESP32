#pragma once

#include <array>
#include <FastLED.h>


// ===== LED SETTINGS =====
constexpr int LED_PIN  = 5;
constexpr int NUM_LEDS = 10;


// ===== API SETTINGS =====
constexpr const char* BASE_URL = "https://aviationweather.gov/api";


// ===== AIRPORT LIST (Sort order must be identical to LED order) =====
constexpr std::array<const char*, 10> AIRPORTS = {
  "KSRQ", "KTPA", "KOCF", "KBKV", "KSEF", "KXMR", "KMCO", "KFPR", "KLAL", "KDAB"
};


// ===== PROGRAMM SETTINGS =====
constexpr int UPDATE_INTERVAL = 60;                 // In seconds

constexpr bool DAYTIME_DIMMING = false;             // Not yet implemented


// ===== NIGHT MODE SETTINGS =====
constexpr int NIGHT_SLEEP_START_HOUR = 22;          // 22:00
constexpr int NIGHT_SLEEP_START_MIN  = 0;

constexpr int NIGHT_SLEEP_END_HOUR   = 8;           // 08:00
constexpr int NIGHT_SLEEP_END_MIN    = 0;


// ===== WIND/LIGHTING ANIMATIONS =====
constexpr bool SHOW_WIND_ANIMATION      = true;
constexpr bool SHOW_HIGH_WINDS_WARNING  = true;

constexpr int WIND_BLINK_SPEED          = 1;        // In seconds
//constexpr int WIND_BLINK_DURATION = 60;           // In seconds

constexpr int WIND_BLINK_THRESHOLD	    = 15;	    // Knots of windspeed to blink/fade
constexpr int HIGH_WINDS_THRESHOLD	    = 25;	    // Knots of windspeed to trigger yellow LED indicating very High Winds. Disable by setting -1

constexpr bool SHOW_LIGHTING_ANIMATION  = false;    // Not yet implemented


// ===== COLORS for FLIGHT CATEGORIES =====
const CRGB COLOR_LIFR = CRGB(125,0,125);
const CRGB COLOR_IFR  = CRGB(255,0,0);
const CRGB COLOR_MVFR = CRGB(0,0,255);
const CRGB COLOR_VFR  = CRGB(0,255,0);

const CRGB COLOR_LIFR_FADE = CRGB(75,0,75);
const CRGB COLOR_IFR_FADE  = CRGB(125,0,0);
const CRGB COLOR_MVFR_FADE = CRGB(0,0,125);
const CRGB COLOR_VFR_FADE  = CRGB(0,125,0);

const CRGB COLOR_HIGH_WINDS = CRGB(255,255,0);
const CRGB COLOR_LIGHTNING  = CRGB(255,255,255);



