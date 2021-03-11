#include <stdlib.h>

#include "vec.h"
#include "util.h"

Vector newVector(size_t len) {
	return (Vector){
		.ptr = xmalloc(len * sizeof(long)),
		.len = len,
		.size = len * sizeof(long),
	};
}

void expandVector(Vector* vec, size_t size) {
	vec->size += size;
	vec->ptr = xrealloc(vec->ptr, vec->size * sizeof(long));
}

void shrinkVector(Vector* vec, size_t size) {
	vec->size -= size;
	vec->ptr = xrealloc(vec->ptr, vec->size * sizeof(long));
}

void doubleVector(Vector* vec) {
	vec->size *= 2;
	vec->ptr = xrealloc(vec->ptr, vec->size * sizeof(long));
}

void deleteVector(Vector* vec) {
	free(vec->ptr);
	vec->size = 0;
	vec->len = 0;
}

void appendVector(Vector* vec, long a) {
	if (vec->len + 1 > vec->size) {
		doubleVector(vec);
	}
	vec->ptr[vec->len] = a;
	vec->len++;
}
