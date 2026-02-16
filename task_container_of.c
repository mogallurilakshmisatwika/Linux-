#include<stdio.h>
#include<stddef.h>

#define container_of(ptr,type,member) ((type *)((char *)(ptr)-offsetof(type,member)))

struct listheader{
        struct listheader *next;
        struct listheader *prev;
};

struct task{
        int data;
        int pid;
        struct listheader list;
};

struct task *get_task_from_list(struct listheader *lh)
{
         size_t offset = offsetof(struct task, list);
         printf("Offset=%zu\n",offset);
         return container_of(lh,struct task,list);
}

int main(void)
{
        struct task t;
        t.data=1;
        t.pid=1000;
        struct listheader *lh = &t.list;
        struct task *p = get_task_from_list(lh);
        printf("data= %d\n",p->data);
        printf("pid= %d\n",p->pid);
}
~
~