# hw4-Memory-Manager

## Build the Project

- Install githook and compile output file `MemManager`

```
make
```

- delete output file
```
make clean
```

## Usage

- execute

```
./MemManager
```

- Then, the process will read `sys_config.txt` to decide its policy. In this input type, we have below parameters.  
  - TLB Replacement Policy: `LRU` (least recently use) or `RANDOM`

  - Page Replacement Policy: `FIFO` (first in first out) or `CLOCK`
  
  - Frame Allocation Policy: `GLOBAL` or `LOCAL`

  - Number of Processes: total process in this simulation
  - Number of Virtual Page: number of virtual page in each process
  - Number of Physical Frame: number of total frame

- After reading configuration file, the process will read `trace.txt`, which is the reference input

  - input format: `Reference({process}, {page table number})`

- finally, we will produce our output file in `trace_output.txt`
  - inside the file is like:
  
  ```
  Process A, TLB Hit, 17=>3
  Process A, TLB Miss, Page Hit, 32=>15
  Process A, TLB Hit, 32=>15
  Process A, TLB Miss, Page Hit, 34=>14
  Process A, TLB Hit, 34=>14
  Process A, TLB Miss, Page Fault, 12, Evict 68 of Process A to 28, 49<<-1
  Process A, TLB Hit, 49=>12
  ```

- the process also produce `analysis.txt`, inside the file we record each process's effective access time and Page fault
  - effective access time: α(m+t) + (1- α)(2m+t)
    - $\alpha$: hit ratio (TLB hit/ total TLB lookup)
    - m: memory cycle time, assume 100ns
    - t: TLB lookup time, assume 20ns
  - page fault rate: page fault rate/ total reference, (total reference didn't include TLB lookup which is after TLB miss)

## Implementations

### basic structure

- page table element
  - variable:
    - pfn_dbn: it save page frame number when it is in frame, and save disk block number when it is inside disk
    - ref: reference bit, if the page is in frame or disk => 1, the process hasn't touch => 0
    - present: if the page is in frame => 1, in disk => 0
  - each process has one array, and the size is Number of Virtual Page

- TLB element
  - variable:
    - vpn: virtual page number
    - pfn: page frame number
    - timestamp: latest access time, used in LRU policy
    - clean: if this element is full or free, full = 1, free = 0
  - Also have one variable `tlb_process`, it can record which process use TLB now
  - When we change from process A to B, clean bit will change to 0
  - in all program, we have an array TLB[32]
  - when we evict a frame, we will change the page table first and then refill the page table

- DiskBlock
  - variable:
    - block_id: disk block number 
    - full: full bit, full = 1, free = 0
    - next: the pointer pointed to next disk block
  - the list of disk block is starts from id=0, then it will have id=1, 2, 3...
  - in all program, we have a variable `total_block` to record how many disk block we create.
  - each time when we need a disk block, we will give it the smallest id
  - when the page move from disk to frame, the disk block will turned free, it can refill another page

- frame
  - variable:
    - f_num: frame number
    - vpn: virtual page number
    - free: free frame = 1, full = 0
    - process: record it belongs to which process, used when output process information
    - used: if it used, marked as 1, when the clock pointer passed, 1 will turned to 0, 0 will be the victim
    - next: the pointer pointed to next frame
  - we have three types of frame list: free list, global list, and local list
  - in the begin, all frame is clean and will in free list
  - in global allocation, the program have one global list, when we allocate a pge, the frame node will move to global list
    - when we need to evict a list, we will pick from this list
    - if we use clock policy, we will have a pointer, we use it to choose victim, and the pointer pointed at the list head initially
  - in local allocation, each process have one list to save its own frame, and each process have their unique frame pointer
  - in clock policy, after we choose a victim, the frame will refill but doesn't move relative position