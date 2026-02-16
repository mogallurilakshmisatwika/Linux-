#include<stdio.h>
#include<string.h>

#define KERNEL_SIZE 100
#define USER_SIZE 100

char userspace[USER_SIZE];
char kernelspace[KERNEL_SIZE];


size_t safecopy(char *dest, char *src, size_t size, char *base, size_t limit)
{
        if(!dest || !src)
                return size;
        if(src<base)
                return size;
        size_t max_copy=(base+limit)-src;
        if(max_copy<=0)
                return size;
        size_t bytes_to_copy=(size<max_copy)?size:max_copy;

        memcpy(dest,src,bytes_to_copy);

        return size-bytes_to_copy;
}
int main()
{
        strcpy(userspace,"Hello from user");
        printf("user space:%s\n",userspace);
        if(safecopy(kernelspace,userspace,strlen(userspace)+1,userspace,USER_SIZE)==0)
        {
                printf("copy sucess\n");
                printf("kernel space:%s\n",kernelspace);
        }

        printf("copy failed case\n");
        size_t not_copied=safecopy(kernelspace,userspace+95,20,userspace,USER_SIZE);
        if(not_copied==0)
        {
                printf("copy sucess\n");
        }
        else
        {
                printf("copy failed, %zu bytes not copied\n",not_copied);
        }
}