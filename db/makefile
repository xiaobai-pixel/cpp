#mysql头文件存放的目录
MYSQLINCL = -I/usr/include/mysql
 
#mysql库文件存放的目录
MYSQLLIB = -L/usr/lib64/mysql
 
#需要链接的mysql库
 
MYSQLLIBS = -lmysqlclient

all:mysqlconnect

mysqlconnect:
	g++ -g  -o test test.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS)  db.h db.cpp  -lm -lc -std=c++11 
