#include <stdlib.h>

#include "util.h"
#include "vec.h"

//! Returns vector array of n primes
Vector get_n_primes(unsigned long n) {
	Vector ret = newVector(n);
	ret.ptr[0] = 2;
	ret.ptr[1] = 3;
	ret.len = 2;

	for (unsigned long i = 5; ret.len < n; i++) {
		unsigned long root = fast_sqrt(i);
		unsigned char prime = 1;

		for (unsigned long k = 0; k < ret.len && ret.ptr[k] <= root; k++) {
			if ((i % ret.ptr[k]) == 0) {
				prime = 0;
				break;
			}
		}

		if (prime) {
			ret.ptr[ret.len] = i;
			ret.len++;
		}
	}

	return ret;
}
