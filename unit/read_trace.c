#include<stdlib.h>
#include<stdio.h>

int main()
{
    char curr_p;
    int curr_page;
    FILE * f_in = fopen("trace2.txt", "r");
    while(fscanf(f_in, "Reference(%c, %d)\n", &curr_p, &curr_page)!=EOF)
        printf("%c, %d\n", curr_p, curr_page);
    printf("file end");
    fclose(f_in);
    return 0 ;
}