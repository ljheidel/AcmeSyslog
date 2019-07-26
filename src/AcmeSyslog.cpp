/*
 *  AcmeSyslog.cpp - A multi-output Arduino system logging library for the ESP8266.
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

void AcmeSyslog::setLogLevel(int t, int s, int f) {
  serial_log_level = t;
  syslog_log_level = s;
  file_log_level = f;
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

int AcmeSyslog::geSyslogLogLevel() {
  return syslog_log_level;
}

void AcmeSyslog::setFileLogLevel(int t) {
  serial_file_level = f;
  return;
}

int AcmeSyslog::getFileLogLevel() {
  return file_log_level;
}

void AcmeSyslog::setLogTimestamp(bool t) {
  log_timestamp = t;
}

bool AcmeSyslog::getLogTimestamp() {
  return t;
}

void AcmeSyslog::setSerialSpeed(long s) {
  serial_speed = s;
}

long AcmeSyslog::getSerialSpeed() {
  return serial_speed;
}

void AcmeSyslog::init() {
  if (mode | USE_SERIAL) {
    init_serial(serial_speed);
  }
}

void AcmeSyslog::initSyslog() {
  if (mode | USE_SYSLOG) {
    syslog(syslogUDP, LOGHOST, 514, "tempsensor", "tempsensor", LOG_KERN);
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
  Serial.println("Serial logging initialized");
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

void AcmeSyslog::logf(int l, const char *fmt, ...) {
  char b[200];
  char tb[255];
  va_list args;

  va_start(args, fmt);
  vsprintf(b, fmt, args);
  va_end(args);

  log_timestamp ? sprintf(tb, "%i: %s\n", millis(), b) : sprintf(tb, "%s\n", b);
  
  if (mode | USE_SERIAL && l <= getSerialLogLevel() && serial_init) {  
    Serial.println(tb);
  }
  return;
}

