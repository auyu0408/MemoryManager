#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *f_config;
FILE *f_in;
FILE *f_out;

int main()
{
    char tlb_policy[50];
    char page_policy[50];
    char frame_policy[50];
    int total_p;
    int n;
    int m;
    f_config = fopen("sys_config.txt", "r");
    fscanf(f_config, "TLB Replacement Policy: %s\n", tlb_policy);
    fscanf(f_config, "Page Replacement Policy: %s\n", page_policy);
    fscanf(f_config, "Frame Allocation Policy: %s\n", frame_policy);
    fscanf(f_config, "Number of Processes: %d\n", &total_p);
    fscanf(f_config, "Number of Virtual Page: %d\n", &n);
    fscanf(f_config, "Number of Physical Frame: %d\n", &m);
    printf("%s %s %s %d %d %d", tlb_policy, page_policy, frame_policy, total_p, n, m);
    fclose(f_config);
    return 0;
}