#include "types.h"
#include "stat.h"
#include "user.h"

//Buffer size must be smaller than PGSIZE - 2 * sizeof(sem_t) - sizeof(int)
#define BUFFER_SIZE 50
#define TOTAL_PRODUCTS 100

int main(int argc, char *argv[])
{
        printf(1, "\n>>>Consumer initiated (Consumer must start before producer)\n");
        if(BUFFER_SIZE > 4096 - 2 * sizeof(sem_t) - sizeof(int))
        {
                printf(1,"Error: Buffer would go on unmapped memory\n");
                exit();
        }
        sem_t * emptyPlaces;
        sem_t * coveredPlaces;
        key k;
        for(int i = 0; i < 16; i++)
        {
                k.keyArray[i] = 9;
        }

        //page = sem: emptyPlaces - sem: coveredPlaces - totalProducts - data
        emptyPlaces = (sem_t *) shmget(&k);
        coveredPlaces = (sem_t *)((uint)emptyPlaces + sizeof(sem_t));
        int * totalProducts = (int *) ((uint) coveredPlaces + sizeof(sem_t));
        *totalProducts = TOTAL_PRODUCTS;
        char * data = (char *) ((uint) totalProducts + sizeof(int));

        sem_init(emptyPlaces , BUFFER_SIZE);
        sem_init(coveredPlaces , 0);

        int sum = 0;
        int consumePointer = 0;

        for(int i = 0; i < *totalProducts; i++)
        {
                sem_down(coveredPlaces);
                sum += data[consumePointer];
                sem_up(emptyPlaces);
                consumePointer += 1;
                if(consumePointer == BUFFER_SIZE)
                        consumePointer = 0;
        }

        printf(1,"Sum of consumes = %d\n",sum);
        printf(1, ">>>Consumer terminated\n");

        exit();
}
