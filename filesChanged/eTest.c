#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{

        printf(1, "\n>>>Error test 1: More than 32 shared pages \n");

        key k;
        for(int i = 0; i < 16; i++)
        {
                k.keyArray[i] = i;
        }
        for(int i = 0; i < 33; i++)
        {
                int j = 0;
                char * temp = shmget(&k);
                if(temp != 0)
                {
                        for(char * i = temp; i < temp + 4096; i++)
                        {
                                *i = (char) (j % 100);
                                j++;
                        }
                }
                k.keyArray[0] += 1;
        }
        printf(1, "Passed test 1\n");

        k.keyArray[0] -= 1;
        for(int i = 0; i < 32; i++)
        {
                k.keyArray[0] -= 1;
                shmrem(&k);
        }

        printf(1, "\n>>>Error test 2: More than 16 procs sharing same page\n");
        for(int i = 0; i < 17; i++)
        {
                int j = 0;
                char * temp = shmget(&k);
                if(temp != 0)
                {
                        for(char * i = temp; i < temp + 4096; i++)
                        {
                                *i = (char) (j % 100);
                                j++;
                        }
                }
        }
        printf(1, "Passed test 2\n");
        exit();
}
