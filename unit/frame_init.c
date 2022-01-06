#include<stdio.h>
#include<stdlib.h>

struct frame
{
    int f_num;//frame number
    int p_num;//process number
    int vpn;
    int free;//free frame or not
    char process;
    struct frame * next;
};
typedef struct frame frame;
typedef struct frame * frame_ptr;
frame_ptr freehead;
frame_ptr freetail;
frame_ptr next;
frame_ptr next_tail;
frame_ptr * clockptr;

void enq(frame_ptr *new_frame, frame_ptr * head, frame_ptr * tail)
{
    if((*head) != NULL)
    {
        (*tail)->next = (*new_frame);
        (*tail) = (*new_frame);
        (*tail)->next = (*head);
    }
    else
    {
        (*head) = (*tail) = (*new_frame);
        (*tail)->next = (*head);
    }
}

frame_ptr deq(int n, frame_ptr * head, frame_ptr * tail)
{
    if((*head) == NULL)
        return NULL;
    else
    {
        frame_ptr temp = (*head);
        frame_ptr temp_ex = NULL;
        while(temp->next != (*head))
        {
            if(temp->f_num != n)
            {
                temp_ex = temp;
                temp = temp->next;
            }
            else
            {
                if(temp == (*head))
                {
                    if((*head) == (*tail))
                        (*head) = (*tail) = NULL;
                    else
                    {
                        (*head) = temp->next;
                        (*tail)->next = (*head);
                    }
                }
                else if(temp == (*tail))
                {
                    (*tail) = temp_ex;
                    (*tail)->next = (*head);
                }
                else
                {
                    temp_ex->next = temp->next;
                }
                break;
            }
        }
        return temp;
    }
}

int main()
{   
    int m = 64;
    freehead = freetail = next = next_tail = NULL;
    clockptr = &freehead;
    for(int i=0; i<m; i++)
    {
        frame_ptr newtemp = malloc(sizeof(frame));
        newtemp->f_num = i;
        newtemp->vpn = 0;
        newtemp->free = 1;
        newtemp->next = NULL;
        enq(&newtemp, &freehead, &freetail);
    }
    frame_ptr temp;
    for(int i=0;i<m;i++)
    {
        temp = deq(18,&freehead,&freetail);
        enq(&temp,&freehead,&freetail);
    }
    printf("\n");
    temp = freehead;
    do
    {
        printf("%d ", temp->f_num);
        temp = temp->next;
    }while(temp != freehead);
    printf("%d ", temp->f_num);
    return 0;
}