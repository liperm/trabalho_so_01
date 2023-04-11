/**
 * Consumidor - Memória compartilhada - Linux
 * Para compilar
 *	gcc shm-posix-consumer.c -lrt
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#define SIZE 4096
#define MAX_COMPUTATIONS 10 //número de ciclos de leitura
#define ITEM_WEIGHT_A 5 //peso dos itens da esteira A
#define ITEM_WEIGHT_B 2 //peso dos itens da esteira B
#define ITEMS_TO_UPDATE 10 //numero de itens para atualizar o peso total (enunciado traz como 500)

int main(){
	const char *name_esteira_a = "esteira_A";
	const char *name_esteira_b = "esteira_B";

	int shm_fd_esteira_a;
	int shm_fd_esteira_b;
	int *ptr_a, *ptr_b;
	int i;

	/* open the shared memory segment */
	shm_fd_esteira_a = shm_open(name_esteira_a, O_RDONLY, 0666);
	if (shm_fd_esteira_a == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	shm_fd_esteira_b = shm_open(name_esteira_b, O_RDONLY, 0666);
	if (shm_fd_esteira_b == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr_a = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd_esteira_a, 0);
	if (ptr_a == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	ptr_b = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd_esteira_b, 0);
	if (ptr_b == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}
	
	int total_items = 0;
	int total_weight = 0;
	int weighing_cicles = 1;
	clock_t start = clock();
	for(int i = 0; i < MAX_COMPUTATIONS; i++){
		total_items = *ptr_b + *ptr_a;
		if(total_items >= ITEMS_TO_UPDATE * weighing_cicles){
			total_weight += (*ptr_a * ITEM_WEIGHT_A) + (*ptr_b * ITEM_WEIGHT_B);
			weighing_cicles ++;
		}

		printf("Total of Items: %d\tTotal from A: %d\tTotal from B: %d\tTotal Weight: %d\n",
				total_items, *ptr_a, *ptr_b, total_weight);
		sleep(2);
	}
	clock_t end = clock();
	printf("Time: %f\n", (float)(end - start)/CLOCKS_PER_SEC);

	/* remove the shared memory segment */
	if (shm_unlink(name_esteira_a) == -1) {
		printf("Error removing %s\n",name_esteira_a);
		exit(-1);
	}

	if (shm_unlink(name_esteira_b) == -1) {
		printf("Error removing %s\n",name_esteira_b);
		exit(-1);
	}
	return 0;
}