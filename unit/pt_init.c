#include<stdio.h>
#include<stdlib.h>

struct page_table_element//page table element
{
    int pfn_dbn;//page number or disk block number
    int ref;//reference bit
    int present;//present bit
};

typedef struct page_table_element PTelement;
PTelement **PT;

int main()
{
    int total_p = 2;
    int n = 24;
    PT = (PTelement **)malloc(sizeof(PTelement *)*total_p);
    for(int i=0; i<total_p; i++)
    {
        PT[i] = (PTelement *)malloc(sizeof(PTelement)*n);
        for(int j=0; j< n; j++)
        {
            PT[i][j].pfn_dbn = -1;
            PT[i][j].ref = 0;
            PT[i][j].present = 0;
        }
    }
    printf("%d", PT[1][21].pfn_dbn);
    return 0;
}