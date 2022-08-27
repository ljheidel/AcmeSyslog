# AcmeSyslog

A multi-output Arduino system logging library for the ESP8266/ESP32.

This is a library which provides the ability to log simultaneously to the
serial console and Syslog using standard syslog format.  And arbitrary callback
is also supported to allow a developer to send logs to a function of their
choosing.

Local logging into a SPIFFS (or LittleFS) filesystem is not fully implemented.
Contributions are desired.

There is a dependency on the arcao/Syslog library that should be removed
at some point in the future.  However, thanks to the author!

The library offers quite a few methods with mostly self-explanatory names.

PROTIP:  Set your RTC for sweet timestamps

    Constructors:
    AcmeSyslog();
    AcmeSyslog(int m);

    Public methods:
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
    void setCallbackLogLevel(int f);
    int getCallbackLogLevel();
    void setLogTimestamp(bool t); 
    bool getLogTimestamp(); 
    void setSerialSpeed(long s); 
    long getSerialSpeed();
    void setSyslogServer(const char* c, uint16_t p);
    void setSyslogServer(IPAddress i, uint16_t p);
    void setDeviceHostname(const char *h);
    String getDeviceHostname();
    void setAppName(const char* a);
    String getAppName();
    void configSyslog(const char* c, uint16_t p, const char* h, const char* a, int dp);
    void setSyslogDefaultPriority(int db); 
    void init();
    void setSyslogActive(bool a);
    bool getSyslogActive();
    void activateSyslog();
    void initSyslog();
    void configSyslog();
    void initFile();
    void dumpFileLog();
    void eraseFileLog(); 
    void initSerial(int s); 
    void logMsg(String s);
    void logMsg(int l, String s);
    void logf(int l, const char *fmt, ...);
    void setCallback(void f(char *));

Version 0.3.1
