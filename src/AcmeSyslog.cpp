/*
 *  AcmeSyslog.cpp - A multi-output Arduino system logging library for the ESP8266.
 *  Author: Luke Heidelberger <ljheidel@gmail.com>
 *  Date:   27 Jul 2019
 *  
 *  This file is part of AcmeSyslog.
 *
 *  Foobar is free software: you can redistribute it and/or modify
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
 *  along with AcmeLogger.  If not, see <https://www.gnu.org/licenses/>.
 *  
 */

#include "AcmeSyslog.h"

int mode = USE_SERIAL;
int serial_log_level = LOG_LEVEL_DEFAULT;
int syslog_log_level = LOG_LEVEL_DEFAULT;
int file_log_level = LOG_LEVEL_DEFAULT;

bool serial_init = false;
bool syslog_init = false;
bool file_init = false;

int serial_speed = SERIAL_SPEED_DEFAULT;
bool log_timestamp = LOG_TIMESTAMP_DEFAULT;

String app_name, device_hostname;

File syslog_file;

WiFiUDP udp_client;
Syslog syslog(udp_client, SYSLOG_PROTO_IETF);

String syslog_filename = DEFAULT_SYSLOG_FILENAME;

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

void AcmeSyslog::setAppName(String a) {
  app_name = a;
  syslog.appName(a.c_str());
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

void AcmeSyslog::configSyslog(String s, int p, String h, String a, int dp){
  setSyslogServer(s, p);
  setDeviceHostname(h);
  setAppName(a);
  setSyslogDefaultPriority(dp);
}

void AcmeSyslog::setSyslogServer(String s, int p) {
  syslog.server(s.c_str(), p);
}

void AcmeSyslog::setSyslogDefaultPriority(int dp) {
  syslog.defaultPriority(dp);
}

void AcmeSyslog::setDeviceHostname(String h) {
  syslog.deviceHostname(h.c_str());
  device_hostname = h;
}

void AcmeSyslog::init() {

  setAppName(DEFAULT_APP_NAME);
  setDeviceHostname(DEFAULT_HOSTNAME);
  
  if (getMode() | USE_SERIAL) {
   initSerial(getSerialSpeed());
  }

  if (getMode() | USE_FILE) {
    initFile();
  }
}

void AcmeSyslog::initSyslog() {
  if (getMode() | USE_SYSLOG) {

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
    serial_init = true;
    Serial.println("Serial logging initialized");
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

String AcmeSyslog::toDigits(long d) {
  String r;
  if (d < 10) {
    r = "0";
    r += (String)d;
  } else {
    r = (String)d;
  }
  return r;
}

void AcmeSyslog::logf(int l, const char *fmt, ...) {
  char b[200];
  char serial_buf[255];
  va_list args;

  va_start(args, fmt);
  vsprintf(b, fmt, args);
  va_end(args);

  log_timestamp ? sprintf(serial_buf, "%s %s: %s", formatTimestamp(now()).c_str(), getAppName().c_str(), b) : sprintf(serial_buf, "%s", b);
  
  if (mode | USE_SERIAL && l <= getSerialLogLevel() && serial_init) {  
    Serial.println(serial_buf);
  }

  if (mode | USE_FILE && l <= getFileLogLevel() && file_init) {
    syslog_file.println(serial_buf);
  }

  if (mode | USE_SYSLOG) {
    syslog.log(getSyslogLogLevel(), b);
  }
  
  return;
}

