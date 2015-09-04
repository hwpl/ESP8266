// The MIT License (MIT)
//
// Copyright (c) 2015 Joern Hoffmann
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Joern Hoffmann, 2015
// University Leipzig    jhoffmann@informatik.uni-leipzig.de
// xceeth Technologies   j.hoffmann@xceeth.com

#include <SoftwareSerial.h>
#include <ArduinoUnit.h>

#include <HttpRequest.h>
#include <Esp8266.h>
#include <IPDParser.h>

SoftwareSerial mySerial(2,3); // RX, TX
Esp8266<SoftwareSerial> esp(mySerial);

// -------------------------------------------------------------------------- //
// Helper
// -------------------------------------------------------------------------- //

// Send a request on a channel to the thingspeak api.
bool connectAndSendGetRequest(unsigned char channelId)
{
  // Build HTTP request
  HttpRequest req(F("/update"));
  req.addParameter(F("api_key"), F("SSZQ72F4VTZW43YS"));
  req.addParameter(F("field1"), F("20"));

  // Build get request string
  unsigned bufferSize = 64;
  char buffer[bufferSize];
  req.get(buffer);

  // Connect to server and send data
  esp.setMultipleConnections(true);
  esp.disconnect(channelId);
  bool ret = esp.connect(channelId, F("api.thingspeak.com"), 80);

  // Send request
  if (ret)
    ret = esp.send(channelId, buffer, strlen(buffer));

  return ret;
}

// -------------------------------------------------------------------------- //
// Tests
// -------------------------------------------------------------------------- //
/*
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

  bool ret = esp.joinAccessPoint(F("ti_iot"), F("ti_iot42!"));
  assertTrue (ret);
}

test (connect_viaTCPWithMultipleConnectionSucceeds)
{
  esp.setMultipleConnections(true);
  esp.disconnect(1);

  bool ret = esp.connect(1, F("google.de"), 80);

  esp.disconnect(1);
  assertTrue(ret);
}

test (connect_viaTCPWithoutMultipleConnectionsFails)
{
  esp.setMultipleConnections(false);
  esp.disconnect(1);

  bool ret = esp.connect(1, F("google.de"), 80);

  esp.disconnect(1);
  assertFalse(ret);
}

test (send_withGetSucceeds)
{
  bool ret = connectAndSendGetRequest(1);
  assertTrue(ret);
}

test (receive_correctlyReceivesOneSingleAnswer)
{
  const unsigned bufferSize = 20;
  char buffer[bufferSize];
  assertTrue(connectAndSendGetRequest(1));

  unsigned int length = esp.receive(1, buffer, bufferSize);

  // Check for an non empty answer
  assertMore(length, 0);
}

test (receive_doesNotReceiveWithWrongBufferPArameters)
{
  char buffer;
  assertTrue(connectAndSendGetRequest(1));

  // Receive data
  unsigned len1 = esp.receive(1, NULL, 1);
  unsigned len2 = esp.receive(1, &buffer, 0);

  // Check for an empty answer
  assertEqual(len1, 0);
  assertEqual(len2, 0);
}

test (receive_doesNotReceiveOnTheWrongChannelId)
{
  char buffer;
  assertTrue(connectAndSendGetRequest(1));

  unsigned len = esp.receive(2, &buffer, sizeof(buffer));

  assertEqual(len, 0);
}
*/

test (receive_correctlyReceivesString)
{
  assertTrue(connectAndSendGetRequest(1));

  // Parse received ip data until the payload appears.
  IPDParser ipd(mySerial);
  assertTrue(ipd.parse());

  // Receive string
  char buffer[20];
  buffer[0] = 0;
  ipd.readPayload(buffer, 20);

  assertMore(atoi(buffer), 0);
}

/*
test (receive_correctlyReceivesFakeString)
{
  FakeSerial fakeSerial;
  Esp8266<FakeSerial> myEsp(fakeSerial);
  fakeSerial.nextBytes("+IPD,1,5:Hello");

  char buffer[10];
  buffer[0] = 0;
  unsigned len = esp.receive(1, buffer, 10);

  Serial.printf("ret buffer : %s", buffer);

  // Check if the two strings are equal
  assertEqual(strcmp(buffer, "Hello") , 0);
}
*/

// -------------------------------------------------------------------------- //
// Main
// -------------------------------------------------------------------------- //
void setup()
{
  Serial.begin(9600);

  esp.configureBaud();
  esp.setBaud(9600);
}

void loop()
{
  Test::run();
}
