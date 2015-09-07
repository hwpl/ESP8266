/**
 *  @file
 *  @brief Helper for time related functions.
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
