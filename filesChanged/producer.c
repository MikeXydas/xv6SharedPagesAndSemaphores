#include "types.h"
#include "stat.h"
#include "user.h"

//care, consumer and producer must have the same size of buffer
#define BUFFER_SIZE 50
//We
int main(void)
{
        printf(1, ">>>Producer initiated (Consumer must start before producer)\n");
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
        char * data = (char *) ((uint) totalProducts + sizeof(int));

        int producePointer = 0, product = 1;

        for(int i = 0; i < *totalProducts; i++)
        {
                sem_down(emptyPlaces);
                data[producePointer] = product % 100;
                sem_up(coveredPlaces);
                producePointer += 1;
                if(producePointer == BUFFER_SIZE)
                        producePointer = 0;
                product++;
        }
        printf(1, ">>>Producer terminated\n");

        exit();
}
