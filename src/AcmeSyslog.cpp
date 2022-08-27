/*
 *  AcmeSyslog.cpp - A multi-output Arduino system logging library for the
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

#include "AcmeSyslog.h"

int mode = USE_SERIAL;
int serial_log_level = LOG_LEVEL_DEFAULT;
int syslog_log_level = LOG_LEVEL_DEFAULT;
int file_log_level = LOG_LEVEL_DEFAULT;
int callback_log_level = LOG_LEVEL_DEFAULT;

bool serial_init = false;
bool syslog_active = false;
bool file_init = false;

int serial_speed = SERIAL_SPEED_DEFAULT;
bool log_timestamp = LOG_TIMESTAMP_DEFAULT;

String app_name, device_hostname;

String short_months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

File syslog_file;

WiFiUDP udp_client;
Syslog syslog(udp_client, SYSLOG_PROTO_BSD);

String syslog_filename = DEFAULT_SYSLOG_FILENAME;

void (*loggerCallback)(char *) = NULL;

AcmeSyslog::AcmeSyslog() {

}

AcmeSyslog::AcmeSyslog(int m) {
  setMode(m);
}

void AcmeSyslog::setMode(int m) {
  mode = m;
}

int AcmeSyslog::getMode() {
  return mode;
}

void AcmeSyslog::setAppName(const char* a) {
  app_name = String(a);
  syslog.appName(a);
}

String AcmeSyslog::getAppName() {
  return app_name;
}

void AcmeSyslog::setLogLevel(int t, int s, int f) {
  serial_log_level = t;
  syslog_log_level = s;
  file_log_level = f;
}

void AcmeSyslog::setLogLevel(int l) {
  serial_log_level = l;
  syslog_log_level = l;
  file_log_level = l;
}

void AcmeSyslog::setSerialLogLevel(int t) {
  serial_log_level = t;
  return;
}

int AcmeSyslog::getSerialLogLevel() {
  return serial_log_level;
}

void AcmeSyslog::setSyslogLogLevel(int s) {
  syslog_log_level = s;
  return;
}

int AcmeSyslog::getSyslogLogLevel() {
  return syslog_log_level;
}

void AcmeSyslog::setFileLogLevel(int f) {
  file_log_level = f;
  return;
}

int AcmeSyslog::getFileLogLevel() {
  return file_log_level;
}

void AcmeSyslog::setCallbackLogLevel(int f) {
  callback_log_level = f;
  return;
}

int AcmeSyslog::getCallbackLogLevel() {
  return callback_log_level;
}

void AcmeSyslog::setLogTimestamp(bool t) {
  log_timestamp = t;
}

bool AcmeSyslog::getLogTimestamp() {
  return log_timestamp;
}

void AcmeSyslog::setSerialSpeed(long s) {
  serial_speed = s;
}

long AcmeSyslog::getSerialSpeed() {
  return serial_speed;
}

void AcmeSyslog::configSyslog(const char* c, uint16_t p, const char* h, const char* a, int dp){
  setSyslogServer(c, p);
  setDeviceHostname(h);
  setAppName(a);
  setSyslogDefaultPriority(dp);
}

void AcmeSyslog::setSyslogActive(bool a) {
  syslog_active = a;
}

bool AcmeSyslog::getSyslogActive() {
  return syslog_active;
}

void AcmeSyslog::activateSyslog() {
  setSyslogActive(true);
}

void AcmeSyslog::setSyslogServer(const char* c, uint16_t p) {
  syslog.server(c, p);
}

void AcmeSyslog::setSyslogServer(IPAddress i, uint16_t p) {
  syslog.server(i, p);
}

void AcmeSyslog::setSyslogDefaultPriority(int dp) {
  syslog.defaultPriority(dp);
}

void AcmeSyslog::setDeviceHostname(const char *h) {
  syslog.deviceHostname(h);
  device_hostname = String(h);
}

void AcmeSyslog::init() {

  setSyslogActive(false);
  setAppName(DEFAULT_APP_NAME);
  setSyslogDefaultPriority(DEFAULT_PRIORITY);
  
  if (getMode() & USE_SERIAL) {
   initSerial(getSerialSpeed());
  }

  if (getMode() & USE_FILE) {
    initFile();
  }
}

void AcmeSyslog::initSyslog() {
  if (getMode() & USE_SYSLOG) {

  }
  return;
}

void AcmeSyslog::initFile() {
  if (getMode() | USE_SYSLOG) {
    if (int status = SPIFFS.begin() && getMode()| USE_SERIAL) {
      Serial.println("File logging initalized");
    } else {
      Serial.println("Error opening SPIFFS"); 
    }
  }
  if (syslog_file = SPIFFS.open(syslog_filename, "a+")) {
    file_init = true;
    Serial.print("Opened ");
    Serial.print(syslog_filename);
    Serial.print(" (");
    Serial.print(syslog_file.size());
    Serial.println(" bytes)");
  } else {
    Serial.print("Couldn't open ");
    Serial.println(syslog_filename); 
  }

  return;
}

void AcmeSyslog::initSerial(int s) {
  if (! Serial) {
    Serial.begin(s);
  }  else {
    Serial.end();
    Serial.begin(s);
  }
  if (getMode() | USE_SERIAL) {
    char s[255];
    sprintf(s, "Serial logging initialized for AcmeSyslog instance at %08x", this);
    serial_init = true;
    Serial.println(s);
  }
  return;
}

void AcmeSyslog::logMsg(String s) {
  if (mode | USE_SERIAL) {
    Serial.println(s);
  }
  return;
}

void AcmeSyslog::logMsg(int l, String s) {
  if (l >= serial_log_level) {
    logMsg(s); 
  }
  return;
}

void AcmeSyslog::dumpFileLog() {
  if (File syslog_read = SPIFFS.open(syslog_filename, "r")) {
    while (syslog_read.available()) {
      Serial.write(syslog_read.read());
    }
    syslog_read.close();
  } else {
    Serial.print("couldn't open ");
    Serial.println(syslog_filename);
  }
}

void AcmeSyslog::eraseFileLog() {
  if (syslog_file) {
    syslog_file.close();
  } 
  if (SPIFFS.remove(syslog_filename)) {
    Serial.print("removed ");
    Serial.println(syslog_filename);
    initFile();
  } else {
    Serial.println("couldn't remove ");
    Serial.println(syslog_filename);
  }
}

/*
String AcmeSyslog::formatTimestamp(time_t t) {
  String buf;
  buf = monthShortStr(month(t));
  buf += " ";
  buf += day(t);
  buf += " ";
  buf += toDigits(hour(t));
  buf += ":";
  buf += toDigits(minute(t));
  buf += ":";
  buf += toDigits(second(t));
  return buf;
}
*/
String AcmeSyslog::toDigits(int i) {
  String r;
  if (i < 10) {
    r = "0";
    r += (String)i;
  } else {
    r = (String)i;
  }
  return r;
}

String AcmeSyslog::formatTimestamp(time_t t) {
  tm* dt = gmtime(&t);
  char buf[80];
  sprintf(buf, "%s %i %s:%s:%s", short_months[dt->tm_mon].c_str(), dt->tm_mday, toDigits(dt->tm_hour).c_str(), toDigits(dt->tm_min).c_str(), toDigits(dt->tm_sec).c_str());
  return String(buf);  
}

void AcmeSyslog::setCallback(void f(char *)) {
  loggerCallback = f;
}


void AcmeSyslog::logf(int l, const char *fmt, ...) {
  char b[200];
  char serial_buf[255];
  char callback_buf[255];
  char syslog_buf[255];
  va_list args;

  va_start(args, fmt);
  vsprintf(b, fmt, args);
  va_end(args);

  sprintf(syslog_buf, "%s", b);
  sprintf(callback_buf, "%s", b);

  time_t now = time(nullptr);

  log_timestamp ? sprintf(serial_buf, "%s %s: %s", formatTimestamp(now).c_str(), getAppName().c_str(), b) : sprintf(serial_buf, "%s", b);
  log_timestamp ? sprintf(callback_buf, "%s %s: %s", formatTimestamp(now).c_str(), getAppName().c_str(), b) : sprintf(callback_buf, "%s", b);
  
  if (mode & USE_SERIAL && l <= getSerialLogLevel() && serial_init) {  
    Serial.println(serial_buf);
  }

  if (mode & USE_FILE && l <= getFileLogLevel() && file_init) {
    syslog_file.println(serial_buf);
  }

  if (mode & USE_SYSLOG && syslog_active) {
    syslog.logf(getSyslogLogLevel(), "%s", syslog_buf);
  }

  if (mode & USE_CALLBACK && loggerCallback != NULL && l <= getCallbackLogLevel()) {
    loggerCallback(callback_buf);
  }
  
  return;
}
