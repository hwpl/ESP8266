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

#include "HttpRequest.h"
#include <String.h>

static const char *lf = "\r\n";

HttpRequest::HttpRequest(const String &path)
{
  _path = path;
  _parameterAdded = false;
}

void HttpRequest::addParameter(const String &key, const String &value)
{
  if (!_parameterAdded)
    _parameterAdded = true;
  else
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

  ret[0] = 0;
  strcat(ret, "GET ");
  strcat(ret, _path.c_str());
  strcat(ret, "?");
  strcat(ret, _request.c_str());
  strcat(ret, lf);
  strcat(ret, lf);
}

const void HttpRequest::post(char *ret) const
{
  if (!ret)
    return;

  char contentLength[10];
  snprintf(contentLength, 10, "%d", _request.length());

  ret[0] = 0;
  strcat(ret, "POST ");
  strcat(ret, _path.c_str());
  strcat(ret, " HTTP/1.1");
  strcat(ret, lf);
  strcat(ret, "Content-Type: application/x-www-form-urlencoded");
  strcat(ret, lf);
  strcat(ret, "Content-Length: ");
  strcat(ret, contentLength);
  strcat(ret, lf);
  strcat(ret, lf);
  strcat(ret, _request.c_str());


/*
  String retStr;
  retStr += F("POST ");
  retStr += _path;
  retStr += F(" HTTP/1.1");
  retStr += lf;

  retStr += F("Content-Type: application/x-www-form-urlencoded");
  retStr += lf;

  retStr += F("Content-Length: ");
  retStr += _request.length();
  retStr += lf + lf;
  retStr += _request;
  retStr += lf;

  return retStr;

  */
}
