#include "helpers.h"


/**
 * 
 */
bool parseMetars(const String& payload, std::vector<Metar>& out) {
  out.clear();
  JsonDocument jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, payload);

  if (error) {
    Serial.printf("JSON error: %s\n", error.c_str());
    return false;
  }

  JsonArray jsonArray = jsonDoc.as<JsonArray>();

  for (JsonObject obj : jsonArray) {
    if (obj["icaoId"].isNull()) {
      Serial.println("No valid ICAO-code provided. Skipping.");
      continue;
    }

    Metar metar;

    if (obj["fltCat"].isNull() && !obj["visib"].isNull()) {      
      float visi = parseVisibility(obj["visib"]);
      metar.fltCat = calcFlightCategory(visi, 9999);
    } else {
      metar.fltCat = obj["fltCat"] | "";
    }

    metar.icao = obj["icaoId"] | "";
    metar.wspd = obj["wspd"] | -1;
    metar.wgst = obj["wgst"] | -1;

    out.push_back(metar);
  }
  return true;
}



String calcFlightCategory(float visibility, int ceiling) {
  if (visibility < 1.0) return "LIFR";
  else if (visibility < 3.0) return "IFR";
  else if (visibility < 5.0) return "MVFR";
  else return "VFR";
}



float parseVisibility(JsonVariant floatString) {
  float vis = NAN;

  if (floatString.isNull()) {
    return vis;
  }

  // Case: int or float
  if (floatString.is<float>() || floatString.is<int>()) {
    vis = floatString.as<float>();
  }

  // Case: string like "6+"
  else if (floatString.is<const char*>()) {
    const char* s = floatString.as<const char*>();

    char buf[8];
    strncpy(buf, s, sizeof(buf));
    buf[sizeof(buf) - 1] = '\0';

    // cutting off "+"
    char* plus = strchr(buf, '+');
    if (plus) *plus = '\0';

    vis = atof(buf);
  }
  return vis;
}

/*
CRGB getColorByFlightCategory(String flightCategory) {
  flightCategory.trim();
  if (flightCategory.equalsIgnoreCase("LIFR")) return COLOR_LIFR;
  if (flightCategory.equalsIgnoreCase("IFR"))  return COLOR_IFR;
  if (flightCategory.equalsIgnoreCase("MVFR")) return COLOR_MVFR;
  if (flightCategory.equalsIgnoreCase("VFR"))  return COLOR_VFR;
  return CRGB::Black;
}
  */


CRGB getLedColorByMetarAndWindCycle(Metar metar, bool windCycle) {
  bool windy = SHOW_WIND_ANIMATION &&
               windCycle &&
               (metar.wspd >= WIND_BLINK_THRESHOLD || metar.wgst >= WIND_BLINK_THRESHOLD);

  bool highWinds = HIGH_WINDS_THRESHOLD != -1 &&
                   (metar.wspd >= HIGH_WINDS_THRESHOLD || metar.wgst >= HIGH_WINDS_THRESHOLD);

  bool lightning = SHOW_LIGHTING_ANIMATION &&
                   windCycle &&
                   false;

  Serial.printf("%s: cat=%s wind=%d gust=%dkt | windy=%d highwinds=%d lightning=%d\n",
    metar.icao.c_str(), metar.fltCat.c_str(), metar.wspd, metar.wgst, windy, highWinds, lightning);
    
  return resolveColor(metar.fltCat, windy, highWinds, lightning);
}


CRGB resolveColor(String flightCategory, 
                  bool windy, 
                  bool highWinds, 
                  bool lightningConditions) {
  if (lightningConditions)                                return COLOR_LIGHTNING;
  if (windy && !highWinds)                                return fadeColorForCat(flightCategory);
  if ((windy && highWinds && SHOW_HIGH_WINDS_WARNING) 
      || (highWinds && !SHOW_WIND_ANIMATION && SHOW_HIGH_WINDS_WARNING)) return COLOR_HIGH_WINDS; // Show steady yellow if not animated but high wind warning enabled
  return baseColorForCat(flightCategory);
}


CRGB baseColorForCat(String cat) {
  if (cat.equalsIgnoreCase("VFR"))  return COLOR_VFR;
  if (cat.equalsIgnoreCase("MVFR")) return COLOR_MVFR;
  if (cat.equalsIgnoreCase("IFR"))  return COLOR_IFR;
  if (cat.equalsIgnoreCase("LIFR")) return COLOR_LIFR;
  return CRGB::Black;
}

CRGB fadeColorForCat(String cat) {
  if (cat.equalsIgnoreCase("VFR"))  return COLOR_VFR_FADE;
  if (cat.equalsIgnoreCase("MVFR")) return COLOR_MVFR_FADE;
  if (cat.equalsIgnoreCase("IFR"))  return COLOR_IFR_FADE;
  if (cat.equalsIgnoreCase("LIFR")) return COLOR_LIFR_FADE;
  return CRGB::Black;
}