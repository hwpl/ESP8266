#ifndef __TIME_HELPER_H__
#define __TIME_HELPER_H__

#include <Arduino.h>

/**
 * Returns if the given value in milliseconds is in the future.
 * @note The method is overflow aware.
 * @note The function internally uses millis() to get the present time stamp.
 * @param futureMillis The timestamp of the future.
 * @return True if the value is in the future. False if it is the same time or in the past.
 */
static inline bool isFuture(unsigned long futureMillis)
{
  return ((long)millis() - (long)futureMillis) <= 0;
}

#endif
