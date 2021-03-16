#ifdef __linux__
#	include <sys/random.h>
#elif __unix__
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#define GRND_RANDOM 1
size_t getrandom(void* b, size_t s, int flags) {
        size_t ret;
        int fd;
        fd = open("/dev/urandom", O_RDONLY);
        ret = read(fd, b, s);
        close(fd);
        return ret;
}
#else
#include <time.h>
#include <stdint.h>
#define GRND_RANDOM 1
size_t getrandom(void* b, size_t s, int flags) {
	void* e = b + s;
	while (b <= e) {
		*(uint8_t*)b = time(0);
		b++;
	}
	return s;
}
#endif

#include "hs.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "vec.h"
#include "prim.h"

/* största och minsta möjliga indexet av primtal vi vill kunna generera */
#define PRIME_MIN (100000ul)
#define PRIME_MAX (999999ul)

/* data strukturer för privata och publika nycklar */
typedef struct {
	unsigned long e;
	unsigned long n;
} pubkey;

typedef struct {
	unsigned long d;
	unsigned long n;
} privkey;

/* data struktur för både privat och publik nyckel */
typedef struct {
	pubkey pub;
	privkey priv;
} keyset;

/* de olika aktionerna som vårt program kan köra */
typedef enum action {
	KEYGEN,
	ENCRYPT,
	DECRYPT,
	HELP,
	HACK,
	UNKNOWN
} action;

/* denna funktionen tar en pubkey p och returnerar motsvarande d */
unsigned long hack(pubkey p) {
	unsigned long d = 0;

	unsigned long phi = 0;

	Vector primes = get_primes_to((p.n / 2) + 1);

	for (unsigned long i = 0; i < primes.len; i++) {
		if (p.n % primes.ptr[i] == 0) {
			phi = ((p.n / primes.ptr[i]) - 1) * (primes.ptr[i] - 1);
			break;
		}
	}

	if (!phi) {
		die("Unable to find phi\n");
	}

	printf("phi: %lu\n", phi);

	d = findDHs(p.e, phi);

	if ((d * p.e) % phi != 1) {
		die("ERROR: Generated invalid d: %lu\n", d);
	}

	return d;
}

/* denna funktionen tar inga argument och returnerar ett keyset med både */
/* en publik och private key som hör ihop */
keyset generate_keyset(void) {
	unsigned int seed;
	if (getrandom(&seed, sizeof(seed), GRND_RANDOM) != sizeof(seed)) {
		pdie("Unable to get random seed");
	}

	srand(seed);

	keyset ret;
	unsigned long q, p, n, e ,d;

	q = PRIME_MIN + (rand() % (PRIME_MAX - PRIME_MIN));
	p = PRIME_MIN + (rand() % (PRIME_MAX - PRIME_MIN));

	unsigned long max = q > p ? q : p;

	printf("Generating %lu primes\n", max);

	Vector primes = get_n_primes(max + 1);

	q = primes.ptr[q];
	p = primes.ptr[p];

	printf("q: %lu\np: %lu\n", q, p);

	deleteVector(&primes);

	n = q * p;

	unsigned long phi = (q-1) * (p-1);

	printf("phi: %lu\nn=%lu\n", phi, n);

	e = 2;
	while (gcd(phi, e) != 1) {
		e++;
	}

	printf("e=%lu\n", e);

	d = findDHs(e, phi);

	if ((d * e) % phi != 1) {
		die("ERROR: Generated invalid d: %lu\n", d);
	}

	printf("d=%lu\n", d);

	ret.priv.n = n;
	ret.pub.n = n;
	ret.priv.d = d;
	ret.pub.e = e;
	return ret;
}

/* startpunkten för vårt program */
int main(int argc, char** argv) {
	action todo = UNKNOWN;
	int status = 0;
	keyset set;
	unsigned long result, a, b, c;

	hs_init(&argc, &argv);

	/* avgör vilken aktion vi bör ta */
	if (argv[1]) {
		if (!strcmp(argv[1], "keygen")) {
			todo = KEYGEN;
		} else if (!strcmp(argv[1], "encrypt") && argv[2] && argv[3] && argv[4]) {
			todo = ENCRYPT;
		} else if (!strcmp(argv[1], "decrypt") && argv[2] && argv[3] && argv[4]) {
			todo = DECRYPT;
		} else if (!strcmp(argv[1], "hack") && argv[2] && argv[3]) {
			todo = HACK;
		} else if (!strcmp(argv[1], "help")) {
			todo = HELP;
		}
	}

	/* konvertera strängar från våra argument till nummer */
	switch (todo) {
		case ENCRYPT:
		case DECRYPT:
			c = strtoul(argv[4], 0, 10);
		case HACK:
			a = strtoul(argv[2], 0, 10);
			b = strtoul(argv[3], 0, 10);
		default:;
	}

	/* kör rätt funktion respektive vår aktion och printa resultaten */
	switch (todo) {
		case KEYGEN:
			set = generate_keyset();
			printf("Generated keyset:\nprivkey = {\n\td: %lu\n\tn: %lu\n}\npubkey = {\n\te: %lu\n\tn: %lu\n}\n",
					set.priv.d, set.priv.n, set.pub.e, set.pub.n);
			break;
		case ENCRYPT:
		case DECRYPT:
			/* result = rsacrypt(c, b, a); */
			result = rsaCryptHs(c, b, a);
			printf("Result: %lu\n", result);
			break;
		case HACK:
			set.pub.n = a;
			set.pub.e = b;
			result = hack(set.pub);
			printf("d: %lu\n", result);
			break;
		case UNKNOWN:
			status = 1;
		case HELP:
			puts("Usage:\n"
				 "\n"
				 "  rsa keygen - generate keys\n"
				 "  rsa encrypt n e x - encrypt x with pubkey n and e\n"
				 "  rsa decrypt n d y - decrypt y with privkey n and d\n"
				 "  rsa hack n e - hack private key from public key\n");
			exit(status);
			break;
	}

	hs_exit();
}
