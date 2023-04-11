/**
 * Produtor - Memória compartilhada - Linux
 * Para compilar
 * 	gcc shm-posix-producer.c -lrt
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#define SIZE 4096
#define NUMBER_OF_ITEMS 10

int main(){
	const char *name = "esteira_A";

	int shm_fd;
	int *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
	clock_t start = clock();
	int number_of_items = 0;
	for(int i = 0; i < NUMBER_OF_ITEMS; i++){
		number_of_items++;
		printf("Total items: %d\n", number_of_items);
		*ptr = number_of_items;
		sleep(2);
	}
	clock_t end = clock();
	printf("Time: %f\n", (float)(end - start)/CLOCKS_PER_SEC);
	return 0;
}