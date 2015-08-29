#include "esp8266.h"
#include <String.h>
#include <SoftwareSerial.h>
#include <Arduino.h>

// -------------------------------------------------------------------------- //
// String Helpers
// -------------------------------------------------------------------------- //
static const bool isReplyOK(const String &reply)
{
  return reply.endsWith(F("OK\r\n"));
}

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

static const String buildParameterList()
{
  return String();
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
bool Esp8266<T>::setMultipleConnections(bool enable)
{
  String cmd = buildSetCommand("CIPMUX", enable);
  sendCommand(cmd);

  if (wasCommandSuccessful()) {
    multipleConnections = enable;
    return true;
  }

  return false;
}

template <class T>
bool Esp8266<T>::getMultipleConnections() const
{
  return multipleConnections;
}

template <class T>
bool Esp8266<T>::queryMultipleConnections()
{
  sendCommand(F("AT+CIPMUX?"));

  String reply = readReply();

  // Get answer
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
bool Esp8266<T>::connect(unsigned channelId, const String &addr, unsigned port, ProtocolMode mode)
{
  if (!multipleConnections)
    return false;

  String modeString = (mode == TCP ? F("TCP") : F("UDP"));
  String cmd = buildSetCommand(F("CIPSTART"), String(channelId), quoteString(modeString), quoteString(addr), port);
  sendCommand(cmd);

  return wasCommandSuccessful(MEDIUM_TIMEOUT);
}

template <class T>
bool Esp8266<T>::disconnect(unsigned channelId)
{
  if (!multipleConnections)
    return false;

  String cmd = buildSetCommand(F("CIPCLOSE"), channelId);
  sendCommand(cmd);

  return wasCommandSuccessful(MEDIUM_TIMEOUT);
}

// -------------------------------------------------------------------------- //
// Private
// -------------------------------------------------------------------------- //
template <class T>
const String Esp8266<T>::readReply(unsigned timeout) const
{
  if (timeout == DEFAULT_TIMEOUT)
    return serial.readString();

  setTimeout(timeout);
  String retString = serial.readString();
  setTimeout(DEFAULT_TIMEOUT);
  return retString;
}

template <class T>
bool Esp8266<T>::wasCommandSuccessful(unsigned timeout) const
{
  return isReplyOK(readReply(timeout));
}

template <class T>
void Esp8266<T>::sendCommand(const String &command) const
{
  serial.print(command);
  serial.print(F("\r\n"));
}

template <class T>
void Esp8266<T>::setTimeout(unsigned timeout) const
{
  serial.setTimeout(timeout);
}

template class Esp8266<SoftwareSerial>;
