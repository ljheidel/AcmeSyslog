#include "AcmeSyslog.h"
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

AcmeSyslog a(USE_SERIAL | USE_FILE | USE_SYSLOG);

int i = 0;

void setup() {
  a.init();
  a.eraseFileLog();

  

}

void loop() {
  a.logf(LOG_WARNING, "loop %i", i++);
  if (i == 10) {
    a.dumpFileLog(); 
  }
  delay(1000);
}
