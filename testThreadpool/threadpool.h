/*此文件是用来作为线程池使用，文件名:threadpool.h*/
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<list>
#include<cstdio>
#include<exception>
#include<pthread.h>
#include "locker.h"


template<typename T>
class threadpool
{
public:
	threadpool(int thread_number=8,int max_requests=1000);
	~threadpool();
	bool append(T* request);

private:
	/*工作线程*/
	static void* worker(void* arg);
	void run();
private:
	int m_thread_number; //线程数量
	int m_max_requests;  //最大处理数
	pthread_t* m_threads; //线程指针
	std::list<T*> m_workqueue; //请求对列
	locker m_queuelocker;   //线程锁
	sem m_queuestat;		//信号量
	bool m_stop;            //是否让线程停下来
};


template<typename T>
threadpool<T>::threadpool(int thread_number,int max_requests):
		m_thread_number(thread_number),m_max_requests(max_requests),m_stop(false),m_threads(NULL)
{
	if((thread_number<=0)||(max_requests<=0))
	{
		/*如果线程数量小于等于0或者最大请求数也小于等于0则抛出异常*/
		throw std::exception();
	}
	/*创建线程*/
    m_threads = new pthread_t[m_thread_number];
    if(!m_threads)
    {
        throw std::exception();
    }
	for(int i=0;i<thread_number;i++)
	{
		printf("create the %dth pthread\n",i);
		if(pthread_create(m_threads+i,NULL,worker,this)!=0)
		{
			/*创建失败则删除指针并抛出异常*/
			delete [] m_threads;
			throw std::exception();
		}
		/*分离线程,分离失败，删除指针和抛出异常*/
		if(pthread_detach(m_threads[i]))
		{
			delete [] m_threads;
			throw std::exception();
		}
	}
}

template<typename T>
threadpool<T>::~threadpool()
{
	delete [] m_threads;
	m_stop=true;
}

/*创建任务队列*/
template<typename T>
bool threadpool<T>::append(T* request)
{
	m_queuelocker.lock(); /*因为任务对列是所有线程共享的，所以要加锁*/
	/*任务对列的最大长度超过了要求的最大长度则表示不能再插入任务了，创建任务失败*/
	if(m_workqueue.size()>m_max_requests)
	{
		m_queuelocker.unlock();
		return false;
	}
	/*插入任务对列*/
	m_workqueue.push_back(request);
	/*解锁，信号量加1*/
	m_queuelocker.unlock();
	m_queuestat.post();
	return true;
}

template<typename T>
void* threadpool<T>::worker(void *arg)
{
	threadpool* pool = (threadpool*)arg;//强制转换类型
	pool->run();
	return pool;
}

template<typename T>
void threadpool<T>::run()
{
	while(!m_stop)
	{
		m_queuestat.wait();  //等待信号量
		m_queuelocker.lock();  //等待锁
		if(m_workqueue.empty())
		{
			/*任务对列为空*/
			m_queuelocker.unlock();
			continue;
		}
		T* request = m_workqueue.front();//取出一个任务
		m_workqueue.pop_front();
		m_queuelocker.unlock();
		if(!request)
		{
			continue;
		}
		request->process();
	}
}

#endif







// #ifndef THREADPOOL_H
// #define THREADPOOL_H

// #include <list>
// #include <cstdio>
// #include <exception>
// #include <pthread.h>
// #include "locker.h"

// template< typename T >
// class threadpool
// {
// public:
//     threadpool( int thread_number = 8, int max_requests = 10000 );
//     ~threadpool();
//     bool append( T* request );

// private:
//     static void* worker( void* arg );
//     void run();

// private:
//     int m_thread_number;
//     int m_max_requests;
//     pthread_t* m_threads;
//     std::list< T* > m_workqueue;
//     locker m_queuelocker;
//     sem m_queuestat;
//     bool m_stop;
// };

// template< typename T >
// threadpool< T >::threadpool( int thread_number, int max_requests ) : 
//         m_thread_number( thread_number ), m_max_requests( max_requests ), m_stop( false ), m_threads( NULL )
// {
//     if( ( thread_number <= 0 ) || ( max_requests <= 0 ) )
//     {
//         throw std::exception();
//     }

//     m_threads = new pthread_t[ m_thread_number ];
//     if( ! m_threads )
//     {
//         throw std::exception();
//     }

//     for ( int i = 0; i < thread_number; ++i )
//     {
//         printf( "create the %dth thread\n", i );
//         if( pthread_create( m_threads + i, NULL, worker, this ) != 0 )
//         {
//             delete [] m_threads;
//             throw std::exception();
//         }
//         if( pthread_detach( m_threads[i] ) )
//         {
//             delete [] m_threads;
//             throw std::exception();
//         }
//     }
// }

// template< typename T >
// threadpool< T >::~threadpool()
// {
//     delete [] m_threads;
//     m_stop = true;
// }

// template< typename T >
// bool threadpool< T >::append( T* request )
// {
//     m_queuelocker.lock();
//     if ( m_workqueue.size() > m_max_requests )
//     {
//         m_queuelocker.unlock();
//         return false;
//     }
//     m_workqueue.push_back( request );
//     m_queuelocker.unlock();
//     m_queuestat.post();
//     return true;
// }

// template< typename T >
// void* threadpool< T >::worker( void* arg )
// {
//     threadpool* pool = ( threadpool* )arg;
//     pool->run();
//     return pool;
// }

// template< typename T >
// void threadpool< T >::run()
// {
//     while ( ! m_stop )
//     {
//         m_queuestat.wait();//�ź���
//         m_queuelocker.lock();//��
//         if ( m_workqueue.empty() )
//         {
//             m_queuelocker.unlock();
//             continue;
//         }
//         T* request = m_workqueue.front();
//         m_workqueue.pop_front();
//         m_queuelocker.unlock();
//         if ( ! request )
//         {
//             continue;
//         }
//         request->process();
//     }
// }

// #endif
