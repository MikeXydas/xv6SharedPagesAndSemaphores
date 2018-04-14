#ifndef TYPES_H
#define TYPES_H

#include "spinlock.h"

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

#define MAX_SHARED_PAGES 32
#define MAX_PROCS_SHARING_PAGE 16


typedef struct Key{
        char keyArray[16];
} key;

typedef struct Key * sh_key_t;

typedef struct procAndPage{
        int pid;
        uint addrPtr;
} procAndPage;

typedef struct pageInfo{
        key pageKey;
        int procsSharingKey;
        procAndPage procsUsingPage[16];
        uint physicalAddr;
} pageInfo;

//Global array which contains all the CURRENT shmget pages (max size 32)
extern pageInfo systemPages[32];

struct spinlock;

typedef struct Sem_t{
        uint value;       // if value == 0 -> locked
        struct spinlock lk; // spinlock protecting this sleep lock

        // For debugging:
        char *name;        // Name of lock.
        int pid;           // Process holding lock
} sem_t;


#endif
