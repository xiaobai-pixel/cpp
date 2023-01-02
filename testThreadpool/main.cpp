#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include "locker.h"
#include "worker.h"
#include "threadpool.h"


int main()
{
    Worker* work = new Worker[8];
    threadpool<  Worker >* pool =new threadpool<  Worker >;
    //pool = ;
    for(int i=0;i<8;i++){
        pool->append(work+i);
    }
    sleep(1000000);
}