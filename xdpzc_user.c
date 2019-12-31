#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "libbpf.h"
#include "xsk.h"
#include "xdpsock.h"
#include <bpf/bpf.h>

#define NUM_FRAMES (4*1024)

int main(int argc, char **argv)
{
    struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
	void *bufs;

	if (setrlimit(RLIMIT_MEMLOCK, &r)) {
		fprintf(stderr, "ERROR: setrlimit(RLIMIT_MEMLOCK) \"%s\"\n",
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	bufs = mmap(NULL, NUM_FRAMES * XSK_UMEM__DEFAULT_FRAME_SIZE,
		    PROT_READ | PROT_WRITE,
		    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (bufs == MAP_FAILED) {
		printf("ERROR: mmap failed\n");
		exit(EXIT_FAILURE);
	}

	umem = xsk_configure_umem(bufs, NUM_FRAMES * XSK_UMEM__DEFAULT_FRAME_SIZE);
	if (opt_bench == BENCH_RXDROP || opt_bench == BENCH_L2FWD) {
		rx = true;
		xsk_populate_fill_ring(umem);
	}


    return 0;
}