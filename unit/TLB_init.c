#include<stdio.h>
#include<stdlib.h>

struct TLBelement
{
    int vpn;//virtual page number
    int pfn;//page frame number
    int timestamp;//least recently used
    int clean;
};
typedef struct TLBelement TLBelement;
TLBelement TLB[32];

int main()
{
    for(int i = 0; i<32; i++)
    {
        TLB[i].vpn = 0;
        TLB[i].pfn = 0;
        TLB[i].clean = 0;
        TLB[i].timestamp = 0;
    }
    printf("%d", TLB[16].clean);
    return 0;
}