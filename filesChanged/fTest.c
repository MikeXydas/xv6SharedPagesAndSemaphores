#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
        printf(1,">>>Fork test 1: 1 fork with 1 shared page\n");
        key k;
        for(int i = 0; i < 16; i++)
        {
                k.keyArray[i] = i;
        }
        char * charArray = (char *) shmget(&k);
        int j = 0;
        for(char * i = charArray; i < charArray + 4096; i++)
        {
                *i = (char) (j % 100);
                j++;
        }
        int pid;
	if((pid = fork()) == -1)
		printf(1,"Fork failed...\n");
        int sum = 0;
        for(char * i = charArray; i < charArray + 4096; i++)
                 sum += (int) *i;
        if(pid == 0)
                printf(1, "Sum of data of shared page on child = %d\n", sum);
        else
	{
		wait();
                printf(1, "Sum of data of shared page on parent = %d\n", sum);
	}
        printf(1, "Deleting page, returns: %d\n", shmrem(&k));
        if(pid == 0)
                exit();
        printf(1, "Passed fork test 1\n");

        printf(1,"\n>>>Fork test 2: Fork must fail since 9 procs using the page\n");
        for(int i =0; i < 9; i++)
                shmget(&k);
	if(fork() == -1)
        	printf(1,"Fork failed...\n");
        printf(1, "\n>>>Passed fork test 2\n");


        exit();
}
