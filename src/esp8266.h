#include <Stream.h>

template <class T>
class Esp8266
{
public:
  static const unsigned int DEFAULT_TIMEOUT =  1000;
  static const unsigned int SHORT_TIMEOUT   =  1000;
  static const unsigned int MEDIUM_TIMEOUT  =  2000;
  static const unsigned int LONG_TIMEOUT    = 10000;

  typedef enum {
    UDP,
    TCP
  } ProtocolMode;

  Esp8266(T &serial) : serial(serial) {
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
   * @note: No command issued. Use updateMultipleConnection to query device.
   * @return The multiple connection state. "true" = enabled, "false" = disabled.
   */
  bool getMultipleConnections() const;

  /**
   * Queries the multiple connection state.
   *
   * @note: Command: AT+CIPMUX?
   * @note: The internal mux state is updated.
   * @return Returns "true" if the command was successful, "false" otherwise.
   */
   bool queryMultipleConnections();

   /**
    * Joins the given access point
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
   bool connect(unsigned channelId, const String &addr, unsigned int port, ProtocolMode mode = TCP);

   /**
    * Diconnects a channel to a server.
    * @param channelId The channel to disconnect
    * @return Returns "true" if the command was successful, "false" otherwise.
    */
   bool disconnect(unsigned channelId);

private:
  T &serial;
  bool multipleConnections;
  void sendCommand(const String &command) const;
  const String readReply(unsigned int timeout = DEFAULT_TIMEOUT) const;
  bool wasCommandSuccessful(unsigned int timeout = DEFAULT_TIMEOUT) const;
  void setTimeout(unsigned int timout) const;

};
