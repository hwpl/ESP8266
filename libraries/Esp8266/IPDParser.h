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

#ifndef __IPDPARSER_H__
#define __IPDPARSER_H__

#include <Arduino.h>
#include <Stream.h>

class IPDParser
{
public:
  IPDParser (Stream &stream);

  /**
  * Parses the input stream for an IPD answer of an ESP8266 module.
  *
  * @note If the parser is successful, all bytes of the stream up to the first
  * payload byte are removed. Therefore one can read the payload.
  * @return Returns true if the IPD answer was successfully parsed.
  */
  bool parse();

  /**
  * Returns the length of payload that was parsed.
  */
  unsigned int getPayloadLength() const;

  /**
  * Reduces the remaining bytes to read by the given amount.
  * This method is particular useful, if the parser is used to manually fetch the
  * IP data from the stream. This has the benefit to reduce book kepping.
  *
  * @param amount The amout to reduce the paload length
  * @return Returns true if the operation was successful, this if the
  * payloadLength was greater than the amount.
  */
  bool reducePayloadLength(unsigned int amount);

  /**
   * Returns the parsed channelId
   * @note Can be used after a successful parse()
   *
   * @return The channel identification.
   */
  unsigned int getChannelId() const;

  /**
   * Reads the payload into a given buffer.
   *
   * @note The method can be called after an successful parse().
   * @param buffer The buffer to be filled with a stream of payload bytes.
   * @param bufferSize The size of the buffer. This is also the maximum amount
   * that can be read from the stream.
   * @return The amount of copied bytes into the buffer.
   */
  unsigned int readPayload(char *buffer, unsigned int bufferSize);

  /**
   *  Returns the payload as string
   *
   * @note Due to memory limitations, only use this method for small payloads.
   * Otherwise, a stack overflow may occur.
   * @return The payload as string.
   */
  String getPayload();

  /**
   * Resets the parser with all values
   */
  void reset();

private:
  Stream &_stream;
  unsigned int _channelId;
  unsigned int _payloadLength;

  // Terminal Symbols
  bool payload();
  bool channel();
  bool header();
  bool isJunk();

  // Non-Terminal Symbols
  char symbol;
  bool parseUInt(unsigned int &number);
  bool accept(const char s);
  bool expect(const char s);
  void nextsym();
};

#endif // __IPDPARSER_H__
