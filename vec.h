#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

#include "util.h"

typedef struct Vector {
	unsigned long* ptr; 
	size_t len;
	size_t size;
} Vector;

Vector newVector(size_t len);
void expandVector(Vector* vec, size_t size);
void shrinkVector(Vector* vec, size_t size);
void doubleVector(Vector* vec);
void deleteVector(Vector* vec);
void appendVector(Vector* vec, unsigned long a);

#endif
