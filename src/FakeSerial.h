#ifndef __FAKESERIAL_H__
#define __FAKESERIAL_H__

#include "FakeStreamBuffer.h"

class FakeSerial : public FakeStreamBuffer
{
public:
  void begin(unsigned long baud)
  { }

  unsigned int write(const char *buffer, unsigned int length)
  {
    writeString += buffer;
    return strlen(buffer);
  }

  const String& getWrittenString() const
  {
    return writeString;
  }


private:
  String writeString;
};

#endif
