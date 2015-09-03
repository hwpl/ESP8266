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
