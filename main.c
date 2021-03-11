#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "vec.h"
#include "prim.h"

#define PRIME_MAX (2000000-1)

typedef struct {
	unsigned long e;
	unsigned long n;
} pubkey;

typedef struct {
	unsigned long d;
	unsigned long n;
} privkey;

typedef struct {
	pubkey pub;
	privkey priv;
} keyset;

typedef enum action {
	KEYGEN,
	ENCRYPT,
	DECRYPT,
	HELP,
	UNKNOWN
} action;

keyset generate_keyset(void) {
	srand(time(0));

	keyset ret;
	unsigned long q, p, n, e ,d;

	q = 1 + rand() % PRIME_MAX;
	p = 1 + rand() % PRIME_MAX;

	unsigned long max = q > p ? q : p;

	printf("Generating %ld primes\n", max);

	Vector primes = get_n_primes(max + 1);

	q = primes.ptr[q];
	p = primes.ptr[p];

	printf("q: %ld\np: %ld\n", q, p);

	deleteVector(&primes);

	n = q * p;
	n = q * p;

	unsigned long phi = (q-1) * (p-1);

	do {
		e = rand() % phi;
		while (gcd(phi, e) != 1) {
			e++;
		}
	} while (e == phi);


	d = rand();
	while ((d * e) % 60 != 1) {
		d++;
	}

	ret.priv.n = n;
	ret.pub.n = n;
	ret.priv.d = d;
	ret.pub.e = e;
	return ret;
}

unsigned long encrypt(pubkey p, unsigned long x) {
	return crypt(x, p.e, p.n);
}

unsigned long decrypt(privkey p, unsigned long y) {
	return crypt(y, p.d, p.n);
}

int main(int argc, char** argv) {
	action todo = UNKNOWN;
	int status = 0;
	keyset set;
	unsigned long result, a, b, c;

	if (argv[1]) {
		if (!strcmp(argv[1], "keygen")) {
			todo = KEYGEN;
		} else if (!strcmp(argv[1], "encrypt") && argv[2] && argv[3] && argv[4]) {
			todo = ENCRYPT;
		} else if (!strcmp(argv[1], "decrypt") && argv[2] && argv[3] && argv[4]) {
			todo = DECRYPT;
		} else if (!strcmp(argv[1], "help")) {
			todo = HELP;
		}
	}

	if (todo == ENCRYPT || todo == DECRYPT) {
		a = strtoul(argv[2], 0, 10);
		b = strtoul(argv[3], 0, 10);
		c = strtoul(argv[4], 0, 10);
	}

	switch (todo) {
		case KEYGEN:
			set = generate_keyset();
			printf("Generated keyset:\nprivkey = {\n\td: %ld\n\tn: %ld\n}\npubkey = {\n\te: %ld\n\tn: %ld\n}\n",
					set.priv.d, set.priv.n, set.pub.e, set.pub.n);
			break;
		case ENCRYPT:
			set.pub.n = a;
			set.pub.e = b;
			result = encrypt(set.pub, c);
			printf("Result: %ld\n", result);
			break;
		case DECRYPT:
			set.priv.n = a;
			set.priv.d = b;
			result = decrypt(set.priv, c);
			printf("Result: %ld\n", result);
			break;
		case UNKNOWN:
			status = 1;
		case HELP:
			puts("Usage:\n"
				 "\n"
				 "  rsa keygen - generate keys\n"
				 "  rsa encrypt n e x - encrypt x with pubkey n and e\n"
				 "  rsa decrypt n d y - decrypt y with privkey n and d\n");
			exit(status);
			break;
	}
}
