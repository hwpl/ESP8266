// The MIT License (MIT)
//
// Copyright (c) 2015 Joern Hoffmann
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Joern Hoffmann, 2015
// University Leipzig    jhoffmann@informatik.uni-leipzig.de
// xceeth Technologies   j.hoffmann@xceeth.com

/*
#include "ArduinoUnit.h"
#include "IPDParser.h"

test (parser_parse_acceptsCorrectHeader)
{
  FakeStreamBuffer stream;
  DataParser parser(stream);
  stream.nextBytes("\r\n+IPD,1,3:1001,CLOSED\r\n");

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
*/
