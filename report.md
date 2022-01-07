# Report

## TLB Replacement Policy

- LRU 
  - sys_config.txt

    ```
    TLB Replacement Policy: LRU
    Page Replacement Policy: FIFO
    Frame Allocation Policy: GLOBAL
    Number of Processes: 2
    Number of Virtual Page: 128
    Number of Physical Frame: 64
    ```
  
  - analysis.txt

    ```
    Process A, Effective Access Time = 164.758
    Process A, Page Fault rate = 0.723
    Process B, Effective Access Time = 163.709
    Process B, Page Fault rate = 0.665
    ```

- RANDOM
  - sys_config.txt

    ```
    TLB Replacement Policy: RANDOM
    Page Replacement Policy: FIFO
    Frame Allocation Policy: GLOBAL
    Number of Processes: 2
    Number of Virtual Page: 128
    Number of Physical Frame: 64
    ```

  - analysis.txt
    ```
    Process A, Effective Access Time = 165.200
    Process A, Page Fault rate = 0.723
    Process B, Effective Access Time = 163.333
    Process B, Page Fault rate = 0.665
    ```

- 若範例input來看，LRU和RANDOM並沒有造成太大的差距，稍微可以看出使用RANDOM的時候Process A的TLB miss比較多，Process B的TLB miss比較少，因為TLB miss 但Page hit的時候會多一次查詢時間，但不會增加Page Fault
- 造成這個結果可能是因為input file並沒有很規律的重複reference相同的page，如果process一直reference差不多的page的時候，LRU的效果會比reference好

## Page Replacement Policy

- FIFO 
  - sys_config.txt

    ```
    TLB Replacement Policy: LRU
    Page Replacement Policy: FIFO
    Frame Allocation Policy: GLOBAL
    Number of Processes: 2
    Number of Virtual Page: 128
    Number of Physical Frame: 64
    ```
  
  - analysis.txt

    ```
    Process A, Effective Access Time = 164.758
    Process A, Page Fault rate = 0.723
    Process B, Effective Access Time = 163.709
    Process B, Page Fault rate = 0.665
    ```

- CLOCK
  - sys_config.txt

    ```
    TLB Replacement Policy: LRU
    Page Replacement Policy: CLOCK
    Frame Allocation Policy: GLOBAL
    Number of Processes: 2
    Number of Virtual Page: 128
    Number of Physical Frame: 64
    ```

  - analysis.txt
    ```
    Process A, Effective Access Time = 164.758
    Process A, Page Fault rate = 0.723
    Process B, Effective Access Time = 163.709
    Process B, Page Fault rate = 0.665
    ```

- 兩者看起來並沒有差異，因此我準備了LOCAL的來做比對

- FIFO 
  - sys_config.txt

    ```
    TLB Replacement Policy: LRU
    Page Replacement Policy: FIFO
    Frame Allocation Policy: LOCAL
    Number of Processes: 2
    Number of Virtual Page: 128
    Number of Physical Frame: 64
    ```
  
  - analysis.txt

    ```
    Process A, Effective Access Time = 164.980
    Process A, Page Fault rate = 0.774
    Process B, Effective Access Time = 163.144
    Process B, Page Fault rate = 0.700
    ```

- CLOCK
  - sys_config.txt

    ```
    TLB Replacement Policy: LRU
    Page Replacement Policy: CLOCK
    Frame Allocation Policy: LOCAL
    Number of Processes: 2
    Number of Virtual Page: 128
    Number of Physical Frame: 64
    ```

  - analysis.txt
    ```
    Process A, Effective Access Time = 164.980
    Process A, Page Fault rate = 0.774
    Process B, Effective Access Time = 163.522
    Process B, Page Fault rate = 0.694
    ```
- 使用clock 的話有機會使Page fault rate在local的時候變小，如果頻繁接觸某幾個page的話，clock policy可以保留頻繁接觸的frame但fifo不行

## Frame Allocation Policy

- GLOBAL
  - sys_config.txt

    ```
    TLB Replacement Policy: LRU
    Page Replacement Policy: FIFO
    Frame Allocation Policy: GLOBAL
    Number of Processes: 2
    Number of Virtual Page: 128
    Number of Physical Frame: 64
    ```
  
  - analysis.txt

    ```
    Process A, Effective Access Time = 164.758
    Process A, Page Fault rate = 0.723
    Process B, Effective Access Time = 163.709
    Process B, Page Fault rate = 0.665
    ```
- LOCAL
  - sys_config.txt

    ```
    TLB Replacement Policy: LRU
    Page Replacement Policy: FIFO
    Frame Allocation Policy: LOCAL
    Number of Processes: 2
    Number of Virtual Page: 128
    Number of Physical Frame: 64
    ```
  
  - analysis.txt

    ```
    Process A, Effective Access Time = 164.980
    Process A, Page Fault rate = 0.774
    Process B, Effective Access Time = 163.144
    Process B, Page Fault rate = 0.700
    ```

- page fault的話global會有比local還要好的效果，因為global能夠根據當下process執行的狀況來分配適當的page，找到process比較夠用的page數量
- 但因為effective access rate受到TLB的影響比較多，所以影響沒有那麼大
- 另外,綜合frame policy和page policy，在範例測資中相同TLBpolicy下，frame policy對Page fault rate的影響會比較大