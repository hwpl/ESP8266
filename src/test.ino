#include "esp8266.h"
#include <SoftwareSerial.h>
#include <ArduinoUnit.h>

SoftwareSerial mySerial(3,2); // RX, TX
Esp8266<SoftwareSerial> esp(mySerial);

/*
test(isOk_succeeds)
{
  bool ret = esp.isOk();

  assertTrue(ret);
}

test(isOk_withInvalidBaudSettingsFails)
{
  mySerial.begin(115200);
  bool ret = esp.isOk();
  mySerial.begin(9600);

  assertFalse(ret);
}

test (setMultipleConnections_succeeds)
{
  bool ret = esp.setMultipleConnections(false);
  assertTrue(ret);

  ret = esp.setMultipleConnections(true);
  assertTrue(ret)
}

test (getMultipleConnections_returnsCorrectResults)
{
  esp.setMultipleConnections(true);
  bool ret = esp.getMultipleConnections();
  assertTrue(ret);
}

test (queryMultipleConnections_succeeds)
{
  bool ret = esp.queryMultipleConnections();
  assertTrue(ret);
}

test (queryMultipleConnections_updatesInternals)
{
  // Set multiple connections without using the interface
  esp.setMultipleConnections(0);
  mySerial.print(F("AT+CIPMUX=1\r\n"));

  esp.queryMultipleConnections();
  bool mux = esp.getMultipleConnections();
}

test (joinAccessPoint_joinsCorrectly)
{
  skip();
  return;
  bool ret = esp.joinAccessPoint("ti_iot", "ti_iot42!");
  assertTrue (ret);
}

test (connect_viaTCPWithMultipleConnectionSucceeds)
{
  esp.setMultipleConnections(true);
  esp.disconnect(1);

  bool ret = esp.connect(1, "google.de", 80);

  esp.disconnect(1);
  assertTrue(ret);
}

test (connect_viaTCPWithoutMultipleConnectionsFails)
{
  esp.setMultipleConnections(false);
  esp.disconnect(1);

  bool ret = esp.connect(1, "google.de", 80);

  esp.disconnect(1);
  assertFalse(ret);
}
*/

test (send_ByteDataSucceeds)
{
  esp.setMultipleConnections(true);
  esp.disconnect(1);
  esp.connect(1, "api.thingspeak.com", 80);

  String data("GET /update?api_key=SSZQ72F4VTZW43YS&field1=1024\r\n\r\n");
  bool ret = esp.send(1, data.c_str(), data.length());

  assertTrue(ret);

  mySerial.setTimeout(5000);
  String retString = mySerial.readString();
  Serial.printf("Ret string : \"%s\"", retString.c_str());
}

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop()
{

  Test::run();

  // esp.queryMultipleConnections();
}
