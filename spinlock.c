#include<stdio.h>
#include<pthread.h>

int flag=0;
int shared_var=0;

void spin_lock()
{
        while(flag);
        flag=1;
}

void spin_unlock()
{
        flag=0;
}

void *thread(void *data)
{
        for(int i=0;i<1000000;i++)
        {
                spin_lock();
                shared_var++;
                spin_unlock();
        }
        return NULL;
}
int main()
{
        pthread_t t1,t2;
        pthread_create(&t1,NULL,thread,NULL);
        pthread_create(&t2,NULL,thread,NULL);
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);
        printf("shared var=%d\n",shared_var);
}