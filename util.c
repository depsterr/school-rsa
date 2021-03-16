#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void pdie(char* s) {
	perror(s);
	exit(1);
}

void die(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	exit(1);
}

void* xmalloc(size_t size) {
	void* r = malloc(size);

	if (!r) {
		pdie("Unable to allocate memory");
	}

	return r;
}

void* xrealloc(void* p, size_t size) {
	void* r = realloc(p, size);

	if (!r) {
		pdie("Unable to allocate memory");
	}

	return r;
}

unsigned long fast_sqrt(unsigned long val) {
	unsigned long a, b;

	a = 5000;

	b = val / a; a = (a+b)/2;
	b = val / a; a = (a+b)/2;
	b = val / a; a = (a+b)/2;
	b = val / a; a = (a+b)/2;
	b = val / a; a = (a+b)/2;
	b = val / a; a = (a+b)/2;

	return a;
}

unsigned long gcd(unsigned long a, unsigned long b) {
	unsigned long tmp;

	while (b != 0) {
		tmp = b;
		b = a % b;
		a = tmp;
	}

	return a;
}
