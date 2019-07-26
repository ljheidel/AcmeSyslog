/*
  AcmeSyslog.h - A multi-output Arduino system logging library for the ESP8266.
*/

#ifndef AcmeSyslog_h
#define AcmeSyslog_h

#define USE_SERIAL 0x1
#define USE_FILE 0x2
#define USE_SYSLOG 0x4

#define SERIAL_SPEED_DEFAULT 115200
#define LOG_LEVEL_DEFAULT LOG_INFO
#define LOG_TIMESTAMP_DEFAULT true
 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Syslog.h>
#include <FS.h>
#include <stdarg.h>

class AcmeSyslog {
  public:
    void setMode(int m); 
    int getMode(); 
    void setLogLevel(int t, int s, int f);
    void setSerialLogLevel(int t);
    int getSerialLogLevel();
    void setSyslogLogLevel(int s);
    int getSyslogLogLevel();
    void setFileLogLevel(int f);
    int getFileLogLevel();
    void setLogTimestamp(bool t); 
    bool getLogTimestamp(); 
    void setSerialSpeed(long s); 
    long getSerialSpeed(); 
    void init();
    void initSyslog(); 
    void initSerial(int s); 
    void logMsg(String s);
    void logMsg(int l, String s);
    void logf(int l, const char *fmt, ...);
  private:
    WiFiUDP syslogUDP;
    Syslog syslog; 
};

#endif
