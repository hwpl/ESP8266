#ifndef __ESP8266_H__
#define __ESP8266_H__

#include <Stream.h>
#include "DataParser.h"

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

  Esp8266(T &serial) : _serial(serial), _parser(serial)
  {
    setTimeout(DEFAULT_TIMEOUT);
  };

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
    * @note Command: AT+CIOBAUD=<baud>
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
    * @note: Command: AT+CWJAP=<ssid>,<passwd>
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
    * Sends data over the connection channel to the server.
    *
    * @note Command: AT+CIPSEND=<id>,<length>\r\n ... <bytes>
    * @param channelId The channel to send the data.
    * @param bytes The buffer to send.
    * @param length The length of the buffer
    * @return Returns "true" if the command was successful, "false" otherwise.
    */
   bool send(unsigned char channelId, const char *bytes, const unsigned length) const;

   unsigned int receive(unsigned char channelId, char *buffer, unsigned int bufferSize);

private:
  // Variables
  T &_serial;
  DataParser _parser;
  unsigned int _receivedBytes;
  unsigned char _receivedChannelID;

  // Command Helpers
  void sendCommand(const String &command) const;
  const String readReply(unsigned long timeout = DEFAULT_TIMEOUT) const;
  bool findAnswer(char *anser, unsigned long timeout = DEFAULT_TIMEOUT) const;
  bool wasCommandSuccessful(unsigned long timeout = DEFAULT_TIMEOUT) const;

  // Stream Helper
  void setTimeout(unsigned int timout) const;
  void flush() const;
  void flushIn() const;
  void flushOut() const;
};

#endif // __ESP8266_H__
