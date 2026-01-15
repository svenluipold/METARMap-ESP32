#pragma once

#include <array>
#include <FastLED.h>


// ===== GENERAL SETUP =====
constexpr int LED_PIN   = 5;
constexpr int NUM_LEDS  = 10;

constexpr int LED_BRIGHTNESS_DAY = 4;           //(Good value: 25)
constexpr int LED_BRIGHTNESS_DIMMED = 3;


// Time zone Berlin CET with summertime
static const char* TZ_INFO = "CET-1CEST,M3.5.0/2,M10.5.0/3";      // Timezone for NTP time. See https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html for more info 



// ===== AIRPORT LIST (Sort order must be identical to LED order) =====
constexpr std::array<const char*, 10> AIRPORTS = {
  "KSRQ", "KTPA", "KOCF", "KBKV", "KSEF", "KXMR", "KMCO", "KFPR", "KLAL", "KDAB"
};

/*
constexpr std::array<const char*, 9> AIRPORTS = {
  "KNFG", "KPSP", "KVCV", "KPMD", "KOXR", "KLAX", "KSNA", "KONT", "KSBD"
};
*/


// ===== PROGRAMM SETTINGS =====
constexpr int UPDATE_INTERVAL           = 300;          // In seconds
constexpr bool ENABLE_NIGHT_SLEEP       = true;         // Not yet implemented
constexpr bool ENABLE_DAYTIME_DIMMING   = false;        // Not yet implemented



// ===== NIGHT SLEEP SETTINGS =====
constexpr int NIGHT_SLEEP_START_HOUR = 22;          // 22:00
constexpr int NIGHT_SLEEP_START_MIN  = 0;

constexpr int NIGHT_SLEEP_END_HOUR   = 8;           // 08:00
constexpr int NIGHT_SLEEP_END_MIN    = 0;



// ===== WIND/LIGHTING ANIMATIONS =====
constexpr bool SHOW_WIND_ANIMATION      = true;
constexpr bool SHOW_HIGH_WINDS_WARNING  = true;

constexpr int WIND_BLINK_SPEED          = 1;        // In seconds
constexpr int WIND_BLINK_DURATION       = 60;       // Not yet implemented

constexpr int WIND_BLINK_THRESHOLD	    = 15;	      // Knots of windspeed to blink/fade
constexpr int HIGH_WINDS_THRESHOLD	    = 25;	      // Knots of windspeed to trigger yellow LED indicating very High Winds. Disable by setting -1

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



// ===== API SETTINGS =====
constexpr const char* BASE_URL = "https://aviationweather.gov/api";



// ===== MISC SETTINGS =====
constexpr int NTP_MAX_RETRIES         = 3;          // Maximum amount of NTP-retries before deepsleep
constexpr int NTP_RETRY_DELAY_SECONDS = 5;          // 5 seconds break in between NTP-Time tries
constexpr int NTP_FAIL_SLEEP_SECONDS  = 10;         // If NTP continues to fail --> deepsleep for 900 seconds