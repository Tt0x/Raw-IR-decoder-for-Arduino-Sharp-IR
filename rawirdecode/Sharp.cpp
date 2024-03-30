#include <Arduino.h>

bool decodeSharp(byte *bytes, int byteCount)
{
  // If this looks like a Sharp code...
  if ( byteCount == 13 && bytes[0] == 0xAA && bytes[1] == 0x5A && bytes[2] == 0xCF ) {
    Serial.println(F("Looks like a Sharp protocol"));

    // Power mode
    switch (bytes[5]) {
      case 0x21:
        Serial.println(F("POWER OFF"));
        break;
      case 0x31:
        Serial.println(F("POWER ON"));
        break;
      case 0x81:
        Serial.println(F("Timer POWER OFF"));
        break;
    }

    // Operating mode
    switch (bytes[6] & 0x0F) {
      case 0x01:
        if (bytes[4] == 0x00) {
          Serial.println(F("MODE MAINTENANCE HEAT"));
        } else {
          Serial.println(F("MODE HEAT"));
        }
        break;
      case 0x02:
        Serial.println(F("MODE COOL"));
        break;
      case 0x03:
        Serial.println(F("MODE DRY"));
        break;
      case 0x04:
        Serial.println(F("MODE ION"));
        break;      
    }

    // Temperature
    Serial.print(F("Temperature: "));
    if (bytes[4] == 0x00) {
      Serial.println(F("10"));
    } else {
      Serial.println(bytes[4] + 17);
    }
    
    // Fan
    switch (bytes[6] & 0xF0) {
      case 0x20:
        Serial.println(F("FAN: AUTO"));
        break;
      case 0x30:
        Serial.println(F("FAN: 1"));
        break;
      case 0x50:
        Serial.println(F("FAN: 2"));
        break;
      case 0x70:
        Serial.println(F("FAN: 3"));
        break;
      case 0x60:
        Serial.println(F("FAN: 4"));
        break;      
    }
    // Flap position
    switch (bytes[8] & 0x0F) {
      case 0x08:
        Serial.println(F("FLAP: AUTO"));
        break;
      case 0x09:
        Serial.println(F("FLAP: TOP"));
        break;
      case 0x0A:
        Serial.println(F("FLAP: TOP-MIDDLE"));
        break;
      case 0x0B:
        Serial.println(F("FLAP: MIDDLE"));
        break;
      case 0x0C:
        Serial.println(F("FLAP: MIDDLE-DOWN"));
        break;
      case 0x0F:
        Serial.println(F("FLAP: DOWN"));
        break;       
    }    

    // Check if the checksum matches
    byte checksum = 0x00;

    for (byte i = 0; i < 12; i++) {
      checksum ^= bytes[i];
    }

    checksum ^= bytes[12] & 0x0F;
    checksum ^= (checksum >> 4);
    checksum &= 0x0F;

    Serial.print(F("Checksum '0x"));
    Serial.print(checksum, HEX);

    if ( ((bytes[12] & 0xF0) >> 4) == checksum ) {
      Serial.println(F("' matches"));
    } else {
      Serial.print(F(" does not match "));
      Serial.println(((bytes[12] & 0xF0) >> 4), HEX);
    }
    return true;
  }

  return false;
}
