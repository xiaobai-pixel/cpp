#ifndef LOG_H
#define LOG_H
#include<fstream> //ofstream类需要包含的头文件
#include<iostream>
#include<stdio.h>
using namespace std;
class LOG
{
public:
	
	ofstream fout;
	LOG(string file)
	{
		fout.open(file,ios::app);

	}
	void write(string s)
	{
		//s += "\n";
		fout<<s;
	}
	~LOG()
	{
		fout.close();
	}
};

#endif
