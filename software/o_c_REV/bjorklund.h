// Copyright (c) 2016 Tim Churches
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
// Bjorklund (Euclidean) patterns, generated by resources/bjorklund.py

#ifndef BJORKLUND_H_
#define BJORKLUND_H_

#include <stdint.h>
// #include <limits.h> // for CHAR_BIT

// rotate functions by John Regehr
// see http://stackoverflow.com/questions/776508/best-practices-for-circular-shift-rotate-operations-in-c
//inline uint32_t rotr32 (uint32_t n, uint8_t c) __attribute__((always_inline));
//inline uint32_t rotr32 (uint32_t n, uint8_t c) {
//  const unsigned int mask = (CHAR_BIT*sizeof(n)-1);
//  c &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
//  return (n>>c) | (n<<( (-c)&mask ));
//}

//inline uint32_t rotl32 (uint32_t n, unsigned int c) __attribute__((always_inline));
//inline uint32_t rotl32 (uint32_t n, unsigned int c) {
//  const unsigned int mask = (CHAR_BIT*sizeof(n)-1);
//  c &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
//  return (n<<c) | (n>>( (-c)&mask ));
//}

inline uint32_t rotl32(uint32_t input, unsigned int length, unsigned int count) __attribute__((always_inline));
inline uint32_t rotl32(uint32_t input, unsigned int length, unsigned int count) {
  input &= ~(0xffffffff << length);
  return (input << count) | (input >> (length - count + 1)); // off-by-ones or parenthesis mismatch likely
}

bool EuclideanFilter(uint8_t num_steps, uint8_t num_beats, uint8_t rotation, uint32_t clock);
uint32_t EuclideanPattern(uint8_t num_steps, uint8_t num_beats, uint8_t rotation);

#endif // BJORKLUND_H_
