#include "db.h"

//数据库配置信息
static string server = "127.0.0.1";
static string user = "admin";
static string password = "rootroot";
static string dbname = "practice";
static LOG log("mysqlerr.log");

//初始化数据库连接
MySQL::MySQL()
{
	_conn = mysql_init(nullptr);
}


//释放数据库连接资源
MySQL::~MySQL()
{
	if(_conn!=nullptr)
	{
		//log.write("mysql连接失败/n");
		mysql_close(_conn);
	}

}

//连接数据库

bool MySQL::connect()
{
	MYSQL *p = mysql_real_connect(_conn,server.c_str(),user.c_str(),
		password.c_str(),dbname.c_str(),3306,nullptr,0);
	if(p!=nullptr)
	{
		//C和C++代码默认的编码字符是ASCII，如果不设置，从MYSQL上拉下来的中文是乱码
		//mysql_query(_conn,"set names utf8");
		mysql_set_character_set(_conn,"utf8");
		log.write("connect mysq success\n");
	}
	
	else
	{
		//这里应该记下错误日志
		log.write("connect mysql fail\n");
	}
	return p;
}

//更新操作
bool MySQL::update(string sql)
{
	//mysql_autocommit(_conn,0);
	if(mysql_query(_conn,sql.c_str()))
	{
		//日志记下错误
		log.write("update error\n");
		//mysql_rollback(_conn);
		return false;
	}
	//mysql_commit(_conn);
	return true;
}


//查询操作
MYSQL_RES *MySQL::query(string sql)
{
	if(mysql_query(_conn,sql.c_str()))
	{
		//日志记下 ”查询失败“
		log.write("query failed\n");
		return nullptr;
	}
	return mysql_use_result(_conn);
	
}


//获取连接
MYSQL* MySQL::getConnection()
{
	return _conn;
}

