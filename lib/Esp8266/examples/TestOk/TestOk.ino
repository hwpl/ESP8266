#include <Esp8266.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);
Esp8266<SoftwareSerial> esp(mySerial);

void setup()
{
  Serial.begin(9600);

  // Wait for serial interface of the Aruino Leonardo and Micro.
  while(!Serial)
    ;

  // Automatically detect and set baud rate of the module.
  // In addition the baud rate of the mySerial interface is set.
  Serial.print(F("Detecting the WiFi module ...\n"));
  esp.configureBaud();

  if(esp.isOk())
    Serial.print(F("  the module answers.\n"));
  else
    Serial.print(F("  the module does not answer.\n"));
}

void loop()
{ }
