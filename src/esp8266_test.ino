#include "esp8266.h"
#include <SoftwareSerial.h>
#include <ArduinoUnit.h>
#include "HttpRequest.h"

SoftwareSerial mySerial(2,3); // RX, TX
Esp8266<SoftwareSerial> esp(mySerial);

test(basic_isOk_succeeds)
{
  bool ret = esp.isOk();
  assertTrue(ret);
}

test(basic_configureBaude_returnsCorrectResults)
{
  mySerial.end();
  unsigned baud = esp.configureBaud();

  mySerial.begin(9600);
  assertEqual(baud, 9600);
}

test(basic_setBaud_succeeds)
{
  // Set to new and back to old rate
  bool ret = esp.setBaud(19200);
  if (ret)
    esp.setBaud(9600);
  else
    esp.configureBaud();

  assertTrue(ret);
}


test(basic_isOk_withInvalidBaudSettingsFails)
{
  mySerial.begin(115200);
  bool ret = esp.isOk();
  mySerial.begin(9600);
  assertFalse(ret);
}

test (basic_setMultipleConnections_succeeds)
{
  esp.setMultipleConnections(false);

  bool ret = esp.setMultipleConnections(true);
  assertTrue(ret)
}

test (basic_getMultipleConnections_returnsCorrectResults)
{
  esp.setMultipleConnections(false);
  esp.setMultipleConnections(true);

  bool mux;
  bool ret = esp.getMultipleConnections(mux);
  assertTrue(ret);
  assertTrue(mux);
}

test (ap_joinAccessPoint_joinsCorrectly)
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


test (send_withGetSucceeds)
{
  // Build request
  HttpRequest req("/update");
  req.addParameter("api_key", "SSZQ72F4VTZW43YS");
  req.addParameter("field1", "1");

  // Set string
  char reqStr[200];
  req.get(reqStr);

  // Connect server
  esp.setMultipleConnections(true);
  esp.connect(1, "api.thingspeak.com", 80);
  bool ret = esp.send(1, reqStr, strlen(reqStr));

  // String retString = mySerial.readString();
  // Serial.printf("Ret string : \"%s\"", retString.c_str());

  assertTrue(ret);
}


void setup()
{
  Serial.begin(9600);

  esp.configureBaud();
  esp.setBaud(9600);
}

void loop()
{

  Test::run();
  // esp.queryMultipleConnections();
}
