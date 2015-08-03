#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "algorithm.h"

#define BLOCKSZ		128
#define BLOCKCNT	((DIM + BLOCKSZ - 1) / BLOCKSZ)

uint8_t *pixels;
static struct block_t {
	uint32_t x, y;
} block;
struct info_t {
	uint32_t id;
	int pipe;
};

int pipeFromThreads[2];
pthread_mutex_t pipeMutex = PTHREAD_MUTEX_INITIALIZER;

void *thread(void *param)
{
	struct info_t *info = param;
	uint32_t id = info->id;
	int pipe = info->pipe;
	free(param);

	uint32_t bx, by;
loop:
	// Write thread id to pipe, ready to receive task
	pthread_mutex_lock(&pipeMutex);
	write(pipeFromThreads[1], &id, sizeof(id));
	pthread_mutex_unlock(&pipeMutex);
	read(pipe, &bx, sizeof(bx));
	read(pipe, &by, sizeof(by));
	if (by == BLOCKCNT) {
		//fprintf(stderr, "Thread %u done.\n", id);
		return NULL;
	}
	fprintf(stderr, "Thread %u task block: (%u, %u).\n", id, bx, by);
	bx *= BLOCKSZ;
	by *= BLOCKSZ;
	uint32_t bw, bh;
	bw = DIM - bx < BLOCKSZ ? DIM - bx : BLOCKSZ;
	bh = DIM - by < BLOCKSZ ? DIM - by : BLOCKSZ;
	fprintf(stderr, "Thread %u task region: (%u, %u, %u, %u).\n",
			id, bx, by, bw, bh);
	int x, y;
	for (y = by; y < by + bh; y++)
		for (x = bx; x < bx + bw; x++) {
			uint32_t clr = pixel_write(x, y);
			memcpy(pixels + 3 * (DIM * y + x), &clr, 3);
		}
	goto loop;
	return NULL;
}

int blocksAvailable(void)
{
	return block.y != BLOCKCNT;
}

int nextBlock(void)
{
	if (!blocksAvailable())
		return 0;
	block.x++;
	if (block.x == BLOCKCNT) {
		block.x = 0;
		block.y++;
		if (block.y == BLOCKCNT)
			return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fputs("Argument: threads\n", stderr);
		return 1;
	}
	int threads = atoi(argv[1]);
	if (threads <= 0) {
		fputs("Need more than 1 threads!\n", stderr);
		return 1;
	}

	pthread_t pid[threads];
	int pipeToThreads[threads][2];
	pipe(pipeFromThreads);
	// Variable initialise & memory allocation
	block.x = 0;
	block.y = 0;
	pixels = malloc(sizeof(uint8_t) * DIM * DIM * 3);
	if (pixels == 0) {
		fputs("Cannot alloc memory!\n", stderr);
		return 1;
	}
	printf("P6\n%d %d\n255\n", DIM, DIM);
	time_t tStart = time(NULL);

	// Rendering
	fputs("Rendering...\n", stderr);
	// Initial threads
	uint32_t id;
	for (id = 0; id < threads; id++) {
		pipe(pipeToThreads[id]);
		struct info_t *info = malloc(sizeof(struct info_t));
		info->id = id;
		info->pipe = pipeToThreads[id][0];
		pthread_create(&pid[id], NULL, thread, info);
	}
	// Task assign
	while (blocksAvailable()) {
		read(pipeFromThreads[0], &id, sizeof(id));
		write(pipeToThreads[id][1], &block.x, sizeof(block.x));
		write(pipeToThreads[id][1], &block.y, sizeof(block.y));
		nextBlock();
	}
	// Threads finishing up
	int i;
	for (i = 0; i < threads; i++) {
		read(pipeFromThreads[0], &id, sizeof(id));
		write(pipeToThreads[id][1], &block.x, sizeof(block.x));
		write(pipeToThreads[id][1], &block.y, sizeof(block.y));
		pthread_join(pid[id], NULL);
		fprintf(stderr, "Thread %u done.\n", id);
	}
	pthread_mutex_destroy(&pipeMutex);
	time_t tRender = time(NULL);
	fprintf(stderr, "Time elapsed: %lds\n", tRender - tStart);

	// Write out pixels
	fputs("Writing output...\n", stderr);
	for (int y = 0; y < DIM; y++)
		for (int x = 0; x < DIM; x++)
			fwrite(pixels, 1, DIM * DIM * 3, stdout);
	time_t tOutput = time(NULL);
	fprintf(stderr, "Time elapsed: %lds\n", tOutput - tRender);
	fprintf(stderr, "Total time elapsed: %lds\n", tOutput - tStart);
	return 0;
}
