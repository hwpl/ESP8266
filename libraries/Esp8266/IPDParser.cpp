/**
 *  @file
 *  @brief IP data header parser (Esp8266 module)
 *  @author Joern Hoffmann <jhoffmann@informatik.uni-leipzig.de>
 *  @author Joern Hoffmann <j.hoffmann@xceeth.com>
 *  @version 1.0
 *
 *  @section LICENSE
 *
 *  The MIT License (MIT)
 *  Copyright (c) 2015 Joern Hoffmann
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "TimeHelper.h"
#include "IPDParser.h"

// Non-terminal symbols
static const char PLUS = '+';
static const char COMMA = ',';
static const char COLON = ':';
// -------------------------------------------------------------------------- //
// Public
// -------------------------------------------------------------------------- //
IPDParser::IPDParser (Stream &stream) : _stream(stream)
{
  reset();
}

bool IPDParser::parse()
{
  reset();

  do {
    nextsym();
  } while(isJunk());

  if (header())
    return true;

  // cleanup
  reset();
  return false;
};

unsigned int IPDParser::getPayloadLength() const
{
  return _payloadLength;
}

bool IPDParser::reducePayloadLength(unsigned int amount)
{
  if (_payloadLength < amount)
    return false;

  _payloadLength -= amount;
  return true;
}

unsigned int IPDParser::getChannelId() const
{
  return _channelId;
}

unsigned int IPDParser::readPayload(char *buffer, unsigned int bufferSize)
{
  if (!buffer || bufferSize == 0 || _payloadLength == 0)
    return 0;

  // Reads the minimum allowed count of bytes and writes it to the receiving buffer
  unsigned int bytesToRead = min(bufferSize, _payloadLength);
  unsigned int readBytes   = _stream.readBytes(buffer, bytesToRead);

  // Update the remaining bytes
  if (!reducePayloadLength(readBytes))  {
    // We should not run into here.
    readBytes = 0;
    reset();
  }

  return readBytes;
}

void IPDParser::reset()
{
  symbol = -1;
  _channelId = 0;
  _payloadLength = 0;
}

// -------------------------------------------------------------------------- //
// Helper
// -------------------------------------------------------------------------- //
// Checks if the value of c is between 0..9
static bool isDigit(char c)
{
  return (c >= 0 && c <= 9);
}

// Checks if the calue of c represents an ASCII encoded digit
static bool isAsciiDigit(char c)
{
  return isDigit(c - '0');
}

// Ascii digit character to numerical representation between 0..9
static char char_to_digit(char c)
{
  // Substract the ASCII value to get the digit
  c -= '0';

  if (isDigit(c))
    return c;

  return -1;
}

// -------------------------------------------------------------------------- //
// Private
// -------------------------------------------------------------------------- //
bool IPDParser::parseUInt(unsigned int &number)
{
  unsigned int convertedNumber = 0;
  unsigned int parsedSymbols = 0;

  do {
    char digit = char_to_digit(symbol);
    if (digit == -1)
      break;

    convertedNumber *= 10;
    convertedNumber += digit;
    parsedSymbols++;

    nextsym();
  } while(true);

  if (!parsedSymbols)
    return false;

  number = convertedNumber;
  return true;
}

// Payload length conversion
bool IPDParser::payload()
{
  if (parseUInt(_payloadLength))
    return true;

  return false;
}

// Channel id char to number conversion
bool IPDParser::channel()
{
  // Check and store the channel number
  char digit = char_to_digit(symbol);
  if(isDigit(digit)) {
    _channelId = digit;
    nextsym();
    return true;
  }

  return false;
}

/*
 *  Header := IPD,<channel_id>,<length>:
 * e.g.: IPD,1,123:
 */
bool IPDParser::header()
{
  if (!expect(PLUS))
    return false;

  if (!expect('I'))
    return false;

  if (!expect('P'))
    return false;

  if (!expect('D'))
    return false;

  if (!expect(COMMA))
    return false;

  // Parse the the cannel number
  if (!channel())
    return false;

  if (!expect(COMMA))
    return false;

  // Parse the payload length
  if(!payload())
    return false;

  if (symbol == COLON)
    return true;

  return false;
}

// All symbols except -1 or '+'
bool IPDParser::isJunk()
{
  if (symbol == -1)
    return false;

  else if(symbol == PLUS)
    return false;

  return true;
}

bool IPDParser::accept(const char s)
{
  if (symbol == s) {
      nextsym();
      return true;
  }

  return false;
}

bool IPDParser::expect(const char s)
{
  if (accept(s))
    return true;

  return false;
}

void IPDParser::nextsym()
{
  // Wait until a byte is available or the timout occurs
  unsigned long until = millis() + 1000;
  while( !_stream.available() && isFuture(until));
    ;

  symbol = _stream.read();
}
