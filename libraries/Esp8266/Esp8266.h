/**
 *  @file
 *  @brief Implementation to control an Esp8266 module via serial commands.
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

#ifndef __ESP8266_H__
#define __ESP8266_H__

#include <Stream.h>

template <class T>
class Esp8266
{
public:
  static const unsigned long DEFAULT_TIMEOUT =  1000;  ///< Timeout to send simple commands to the module, e.g. isOK()
  static const unsigned long MEDIUM_TIMEOUT  =  5000;  ///< Timemout for medium lasting commands, e.g. connect()
  static const unsigned long LONG_TIMEOUT    = 10000;  ///< Timeout for long commenads, e.g. joinAccessPoint()

  typedef enum {
    TCP,            ///< Transmission control protocol for stateful communications (default)
    UDP             ///< User datagram protocol for connection-less communications
  } ProtocolMode;

  /**
   * Constructs an object to handle an ESP8266 module.
   * @param serial The serial interface to which the module is connected.
   */
  Esp8266(T &serial);

  /**
   * Checks the communication with the module.
   *
   * @note Command: AT
   * @return Returns "true" if the command was successful, "false" otherwise.
   */
  bool isOk() const;

  /**
   * Automatically probes and configures the BAUD rate of the module.
   * The following rates are probed:
   *  2400, 4800, 9600, 19200, 38400, 57600 and 115200.
   *
   * @note: The detected rate is also set to the serial stream.
   * @return The found BAUD rate of the module. 0 if the module does not answer.
   */
   unsigned long configureBaud() const;

   /**
    * Changes the baud rate of the pair: connection and module.
    * @note Command: AT+UART_CUR=<baud>,8,1,0,0
    * @parameter baud The new baud rate to set. The following rates are
    *   supported: 2400, 4800, 9600, 19200, 38400, 57600 and 115200.
    * @return True if the command was successful
    */
   bool setBaud(unsigned long baud) const;

  /**
   * Sets the multiple connection support of the module.
   *
   * @note Command: AT+CIPMUX=<value>
   * @param value True enables and false disables multiple connection support.
   * @return Returns "true" if the command was successful, "false" otherwise.
   */
  bool setMultipleConnections(bool value);

  /**
   * Returns the currently set multiple connection state.
   *
   * @note: Command: AT+CIPMUX?
   * @param multipleConnections Reference to store the state. "true" = enabled, "false" = disabled.
   * @return Returns "true" if the command was successful, "false" otherwise.
   */
  bool getMultipleConnections(bool &multipleConnections) const;

   /**
    * Joins the given access point.
    *
    * @note: Command: AT+CWMODE_CUR=1 and AT+CWJAP_CUR=<ssid>,<passwd>
    * @param ssid The ssid of the access point to join.
    * @param passwd The password to join the network.
    * @return Returns "true" if the command was successful, "false" otherwise.
    */
   bool joinAccessPoint(const String &ssid, const String &passwd) const;

   /**
    * Establishes a channel to a server.
    *
    * @note: Command: AT+CIPSTART=<id>,"[TCP,UPD]","<address>",port
    * @param channelId The channelId to distinguise between multiple connections.
    * @param addr The address of the server. Provide either an IP-Address or the DNS name of the server.
    * @param port The port of the service to connect with.
    * @param mode The connection mode. Currently TCP or UDP.
    * @return Returns "true" if the command was successful and the connection was established, "false" otherwise.
    */
   bool connect(unsigned channelId, const String &addr, unsigned int port, ProtocolMode mode = TCP) const;

   /**
    * Diconnects a channel to a server.
    *
    * @note Command: AT+CIPSTOP=<channelId>
    * @param channelId The channel to disconnect
    * @return Returns "true" if the command was successful, "false" otherwise.
    */
   bool disconnect(unsigned channelId) const;

   /**
    * Sends a buffer to the server.
    *
    * @note Command: AT+CIPSEND=<id>,<length>\r\n ... <bytes>
    * @param channelId The channel that is used to send the data.
    * @param bytes The buffer to send.
    * @param length The length of the buffer
    * @return Returns "true" if the command was successful, "false" otherwise.
    */
   bool send(unsigned char channelId, const char *bytes, const unsigned length) const;

   /**
    * Sends a string to the server.
    *
    * @note Command: AT+CIPSEND=<id>,<length>\r\n ... <bytes>
    * @param channelId The channel that is used to send the data.
    * @param string The string to send.
    * @return Returns "true" if the command was successful, "false" otherwise.
    */
   bool send(unsigned char channelId, const String &string) const;

private:
  // Serial Interface
  T &_serial;
  void setTimeout(unsigned int timout) const;
  void flush() const;
  void flushIn() const;
  void flushOut() const;

  // Commands
  void sendCommand(const String &command) const;
  const String readReply(unsigned long timeout = DEFAULT_TIMEOUT) const;
  bool findAnswer(char *anser, unsigned long timeout = DEFAULT_TIMEOUT) const;
  bool wasCommandSuccessful(unsigned long timeout = DEFAULT_TIMEOUT) const;
};

// Provide template definition
#include <utility/Esp8266.cpp>

#endif // __ESP8266_H__
