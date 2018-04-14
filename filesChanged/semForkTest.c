#include "types.h"
#include "stat.h"
#include "user.h"


//in this example we are reading and writing on some shared pages
//reading and writing is done by 2 procs (child and parent) synchronised with a binarey sem
//first the child will fill all the pages, then the parent will read them
int main(void)
{
        printf(1,"\n>>>Initialising fork and semaphore test\n");
        sem_t * sync;
        key k;
        //gathering 32 shared pages (care as the first page will also contain the semaphore)
        for(int i = 0; i < 16; i++)
        {
                k.keyArray[i] = i;
        }
        char * sharedPagesArray[32];
        for(int i = 0; i < 32; i++)
        {
                sharedPagesArray[i] = shmget(&k);
                k.keyArray[0] += 1;
        }
        sync = (sem_t *) sharedPagesArray[0];
        sem_init(sync , 0);

        int pid= fork();

        //child filling the pages with random data
        if(pid == 0)
        {
                printf(1,"\nChild started filling pages\n");
                for(int whichPage = 0; whichPage < 32; whichPage++)
                {
                        int startingPoint = 0;
                        //avoiding semaphore
                        if(whichPage == 0)
                                startingPoint += sizeof(sem_t);
                        for(int whichByte = startingPoint; whichByte < 4096; whichByte++)
                        {
                                sharedPagesArray[whichPage][whichByte] = whichByte % 100;
                        }
                }
                printf(1,"Child finished filling pages\n");
                sem_up(sync);
                exit();
        }
        else
        {
                sem_down(sync);
                int sum = 0;
                printf(1,"\nParent started reading pages\n");
                for(int whichPage = 0; whichPage < 32; whichPage++)
                {
                        int startingPoint = 0;
                        //avoiding semaphore
                        if(whichPage == 0)
                                startingPoint += sizeof(sem_t);
                        for(int whichByte = startingPoint; whichByte < 4096; whichByte++)
                        {
                                sum += sharedPagesArray[whichPage][whichByte];
                        }
                }
                printf(1, "Total sum = %d\n",sum);
                printf(1,"Parent finished reading pages\n");
                printf(1,"\n>>>Finished fork and semaphore test\n");
                wait();
                exit();
        }

}
