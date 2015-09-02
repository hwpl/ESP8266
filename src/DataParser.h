#include <Arduino.h>
#include <Stream.h>

class DataParser
{
public:
  // Termina symbols
  static const char PLUS = '+';
  static const char COMMA = ',';
  static const char COLON = ':';

  DataParser (Stream &stream) : stream (stream) {
    reset();
  }

  bool parse()
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

  unsigned int getPayloadLength() const
  {
    return payloadLength;
  }

  unsigned int getChannelId() const
  {
    return channelId;
  }

  void reset()
  {
    symbol = -1;
    channelId = 0;
    payloadLength = 0;
  }

private:
  Stream &stream;
  char symbol;
  unsigned int channelId;
  unsigned int payloadLength;

  // Checks if the value of c is between 0..9
  bool isDigit(char c) const
  {
    return (c >= 0 && c <= 9);
  }

  // Checks if the calue of c represents an ASCII encoded digit
  bool isAsciiDigit(char c)
  {
    return isDigit(c - '0');
  }

  // Ascii digit character to numerical representation between 0..9
  char char_to_digit(char c) const
  {
    // Substract the ASCII value to get the digit
    c -= '0';

    if (isDigit(c))
      return c;

    return -1;
  }

  bool parseUInt(unsigned int &number)
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
  bool payload()
  {
    unsigned int number;
    if (parseUInt(number)) {
      payloadLength = number;
      return true;
    }

    return false;
  }

  // Channel id char to number conversion
  bool channel()
  {
    // Check and store the channel number
    char digit = char_to_digit(symbol);
    if(digit != -1) {
      channelId = digit;
      nextsym();
      return true;
    }

    return false;
  }

  /*
   *  Header := IPD,<channel_id>,<length>:
   * e.g.: IPD,1,123:
   */
  bool header()
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
  bool isJunk()
  {
    if (symbol == -1)
      return false;

    else if(symbol == PLUS)
      return false;

    return true;
  }

  bool accept(const char s)
  {
    if (symbol == s) {
        nextsym();
        return true;
    }

    return false;
  }

  bool expect(const char s)
  {
    if (accept(s))
    return true;

    Serial.print("expect: unexpected symbol");
    return false;
  }

  void nextsym() {
    symbol = stream.read();
  }

};
