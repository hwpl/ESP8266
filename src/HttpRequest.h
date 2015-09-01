#ifndef __HTTPREQUEST_H__
#define __HTTPREQUEST_H__

#include <Arduino.h>

class HttpRequest
{
public:
  HttpRequest(const String &path);
  void addParameter(const String &key, const String &value);
  const void get(char *ret) const;
  const void post(char *ret) const;

private:
  String _path;
  String _request;
  bool _parameterAdded;
};

#endif //__HTTPREQUEST_H__
