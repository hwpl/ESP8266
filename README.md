# ESP8266

Arduino library to support the Espressif ESP8266 WLAN module.

You need at least the [official firmware] version 2.0.0. This library is not yet stable, so the API may change at any time.

## Features

* Automatically set the baud rate of the serial interface
* Send basic AT commands to the module
* Connect to an access point
* Establish a TCP, UDP or TLS connection to a server
* Send and receive data from a server
* Make GET and POST HTTP requests

## Installation

Copy `Esp8266` and optionally `HttpRequest` from the `libraries` folder into your Arduino library path (eg. `$HOME/Arduino/libraries/`).

## Example

```cpp
#include <Esp8266.h>
#include <HttpRequest.h>
#include <IPDParser.h>

Esp8266<HardwareSerial> esp(Serial);

void setup() {
    esp.configureBaud();
    esp.setBaud(9600);

    esp.joinAccessPoint("ssid", "psk");
    esp.setMultipleConnections(true);
}

void loop() {
    HttpRequest req("/ping");
    req.addParameter(F("key"), "my-api-key");

    esp.connectSecure(1, F("api.myservice.test"));
    esp.send(1, req.post());

    IPDParser parser(Serial);
    if(parser.parse()) {
        doSomething(parser.getPayload());
    }
}
```

[official firmware]: http://www.electrodragon.com/w/File:V2.0_AT_Firmware(ESP).zip
