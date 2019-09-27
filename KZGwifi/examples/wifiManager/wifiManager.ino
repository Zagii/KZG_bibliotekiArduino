#include <KZGwifi.h>

#include "KZGwifi.h"

KZGwifi kzgWifi;

void setup()
{
  Serial.begin(115200);
  kzgWifi.begin("KZGtest");
}

void loop()
{
  kzgWifi.loop();
}
