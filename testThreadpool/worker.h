#ifndef WORKER_H
#define WORKER_H

#include<stdio.h>



class Worker
{
public:
    Worker(){};
    void process()
    {
        int sum=0;
        for(int i=0;i<10;i++)
        {
            sum += i;
        }
        printf("sum=%d\n",sum);
    }
};

#endif