#include "types.h"
#include "stat.h"
#include "user.h"


//in this example we are testing exit() and shmrem
int main(void)
{
        printf(1,"\n>>>Initialising shmrem test\n");

        key k;
        key tempKey = k;
        //gathering 32 shared pages
        for(int i = 0; i < 16; i++)
        {
                k.keyArray[i] = i;
        }

        char * sharedPagesArray[32];
        //allocating pages
        for(int i = 0; i < 32; i++)
        {
                sharedPagesArray[i] = shmget(&tempKey);
                tempKey.keyArray[0]++;
        }
        tempKey.keyArray[0] = k.keyArray[0];
        //reallocating same pages
        for(int i = 0; i < 32; i++)
        {
                sharedPagesArray[i] = shmget(&tempKey);
                tempKey.keyArray[0]++;
        }
        tempKey.keyArray[0] = k.keyArray[0];

        //making sure that the mapping is correct
        for(int i = 0; i < 32; i++)
                for(int j = 0; j < 4096; j++)
                        sharedPagesArray[i][j] = j % 100;

        //deleting the allocations (virtual adress unmapped, physical adress freed)
        for(int i = 0; i < 32; i++)
        {
                shmrem(&tempKey);
                tempKey.keyArray[0]++;
        }
        tempKey.keyArray[0] = k.keyArray[0];

        //reallocating the pages
        for(int i = 0; i < 32; i++)
        {
                sharedPagesArray[i] = shmget(&tempKey);
                tempKey.keyArray[0]++;
        }
        tempKey.keyArray[0] = k.keyArray[0];

        //making sure that the new mapping is correct
        for(int i = 0; i < 32; i++)
                for(int j = 0; j < 4096; j++)
                        sharedPagesArray[i][j] = j % 100;

        //deleting the pages (in order to move to the next test)
        for(int i = 0; i < 32; i++)
        {
                shmrem(&tempKey);
                tempKey.keyArray[0]++;
        }
        tempKey.keyArray[0] = k.keyArray[0];

        printf(1,">>>Finished shmrem test\n");

        printf(1,"\n>>>Initialising exit() test\n");
        int pid = fork();

        //the child will shmget 32 pages and then it will exit
        //after the exit the parent will attempt to allocate 32 different pages
        if(pid == 0)
        {
                for(int i = 0; i < 32; i++)
                {
                        sharedPagesArray[i] = shmget(&tempKey);
                        tempKey.keyArray[0]++;
                }
                tempKey.keyArray[0] = k.keyArray[0];
                //making sure that the mapping is correct
                for(int i = 0; i < 32; i++)
                        for(int j = 0; j < 4096; j++)
                                sharedPagesArray[i][j] = j % 100;
                exit();
        }
        else
        {
                //waiting for the child to finish
                wait();
                tempKey.keyArray[1]++;
                for(int i = 0; i < 32; i++)
                {
                        sharedPagesArray[i] = shmget(&tempKey);
                        tempKey.keyArray[1]++;
                }
                //making sure that the mapping is correct
                for(int i = 0; i < 32; i++)
                        for(int j = 0; j < 4096; j++)
                                sharedPagesArray[i][j] = j % 100;
                printf(1,">>>Finshed exit() test\n");
                exit();

        }
}
