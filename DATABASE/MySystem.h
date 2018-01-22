//在MySystem中处理语法
//在DataBase中处理语义

#pragma once
#include "MICRO.h"
#include "DataBase.h"
#include <string>
#include <vector>
using namespace std;
struct AuthorStr
{
	string User;
	string Table;
	string Author;
};
class MySystem
{
private:
	string command; //命令
public:
	inline void ClearCommand(){command.clear();};//清空命令
	void InputCommand();
	bool AnalysisCommand(int &flag);//分析命令， flag[out]为对应的命令的宏标志 ;返回值为真,表明命令正确
	void DealWithCommand(int flag);//根据宏标志处理命令
//辅助函数
public:
	inline string GetCommand(){return command;}
//	私有辅助函数
private:
	void    CreateTable();//创建表格
	void 	DropTable();
	void 	AlterTableAdd();
	void 	AlterTableDrop();
	void 	Insert();
	void 	myDelete();
	void 	Update();
	void 	Select();
	void 	CreateIndex();
	void 	DropIndex();
	void 	CreateUser();
	void 	Grant();
	void     Revoke();
	//创建表格所定义的函数和成员变量
private:
	DataBase database;
public:
	inline void WriteToFile(){database.WriteToFile();}//将所有数据写入文件中
	inline void PowerOnDataBase(){database.PowerOnDataBase();}
	inline void PrintTables(){database.PrintDictionary();}
private:
	vector<string> SpliteCommand();//分割字符串
	//连接条件而增加的函数
public:
	//功能：将Exp中的字符流分割为连接条件和选择条件
	//参数:[out]Link,[out]Choose
	bool Seperate(vector<string>Exp,vector<LinkCondi> &Link,vector<string> &Choose);


//权限
public:
	//功能：使用用户名和密码登陆
	void LogOn();
//查询优化
public:
	//功能：查询优化
	//参数:Table.size()>=3,Field不含*,所有属性能存在表中,且expstream中连接条件和选择条件都正确
	void QueryOptimize(vector<string> Table,vector<string> Field,vector<string> expstream);

};

//********************查询优化********************//


//功能:初始关系树
//参数：Table.size()>=3,Field不含*,所有属性能存在表中,且expstream中连接条件和选择条件都正确
void InitilizeTree(vector<pair<string,int> > &Tree, vector<string> Table, vector<string> expstream, vector<string> Field);
