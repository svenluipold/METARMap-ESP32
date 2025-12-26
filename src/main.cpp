#include <Arduino.h>
#include <FastLED.h>
#include <HTTPClient.h>
#include <map>
#include <vector>
#include <WiFi.h>

#include "helpers.h"
#include "metar.h"
#include "secrets.h"
#include "settings.h"
#include "sleep.h"


CRGB leds[NUM_LEDS];

std::vector<Metar> resolvedMetars;
std::map<String, size_t> airportMetarIndexMap;

void ledsOff();


/* Program setup */
void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(25);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  Serial.println("\nWiFi connected");

  // Getting time via NTP and initate sleep if current time is sleeptime
  if (syncTimeOrFail()) {
    checkforEnteringNightSleep(ledsOff);
  } else {
    Serial.println("Error while NTP connect ... sleeping 30 minutes.");
    ESP.deepSleep(30 * 60 * 1000000ULL);
  }
}


/* Program loop */
void loop() {
  boolean resultRecieved = false;
  HTTPClient http;
  CRGB choosenColor = CRGB::Green;
  
  String requestURL;
  requestURL = String(BASE_URL);
  requestURL += "/data/metar?format=json&ids=";

  for (size_t i = 0; i < AIRPORTS.size(); i++) {
    if (i > 0) requestURL += ",";
    requestURL += AIRPORTS[i];
  }

  http.begin(requestURL);
  int code = http.GET();

  if (code == 200) {
    String body = http.getString();
    Serial.println(body);

    if (parseMetars(body, resolvedMetars)) {
      // Copying resolved metars into map with ICAO-Code as key and metar-index as value
      for (size_t i = 0; i < resolvedMetars.size(); ++i) {
        airportMetarIndexMap[resolvedMetars[i].icao] = i;
      }

      Serial.print("Lookup map entries: ");
      Serial.println(airportMetarIndexMap.size());

    } else {
      return;
    }
  } else {
    Serial.println("Error - trying again soon ...");
    return;
  }


  int windCyclesRemaining = 1;
  bool windCycle = false;


  if (SHOW_WIND_ANIMATION)
    windCyclesRemaining = UPDATE_INTERVAL / WIND_BLINK_SPEED;


  // wind-loop
  while (windCyclesRemaining > 0) {
    int currentLoopIndex = 0;

    for (const auto& airport : AIRPORTS) {
      // Obtain index (iterator) of METAR-dataset from current airport in resolvedMetars from airportMetarIndexMap
      auto iterator = airportMetarIndexMap.find(airport);

      // No METAR info exists for airport --> blank and skip
      if (iterator == airportMetarIndexMap.end()) {
        Serial.printf("%s: No METAR-Info received. Skipping.\n", String(airport));
        leds[currentLoopIndex] == CRGB::Black;
        currentLoopIndex += 1;
        continue;
      }

      // Load resolved metar from index (iterator) of resolvedMetars
      const Metar& metar = resolvedMetars[iterator -> second];
      leds[currentLoopIndex] = getLedColorByMetarAndWindCycle(metar, windCycle);
      
      currentLoopIndex += 1;
    }
    FastLED.show();
    windCyclesRemaining -= 1;
    windCycle = !windCycle;

    delay(WIND_BLINK_SPEED * 1000);

    if (SHOW_WIND_ANIMATION)
      Serial.printf("%d wind cyles remaining ... \n", windCyclesRemaining);
  }
  
  // Make ESP sleep if no wind animation is wanted
  if (!SHOW_WIND_ANIMATION) {
    Serial.printf("Sleeping %d seconds ... \n", UPDATE_INTERVAL);
    ESP.deepSleep(UPDATE_INTERVAL * 1 * 1000000ULL);
  }
}

void ledsOff() {
  FastLED.clear();
  FastLED.show();
}