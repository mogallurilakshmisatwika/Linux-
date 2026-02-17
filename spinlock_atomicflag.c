#include<stdio.h>
#include<pthread.h>
#include<stdatomic.h>

atomic_flag flag=ATOMIC_FLAG_INIT;
int shared_var=0;

void spin_lock()
{
        while(atomic_flag_test_and_set(&flag));
}

void spin_unlock()
{
        atomic_flag_clear(&flag);
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
        printf("shared varaibale value:%d\n",shared_var);
}