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
