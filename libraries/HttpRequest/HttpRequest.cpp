/**
 *  @file
 *  @brief Iplementation of a simple HTTP GET and POST string builder.
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

#include "HttpRequest.h"
#include <String.h>

static const char LF[] PROGMEM = "\r\n";
static const char GET[] PROGMEM = "GET ";
static const char POST[] PROGMEM = "POST ";
static const char HTTP[] PROGMEM = " HTTP/1.0";
static const char FORM_URLENCODED[] PROGMEM = "Content-Type: Application/x-www-form-urlencoded";
static const char QUESTION_MARK[] PROGMEM = "?";
static const char CONTENT_LENGTH[] PROGMEM = "Content-Length: ";

static void strcls(char *str)
{
  str[0] = 0;
}

HttpRequest::HttpRequest(const String &path)
{
  _path = path;
}

void HttpRequest::addParameter(const String &key, const String &value)
{
  // Add separator
  if (_request.length() != 0)
    _request += F("&");

  // Add parameter
  _request += key;
  _request += F("=");
  _request += value;
}

const void HttpRequest::get(char *ret) const
{
  if (!ret)
    return;

  strcls(ret);
  strcat_P(ret, GET);
  strcat(ret, _path.c_str());
  strcat_P(ret, QUESTION_MARK);
  strcat(ret, _request.c_str());
  strcat_P(ret, LF);
  strcat_P(ret, LF);
}

const void HttpRequest::post(char *ret) const
{
  if (!ret)
    return;

  char contentLength[10];
  snprintf(contentLength, 10, "%d", _request.length());

  // Clear string
  strcls(ret);

  // Post field
  strcat_P(ret, POST);
  strcat(ret, _path.c_str());
  strcat_P(ret, HTTP);
  strcat_P(ret, LF);

  // URL-encoded field
  strcat_P(ret, FORM_URLENCODED);
  strcat_P(ret, LF);

  // Content-Length field
  strcat_P(ret, CONTENT_LENGTH);
  strcat(ret, contentLength);
  strcat_P(ret, LF);
  strcat_P(ret, LF);

  // Query string
  strcat(ret, _request.c_str());
}
