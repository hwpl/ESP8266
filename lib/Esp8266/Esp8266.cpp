#ifdef __ESP8266_H__

#define BAUD_MIN 2400
#define BAUD_MAX 115200

// -------------------------------------------------------------------------- //
// String Helpers
// -------------------------------------------------------------------------- //
static const String substringFromBeginUntilOccurenceOfString(const String &string, const String &untilString)
{
  int index = string.indexOf(untilString);
  if (index == -1 || index == 0 || string.length() == 0)
    return String();

  return string.substring(0, index-1);
}

static const String substringAfterOccurenceOfStringToEnd(const String &string, const String &fromString)
{
  int index = string.indexOf(fromString);
  if (index == -1 || string.length() == 0 || fromString.length() == 0)
    return String();

  return string.substring(index+1);
}

static const String getAnswerSubstring(const String &string)
{
  String answer = substringFromBeginUntilOccurenceOfString(string, "\r\nOK\r\n");
  answer = substringAfterOccurenceOfStringToEnd(answer, ":");

  return answer;
}

static const String buildParameterList(const String &param)
{
  return param;
}

static const String buildParameterList(int param)
{
  return String(param);
}

template<typename ... Types>
static const String buildParameterList(const String &param, const Types &...rest)
{
  return param + F(",") + buildParameterList(rest...);
}

static const String quoteString(const String &string)
{
  static const String quote(F("\""));
  return String(quote + string + quote);
}

template <typename ... Types>
static const String buildSetCommand(const String &command, const Types &...parameters)
{
  String prefix(F("AT+"));
  prefix += command;
  prefix += F("=");
  return prefix + buildParameterList(parameters...);
}

// Deprecated, now we use the methods of the stream class, like findUntil() etc.
/*
static bool findStringBeforeTerminator(Stream &stream, const char *search_str, const char terminator, const size_t bufflen = 64)
{
  // Check if we have a valid string to compare
  if (!search_str || strlen(search_str) == 0)
    return true;

  // Create temporary buffer
  char buffer[bufflen+1];

  // Read stream until we detect the terminator
  size_t read_len = stream.readBytesUntil(terminator, buffer, bufflen);

  // Check if we have read bytes. Otherwise the timout occurs.
  if (read_len == 0)
    return false;

  // Terminate the read string
  buffer[read_len] = 0;

  Serial.printf("String received : \"%s\"\n", buffer);

  // Check if we can found the string
  if (strstr(buffer, search_str) != NULL)
    return true;

  return false;
}

template <class T>
bool Esp8266<T>::findAnswer(char *answer) const
{
  bool ret = false;
  unsigned long until  = millis() + 5000;

  do {
    ret = findStringBeforeTerminator(serial, answer, '\n');
    if (ret)
      break;
  } while (millis() <= until);

  return ret;
}
*/

// -------------------------------------------------------------------------- //
// Computational helpers
// -------------------------------------------------------------------------- //
static unsigned long nextBaud(unsigned long baud)
{
  if (baud == 38400)
    baud = 57600;
  else
    baud *= 2;

  if (baud > BAUD_MAX)
    baud = BAUD_MAX;

  return baud;
}

static bool isBaudRateSupported(unsigned long baud)
{
  unsigned long baudCompare = BAUD_MIN;
  while (baudCompare <= BAUD_MAX)
  {
    if (baud == baudCompare)
      return true;

    baudCompare = nextBaud(baudCompare);
  };

  return false;
}

// -------------------------------------------------------------------------- //
// Public
// -------------------------------------------------------------------------- //
template <class T>
bool Esp8266<T>::isOk() const
{
  sendCommand(F("AT"));
  return wasCommandSuccessful();
}

template <class T>
unsigned long Esp8266<T>::configureBaud() const
{
  unsigned long baud = BAUD_MIN;
  while (baud <= BAUD_MAX)
  {
    // Set new rate to stream
    _serial.begin(baud);

    // Check module reply, if it answers correctly return rate.
    if (isOk())
      return baud;

    // Continue
    baud = nextBaud(baud);
  }

  return 0;
}

template <class T>
bool Esp8266<T>::setBaud(unsigned long baud) const
{
  if (!isBaudRateSupported(baud))
    return false;

  // Send command
  String cmd = buildSetCommand(F("CIOBAUD"), baud);
  sendCommand(cmd);

  // Change baud, send some stuff and delete possible wrong characters
  _serial.begin(baud);
  isOk();
  isOk();
  flushIn();

  return isOk();
}

template <class T>
bool Esp8266<T>::setMultipleConnections(bool enable)
{
  String cmd = buildSetCommand(F("CIPMUX"), enable);
  sendCommand(cmd);

  return wasCommandSuccessful();
}

template <class T>
bool Esp8266<T>::getMultipleConnections(bool &multipleConnections) const
{
  sendCommand(F("AT+CIPMUX?"));

  // Get answer
  String reply = readReply();
  String answer = getAnswerSubstring(reply);
  if (answer.length() == 0)
    return false;

  // "Parse" answer
  // Todo: add checks if we have a value
  multipleConnections = answer.toInt();
  return true;
}

template <class T>
bool Esp8266<T>::joinAccessPoint(const String &ssid, const String &passwd) const
{
  String cmd = buildSetCommand(F("CWJAP"), quoteString(ssid), quoteString(passwd));
  sendCommand(cmd);

  return wasCommandSuccessful(LONG_TIMEOUT);
}

template <class T>
bool Esp8266<T>::connect(unsigned channelId, const String &addr, unsigned port, ProtocolMode mode) const
{
  String modeString = (mode == TCP ? F("TCP") : F("UDP"));
  String cmd = buildSetCommand(F("CIPSTART"), String(channelId), quoteString(modeString), quoteString(addr), port);

  sendCommand(cmd);

  return wasCommandSuccessful(MEDIUM_TIMEOUT);
}

template <class T>
bool Esp8266<T>::disconnect(unsigned channelId) const
{
  String cmd = buildSetCommand(F("CIPCLOSE"), channelId);
  sendCommand(cmd);

  return wasCommandSuccessful(MEDIUM_TIMEOUT);
}

template <class T>
bool Esp8266<T>::send(unsigned char channelId, const char *bytes, const unsigned length) const
{
  String cmd = buildSetCommand(F("CIPSEND"), String(channelId), length);
  sendCommand(cmd);

  // Is module ready to get data?
  if (!wasCommandSuccessful())
    return false;

  // Write data
  _serial.write(bytes, length);
  flushOut();

  return wasCommandSuccessful();
}

template <class T>
unsigned int Esp8266<T>::receive(unsigned char channelId, char *buffer, unsigned int bufferSize)
{
  if (!buffer || bufferSize == 0)
    return 0;

  // Parse a possible answer of the module
  parseIPData();

  // If we are here, we work with previously received IP data or
  // with the parsed IP data above.
  // Check data
  if (_receivedBytes == 0 || channelId != _receivedChannelID)
    return 0;

  // Reads the minimum allowed count of bytes and writes it to the receiving buffer
  unsigned int count = min(bufferSize, _receivedBytes);
  count = _serial.readBytes(buffer, count);

  // Update the remaining bytes
  if (_receivedBytes >= count)
    _receivedBytes -= count;
  else
  {
    // Should not happen
    _receivedBytes = count = 0;
    Serial.print("Here 2");
  }

  return count;
}

// -------------------------------------------------------------------------- //
// Private
// -------------------------------------------------------------------------- //
// Byte-wise parses the answer of the module to identify requested IP data (+IPD)
template <class T>
void Esp8266<T>::parseIPData()
{
 if (_receivedBytes != 0)
   return;

 if (!_parser.parse())
  return;

  _receivedBytes = _parser.getPayloadLength();
  _receivedChannelID = _parser.getChannelId();
}

/**
 * Reads the reply of an AT-command.
 *
 * @param timout The maximum time to wait to fill the reply string.
 * return The string representation of the reply.
 */
template <class T>
const String Esp8266<T>::readReply(unsigned long timeout) const
{
  // TODO: reimplement with finer timeout control.

  if (timeout == DEFAULT_TIMEOUT)
    return _serial.readString();

  setTimeout(timeout);
  String retString = _serial.readString();
  setTimeout(DEFAULT_TIMEOUT);
  return retString;
}

/**
 * Scans the stream for the given AT answer.
 *
 * @note The method doesn't create a string. It just reacts on the terminal
 * substring '\r\n' of an AT command and scans the string before it.
 *
 * @parameter answer The answer to search befor the
 * @paramter timeout The absolute timeout to wait for the answer.
 * @return True if the answer string was found in the reply, false otherwise.
 */
template <class T>
bool Esp8266<T>::findAnswer(char *answer, unsigned long timeout) const
{
  unsigned long until = millis() + timeout;

  // Read until answer was parsed or timout occurs.
  do {
    if (_serial.findUntil(answer, "\r\n"))
      return true;
  } while (millis() <= until);

  return false;
}

/**
 * Checks if the answer of an AT-command was "OK".
 *
 * @param timout The maximum time to wait for the answer.
 * @return Returns "true" if the AT command was successful.
 */
 template <class T>
 bool Esp8266<T>::wasCommandSuccessful(unsigned long timeout) const
 {
   return findAnswer("OK", timeout);
 }

/// Sends an command with the tailing line feed of AT-commands
template <class T>
void Esp8266<T>::sendCommand(const String &command) const
{
  flushIn();
  _serial.print(command);
  _serial.print(F("\r\n"));
  flushOut();
}

/// Sets the timeout to read strings from the input stream
template <class T>
void Esp8266<T>::setTimeout(unsigned timeout) const
{
  _serial.setTimeout(timeout);
}

/// Cleans the output stream
template <class T>
void Esp8266<T>::flushOut() const
{
  _serial.flush();
}

/// Clean the input stream
template <class T>
void Esp8266<T>::flushIn() const
{
  while (_serial.available()) {
    _serial.read();
  }
}

/// Cleans the input and output buffer of the stream
template <class T>
void Esp8266<T>::flush() const
{
  flushOut();
  flushIn();
}

#undef BAUD_MIN
#undef BAUD_MAX

#endif
