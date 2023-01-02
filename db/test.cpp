#include<stdio.h>
#include "log.h"
#include "db.h"
int main()
{
	MySQL conn;
	conn.connect();
	string sql = "select * from file_info";
	//conn.query(sql);
	char sql1[] = "insert into file_info (md5,file_id,url)values('md5','file','jpg')";
	if(conn.update(sql1)) printf("update success!\n");
	else printf("update failed\n");
	return 0;
}
