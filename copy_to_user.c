#include<stdio.h>
#include<string.h>

#define USER_SIZE 100
#define KERNEL_SIZE 100

char userspace[USER_SIZE];
char kernelspace[KERNEL_SIZE];

size_t safecopy(char *dest, char *src, int size, char *base, int limit)
{
        if(dest<base)
        {
                printf("destination below valid memory\n");
                return size;
        }
        if(!dest || !src)
        {
                return size;
        }
        size_t max_copy=(base+limit)-dest;
        if(max_copy<=0)
                return size;
        size_t bytes_to_copy=(size<max_copy)?size:max_copy;
        memcpy(dest,src,bytes_to_copy);
        //strncpy(dest,src,bytes_to_copy);
        //strcpy(dest,src);
        return size-bytes_to_copy;
}
int main()
{
        strcpy(kernelspace,"hello from kernel");
        printf("kernel space :%s\n",kernelspace);
        if(safecopy(userspace,kernelspace,strlen(kernelspace)+1,userspace,USER_SIZE)==0)
        {
                printf("safe copy sucess\n");
                printf("userspace:%s\n",userspace);
        }
        printf("copy invalid case\n");

        size_t not_copied=safecopy(userspace+95,kernelspace,20,userspace,USER_SIZE);
        if(not_copied==0)
        {
                printf("copy sucess\n");
        }
        else
        {
                printf("copy failed, %zu bytes not copied\n",not_copied);
        }
}
