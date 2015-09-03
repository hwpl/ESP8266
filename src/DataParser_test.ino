#include "ArduinoUnit.h"
#include "DataParser.h"

test (parser_parse_acceptsCorrectHeader)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("+IPD,1,5:...Data...");

  bool ret = parser.parse();

  assertTrue(ret);
}

test (parser_parse_acceptsCorrectHeaderWithJunk)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("A_lot_of_Junk\r\nJunk+IPD,1,5:...Data...");

  bool ret = parser.parse();

  assertTrue(ret);
}

test (parser_parse_deniesIncorrectHeader1)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\n+IPD,,5:False");

  bool ret = parser.parse();

  assertFalse(ret);
}

test (parser_parse_deniesIncorrectHeader2)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\n+IPD,15:False");

  bool ret = parser.parse();

  assertFalse(ret);
}

test (parser_parse_deniesIncorrectHeader3)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\nIPD,1,15:False");

  bool ret = parser.parse();

  assertFalse(ret);
}

test (parser_parse_deniesIncorrectHeader4)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\n++IPD,1,15:False");

  bool ret = parser.parse();

  assertFalse(ret);
}

test (parser_parse_deniesIncorrectHeader5)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\n+IPD,1,15False");

  bool ret = parser.parse();

  assertFalse(ret);
}

test (parser_parse_doeNotAcceptNegativeNumbers1)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\n+IPD,-1,15:False");

  bool ret = parser.parse();

  assertFalse(ret);
}

test (parser_parse_doeNotAcceptNegativeNumbers2)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\n+IPD,1,-15:False");

  bool ret = parser.parse();

  assertFalse(ret);
}

test (parser_parse_leavesFirstByteAfterHeaderOnStream)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\n+IPD,1,6:First Byte");

  bool ret = parser.parse();

  assertTrue(ret);
  assertEqual(stream.read(), 'F');
}
