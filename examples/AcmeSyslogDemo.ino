/*
 *  AcmeSyslogDemo.ino - A demo of an Arduino system logging library for the 
 *  ESP32/ESP8266.
 *  Author: Luke Heidelberger <ljheidel@gmail.com>
 *  Date:   27 Aug 2022
 *  
 *  This file is part of AcmeSyslog.
 *
 *  AcmeSyslog is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AcmeSyslog.  If not, see <https://www.gnu.org/licenses/>.
 *  
 */
 
#include <ESP8266WiFi.h>
#include <AcmeSyslog.h>
#include "AcmeSyslogDemo.h"

/*
 * Create an AcmeSyslog instance that uses serial logging and syslog.
 */

AcmeSyslog acme_syslog(USE_SERIAL | USE_SYSLOG);

void setup()
{
    char buf[80];
    Serial.begin(115200);
    delay(10);

    sprintf(buf, "\nConnecting to WiFi network %s...", OUR_SSID);
    Serial.print(buf);

    WiFi.begin(OUR_SSID, OUR_PASSPHRASE);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("\n");

    sprintf(buf, "WiFi connected.  Local IP is %i.%i.%i.%i\n",
      WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    Serial.print(buf);
    delay(100);
    /*
     * Initialize AcmeSyslog
     */
    acme_syslog.init();
    /*
     * Set the minimum log levels for each output
     */
    acme_syslog.setLogLevel(LOG_DEBUG);
    acme_syslog.setSyslogLogLevel(LOG_DEBUG);
    // acme_syslog.setCallbackLogLevel(LOG_INFO);
    /*
     * Set the syslog server
     */
    acme_syslog.setSyslogServer(OUR_LOGHOST, 1514);
    /*
     * Set your hostname for syslog
     */
    acme_syslog.setDeviceHostname("localhost");
    /*
     * The name of the app
     */
    acme_syslog.setAppName("acmesyslogdemo");
    /*
     * Turn it on.
     */
    acme_syslog.activateSyslog();

}

void loop()
{
  /*
   * Send a log message.
   */
  acme_syslog.logf(LOG_INFO, "Uptime is %i ms", millis());
  delay(15000);   
}
