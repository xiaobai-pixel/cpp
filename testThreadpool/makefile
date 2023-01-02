all:clean threadpool

threadpool:
	g++ -g -o threadpool main.cpp locker.h worker.h threadpool.h -lpthread

clean:
	rm threadpool