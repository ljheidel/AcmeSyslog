#include "AcmeSyslog.h"
#include "creds.h"
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

AcmeSyslog a(USE_SERIAL | USE_FILE);

int i = 0;

void setup() {
  a.init();
  a.eraseFileLog();
  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    a.logf(LOG_INFO, "Connecting...");
  }

  a.logf(LOG_INFO, "WiFi Connected, IP Addr: %s", IpAddress2String(WiFi.localIP()).c_str());
  a.logf(LOG_INFO, "Hostname is %s", WiFi.hostname().c_str());

  a.setSyslogServer("192.168.109.1", 514);
  a.activateSyslog();

}

void loop() {
  a.logf(LOG_ERR, "loop %i", i++);
  if (i == 10) {
    a.dumpFileLog(); 
  }
  delay(1000);
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}
