#ifndef MEM
#define MEM
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<string.h>

struct page_table_element//page table element
{
    int pfn_dbn;//page number or disk block number
    int ref;//reference bit
    int present;//present bit
};

struct TLBelement
{
    int vpn;//virtual page number
    int pfn;//page frame number
    int timestamp;//least recently used
    int clean;
};

struct DiskBlock
{
    int block_id;//disk block number
    int full;
    struct DiskBlock * next;
};

struct frame
{
    int f_num;//frame number
    int vpn;
    int free;//free frame or not
    char process;
    int used;
    struct frame * next;
};

typedef struct page_table_element PTelement;
typedef struct TLBelement TLBelement;
typedef struct DiskBlock DBlock;
typedef struct frame frame;
typedef struct frame * frame_ptr;

char tlb_policy[10];
char page_policy[10];
char frame_policy[10];
int total_p;
int n;
int m;
int i;//counting variable
long timer=0;
int total_block = 0;
int tlb_process = -1;
int * total_access;
int * effective_access;
int * page_fault;
int * origin_access;

TLBelement TLB[32];
frame_ptr freehead;
frame_ptr freetail;
frame_ptr ghead;
frame ** lhead;
frame_ptr gtail;
frame ** ltail;
frame_ptr * gptr;
frame *** lptr;
PTelement **PT;
DBlock * dbhead;
DBlock * dbtail;

#endif