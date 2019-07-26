#include "AcmeSyslog.h"

AcmeSyslog a;

int i = 0;

void setup() {
  a.init();

}

void loop() {
  a.logMsg("test");
  a.logf(LOG_WARNING, "loop %i", i++); 
  delay(1000);
}
