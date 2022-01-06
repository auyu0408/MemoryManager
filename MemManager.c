#include "MemManager.h"

FILE *f_config;
FILE *f_in;
FILE *f_out;
FILE *f_ana;

void open_file()
{
    f_config = fopen("sys_config.txt", "r");
    if(f_config == NULL) exit(0);
    f_in = fopen("trace.txt", "r");
    if(f_in == NULL) exit(0);
    f_out = fopen("trace_output.txt", "w");
    if(f_out == NULL) exit(0);
    f_ana = fopen("analysis.txt", "w");
    if(f_ana == NULL) exit(0);
}

void get_config()
{
    fscanf(f_config, "TLB Replacement Policy: %s\n", tlb_policy);
    fscanf(f_config, "Page Replacement Policy: %s\n", page_policy);
    fscanf(f_config, "Frame Allocation Policy: %s\n", frame_policy);
    fscanf(f_config, "Number of Processes: %d\n", &total_p);
    fscanf(f_config, "Number of Virtual Page: %d\n", &n);
    fscanf(f_config, "Number of Physical Frame: %d\n", &m);
    fclose(f_config);
}

void set_TLB()
{
    for(i = 0; i<32; i++)
    {
        TLB[i].vpn = 0;
        TLB[i].pfn = 0;
        TLB[i].clean = 0;
        TLB[i].timestamp = 0;
    }
}

void f_enq(frame_ptr *new_frame)
{
    if(freehead != NULL)
    {
        freetail->next = (*new_frame);
        freetail = (*new_frame);
        freetail->next = NULL;
    }
    else
    {
        freehead = freetail = (*new_frame);
        freetail->next = NULL;
    }
}

frame_ptr f_deq()
{
    if(freehead == NULL)
        return NULL;
    else
    {
        frame_ptr temp = freehead;
        if(freehead != freetail)
        {
            freehead = freehead->next;
        }
        else
            freehead = freetail = NULL;
        return temp;
    }
}

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

void set_frame()
{
    for(i=0; i<m; i++)
    {
        frame_ptr newtemp = malloc(sizeof(frame));
        newtemp->f_num = i;
        newtemp->vpn = 0;
        newtemp->free = 1;
        newtemp->process = -1;
        newtemp->next = NULL;
        f_enq(&newtemp);
    }
}

void set_pt()
{
    PT = (PTelement **)malloc(sizeof(PTelement *)*total_p);
    for(i=0; i<total_p; i++)
    {
        PT[i] = (PTelement *)malloc(sizeof(PTelement)*n);
        for(int j=0; j< n; j++)
        {
            PT[i][j].pfn_dbn = -1;
            PT[i][j].ref = 0;
            PT[i][j].present = 0;
        }
    }
}

void set_list()
{
    ghead = gtail = NULL;
    gptr = &ghead;
    lhead = (frame **)malloc(sizeof(frame *)*total_p);
    ltail = (frame **)malloc(sizeof(frame *)*total_p);
    lptr = (frame ***)malloc(sizeof(frame **)*total_p);
    for(i=0; i<total_p; i++)
    {
        lhead[i] = ltail[i] = NULL;
        lptr[i] = &lhead[i];
    }
}

void set_access_table()
{
    total_access = malloc(sizeof(int)*total_p);
    effective_access = malloc(sizeof(int)*total_p);
    page_fault = malloc(sizeof(int)*total_p);
    origin_access = malloc(sizeof(int)*total_p);
    for(i=0; i<total_p; i++)
    {
        total_access[i] = 0;
        effective_access[i] = 0;
        page_fault[i] = 0;
        origin_access[i] = 0;
    }
}

void initialize()
{
    set_TLB();
    set_frame();
    set_pt();
    set_list();
    set_access_table();
}

void mark_used(int pfn, char process)
{
    if(strcmp(frame_policy, "GLOBAL") == 0)
    {
        frame_ptr temp = ghead;
        while(temp->next != ghead)
        {
            if(temp->f_num == pfn)
            {
                temp->used = 1;
                return;
            }
            else
                temp = temp->next;
        }
    }
    else
    {
        frame_ptr temp = lhead[process-'A'];
        while(temp->next != ghead)
        {
            if(temp->f_num == pfn)
            {
                temp->used = 1;
                return;
            }
            else
                temp = temp->next;
        }
    }
}

int search_tlb(char process, int page)
{
    if((process-'A') != tlb_process)
    {
        tlb_process = process-'A';
        for(i=0; i<32; i++)
            TLB[i].clean = 0;
        return 0;
    }
    else
    {
        for(i=0; i<32; i++)
        {
            if(TLB[i].clean == 0)
                continue;
            if(TLB[i].vpn == page)
            {
                TLB[i].timestamp = timer;
                fprintf(f_out, "Process %c, TLB Hit, %d=>%d\n"
                        , process, page, TLB[i].pfn);
                mark_used(TLB[i].pfn, process);
                return 1;
            }
        }
        return 0;
    }
}

int tlb_replacement()
{
    if(strcmp(tlb_policy, "LRU")==0)
    {
        int out = 0;
        for(i=1; i<32; i++)
        {
            if(TLB[i].timestamp < TLB[out].timestamp)
                out = i;
            else continue;
        }
        return out;
    }
    else
        return rand()%32;
}

void change_tlb(char process, int page)
{
    int target = -1;
    for(i=0; i<32; i++) //free table block
    {
        if(TLB[i].clean == 0)
        {
            target = i;
            break;
        }
    }
    if(target == -1)//find one to replacement
        target = tlb_replacement();
    TLB[target].clean = 1;
    TLB[target].vpn = page;
    TLB[target].pfn = PT[process-'A'][page].pfn_dbn;
    TLB[target].timestamp = timer;
    return;
}

int search_pt(char process, int page)
{
    if(PT[process-'A'][page].ref == 1)
    {
        if(PT[process-'A'][page].present == 1)
        {
            fprintf(f_out, "Process %c, TLB Miss, Page Hit, %d=>%d\n"
                    , process, page, PT[process-'A'][page].pfn_dbn);
            mark_used(PT[process-'A'][page].pfn_dbn, process);
            change_tlb(process, page);
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}

int set_freeframe(char process, int page)
{
    if(freehead == NULL)
        return 0;
    else
    {
        frame_ptr temp = f_deq();
        temp->free = 0;
        temp->vpn = page;
        temp->process = process;
        temp->used = 1;
        if(strcmp(frame_policy, "GLOBAL") == 0)
            enq(&temp, &ghead, &gtail);
        else
            enq(&temp, &lhead[process-'A'], &ltail[process-'A']);
        fprintf(f_out, "Process %c, TLB Miss, Page Fault, %d, Evict -1 of Process %c to -1, %d<<%d\n"
                , process, temp->f_num, process, page, PT[process-'A'][page].pfn_dbn);
        //modified page table
        PT[process-'A'][page].pfn_dbn = temp->f_num;
        PT[process-'A'][page].present = 1;
        PT[process-'A'][page].ref = 1;
        //modified TLB
        change_tlb(process, page);
        return 1;
    }
}

frame_ptr fifo_replace(char process)
{
    int target;
    frame_ptr temp;
    if(strncmp(frame_policy, "GLOBAL", 6) == 0)
    {
        target = ghead->f_num;
        temp = deq(target, &ghead, &gtail);
    }
    else
    {
        target = lhead[process-'A']->f_num;
        temp = deq(target, &lhead[process-'A'], &ltail[process-'A']);
    }
    for(i=0; i<32; i++)
        if(TLB[i].pfn == target)
            TLB[i].clean = 0;
    return temp;
}

frame_ptr clock_replace(char process)
{
    frame_ptr temp;
    if(strncmp(frame_policy, "GLOBAL", 6) == 0)
    {
        while(1)
        {
            if((*gptr)->used)
            {
                (*gptr)->used = 0;
                (*gptr) = (*gptr)->next;
            }
            else
            {
                temp = (*gptr);
                (*gptr) = (*gptr)->next;
                break;
            }
        }
    }
    else
    {
        while(1)
        {
            if((*lptr[process-'A'])->used)
            {
                (*lptr[process-'A'])->used = 0;
                (*lptr[process-'A']) = (*lptr[process-'A'])->next;
            }
            else
            {
                temp = (*lptr[process-'A']);
                (*lptr[process-'A']) = (*lptr[process-'A'])->next;
                break;
            }
        }
    }
    for(i=0; i<32; i++)
    {
        if(TLB[i].pfn == temp->f_num)
            TLB[i].clean = 0;
    }
    return temp;
}

int find_block(int source)
{
    int dest;
    DBlock * target = NULL;
    DBlock * temp = dbhead;
    while(temp != NULL)
    {
        if(temp->full)
            temp = temp->next;
        else
        {
            target = temp;
            break;
        }
    }
    if(target == NULL)
    {
        target = malloc(sizeof(DBlock));
        target->block_id = total_block;
        total_block++;
        if(dbhead != NULL)
        {
            dbtail->next = target;
            dbtail = target;
            dbtail->next = NULL;
        }
        else
            dbhead = dbtail = target;
    }
    target->full = 1;
    dest = target->block_id;
    if(source != -1)
    {
        temp = dbhead;
        while(temp!= NULL)
        {
            if(temp->block_id == source)
            {
                temp->full = 0;
                break;
            }
            temp = temp->next;
        }
    }
    return dest;
}

void set_framelist(char process, int page)
{
    int disk_dest;
    frame_ptr replace;
    if(strncmp(page_policy, "FIFO", 4) == 0)
        replace = fifo_replace(process);
    else
        replace = clock_replace(process);
    //find disk to put
    disk_dest = find_block(PT[process-'A'][page].pfn_dbn);
    PT[(replace->process)-'A'][replace->vpn].pfn_dbn = disk_dest;
    PT[(replace->process)-'A'][replace->vpn].present = 0;
    PT[(replace->process)-'A'][replace->vpn].ref = 1;
    //print message
    fprintf(f_out, "Process %c, TLB Miss, Page Fault, %d, Evict %d of Process %c to %d, %d<<%d\n"
            , process, replace->f_num, replace->vpn, replace->process, disk_dest, page, PT[process-'A'][page].pfn_dbn);
    //frame allocation
    replace->vpn = page;
    replace->free = 0;
    replace->process = process;
    replace->used = 1;
    if(strncmp(page_policy, "FIFO", 3) == 0)
        if(strncmp(frame_policy, "GLOBAL", 6) == 0)
            enq(&replace, &ghead, &gtail);
        else
            enq(&replace, &lhead[process-'A'], &ltail[process-'A']);
    //change page table
    PT[process-'A'][page].pfn_dbn = replace->f_num;
    PT[process-'A'][page].present = 1;
    PT[process-'A'][page].ref = 1;
    //change TLB
    change_tlb(process, page);
    return;
}

void tracing()
{
    char curr_process;
    int curr_page;
    int tlb_hit, pt_hit, f_free;
    while(fscanf(f_in, "Reference(%c, %d)\n", &curr_process, &curr_page)!=EOF)
    {
        tlb_hit = 0;
        origin_access[curr_process-'A']++;
        while(tlb_hit==0)
        {
            timer++;
            total_access[curr_process-'A']++;
            fflush(f_out);
            tlb_hit = search_tlb(curr_process, curr_page);
            if(tlb_hit)
            {
                effective_access[curr_process-'A']++;
                continue;
            }
            pt_hit = search_pt(curr_process, curr_page);
            if(pt_hit) continue;
            page_fault[curr_process-'A']++;
            f_free = set_freeframe(curr_process, curr_page);
            if(f_free) continue;
            set_framelist(curr_process, curr_page);
        }
    }
    fclose(f_in);
}

void analysis()
{
    for(i=0; i<total_p; i++)
    {
        double fault_rate = (double)page_fault[i]/(double)origin_access[i];
        double hit_rate = (double)effective_access[i]/(double)total_access[i];
        double access = hit_rate*(120) + (1-hit_rate)*(220);
        fprintf(f_ana, "Process %c, Effective Access Time = %.3f\n", i+'A', access);
        fprintf(f_ana, "Process %c, Page Fault rate = %.3f\n", i+'A', fault_rate);
    }
}

int main()
{
    srand(time(NULL));
    open_file();
    get_config();
    initialize();
    tracing();
    analysis();
    fclose(f_out);
    fclose(f_ana);
    return 0;
}