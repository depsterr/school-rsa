#ifndef UTIL_H
#define UTIL_H
#include <stddef.h>

void pdie(char* s);
void die(const char* fmt, ...);
void* xmalloc(size_t size);
void* xrealloc(void* p, size_t size);
unsigned long fast_sqrt(unsigned long val);
unsigned long gcd(unsigned long a, unsigned long b);
unsigned long find_d(unsigned long e, unsigned long phi);
#endif
