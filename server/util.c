#include "util.h"

int generate_random_number() {
    int random_fd = open("/dev/urandom", O_RDONLY);
    if (random_fd < 0) {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }

    unsigned int random_value;
    if (read(random_fd, &random_value, sizeof(random_value)) < sizeof(random_value)) {
        perror("Failed to read from /dev/urandom");
        close(random_fd);
        exit(EXIT_FAILURE);
    }

    close(random_fd);

	int id_min_value = 10000000;
	int id_max_value = 99999999;

	unsigned int range = (unsigned int)(id_max_value - id_min_value + 1);
	
    return (random_value % range) + id_min_value;
}