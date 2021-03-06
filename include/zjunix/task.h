#ifndef _ZJUNIX_TASK_H
#define _ZJUNIX_TASK_H


#include<zjunix/fs/fat.h>
#include<page.h>


//进程状态定义
#define STATE_WAITTING 0
#define STATE_READY    1
#define STATE_RUNNING  2
//默认时间片
#define DEFAULT_TIMESLICES 6
//进程优先级
#define HIGH_PRIORITY 13
#define ABOVE_NORMAL_PRIORITY 10
#define NORMAL_PRIORITY 8
#define BELOW_NORMAL_PRIORITY 6
#define IDLE_PRIORITY 4




typedef struct list_pcb list_pcb;
typedef struct task_struct PCB;
typedef struct {
    unsigned int epc;
    unsigned int at;
    unsigned int v0, v1;
    unsigned int a0, a1, a2, a3;
    unsigned int t0, t1, t2, t3, t4, t5, t6, t7;
    unsigned int s0, s1, s2, s3, s4, s5, s6, s7;
    unsigned int t8, t9;
    unsigned int hi, lo;
    unsigned int gp;
    unsigned int sp;
    unsigned int fp;
    unsigned int ra;
} context;

typedef struct{
    context *context;
    unsigned int tid;
}TCB;

struct list_pcb{
    list_pcb  *prev;
    list_pcb  *next;
    PCB * pcb;
};
struct task_struct{
    context *context;//上下文

    char name[32];
    unsigned char asid;//address space id   pid
    unsigned int parent;//父进程id
    int uid;//user id 
    unsigned int counter;//时间片
    pgd_term *pgd;//页目录（第一级页表）
    unsigned long start_time;
    unsigned int state;
    unsigned int priority;//优先级
    short policy;//对该进程使用的调度策略

    list_pcb sched;//就绪队列
    list_pcb process;//pcb链表


    struct TCB *thread_head;//指向进程中的第一个线程
    unsigned int  num_thread;//线程数量  
    
    FILE * file;//文件信息

    struct shared_memory* shm;

    // struct list_head stack_vma_head;//用户栈表头
    // struct list_head code_vma_head;//代码区表头
    // struct list_head heap_vma_head;//数据区表头
    // unsigned int user_stack;//用户态栈顶指针
    // unsigned int user_code;//用户态代码起始地址
};



typedef union{
    PCB pcb;
    unsigned char kernel_stack[4096];
}task_union;


#define KERNEL_STACK_SIZE 4096


list_pcb pcbs;//进程队列
unsigned char idmap[32];//设置256个进程id

void task_test();
//init进程设置以及相关中断、系统调用注册
void init_task();

//分配一个进程号
unsigned char get_emptypid();

//把一个进程pcb添加到链表末尾
void add_task(list_pcb* process);

//删除进程
unsigned int  del_task(unsigned int asid);

unsigned int do_fork(context* args,PCB*parent);

pgd_term *copy_pagetables(PCB * parent);

#endif