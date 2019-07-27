/*
 *  AcmeSyslog.h
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

#ifndef AcmeSyslog_h
#define AcmeSyslog_h

#define USE_SERIAL 0x1
#define USE_FILE 0x2
#define USE_SYSLOG 0x4

#define DEFAULT_SYSLOG_FILENAME "/log/syslog"

#define SERIAL_SPEED_DEFAULT 115200
#define LOG_LEVEL_DEFAULT LOG_INFO
#define LOG_TIMESTAMP_DEFAULT true

#define DEFAULT_APP_NAME "sketch"
#define DEFAULT_HOSTNAME "esp8266"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Syslog.h>
#include <FS.h>
#include <stdarg.h>
#include <Time.h>

class AcmeSyslog {
  public:
    AcmeSyslog();
    AcmeSyslog(int m);
    void setMode(int m); 
    int getMode(); 
    void setService(String s);
    String getService();
    void setLogLevel(int t, int s, int f);
    void setLogLevel(int l);
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
    void setSyslogServer(String s, int p);
    void setDeviceHostname(String h);
    String getDeviceHostname();
    void setAppName(String a);
    String getAppName();
    void configSyslog(String s, int p, String h, String a, int dp);
    void setSyslogDefaultPriority(int db); 
    void init();
    void initSyslog();
    void configSyslog();
    void initFile();
    void dumpFileLog();
    void eraseFileLog(); 
    void initSerial(int s); 
    void logMsg(String s);
    void logMsg(int l, String s);
    void logf(int l, const char *fmt, ...);
  private:
    String formatTimestamp(time_t t);
    String toDigits(long d);
/*    WiFiUDP syslogUDP;
    Syslog syslog; */
};

#endif
