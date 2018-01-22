// DATABASE.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include "MySystem.h"
using namespace std;
void Version(); //版本号，基本说明
int main(int argc, char* argv[])
{
	MySystem S;
	int flag;
//	Version();
	S.PowerOnDataBase();
	S.LogOn();
	while(1)
	{
//		S.PrintTables();
		S.ClearCommand();
		S.InputCommand();
//		cout<<"command:"<<S.GetCommand()<<endl;
		if(S.GetCommand()=="exit" )
			break;
		if(!S.AnalysisCommand(flag) )
		{
		    cout<<"命令错误"<<endl;
		}
		else
		{
//			cout<<"正确命令  "<<endl;
			S.DealWithCommand(flag);
		}
		
	}
	return 0;
}
void Version()
{
	cout<<"支持换行，以分号结束命令"<<endl;
	cout<<"所有命令小写"<<endl;
	cout<<"输入\"exit;\"退出"<<endl;
	cout<<"支持的最大元组数 "<<ElementsCol::MaxLine<<endl;
	cout<<"支持的最大属性个数 "<<ElementsCol::MaxColumn<<endl;
	cout<<"insert 不支持交叉赋值"<<endl;
	cout<<"默认第一个属性当主键"<<endl;
	cout<<"insert第一个属性必须是主键"<<endl;
	cout<<"输入多个属性时属性的相对位置不能发生变化"<<endl;
	cout<<"本实验数据结构定义失误，导致很多操作变得相当繁琐(将表的元组和表的属性定义为两个类，本以为方便了，实际上复杂了)"<<endl;
	cout<<"创建索引支持一个属性值，不知道多个属性值有啥用？估计是多个键属性吧"<<endl;
	cout<<"由于在建立主索引的时候排序，所以辅助索引要在主索引建立之前，不然在建立主索引的时候会改变元组的次序;按理来说创建索引是不应该改变元组集合的"<<endl;
	cout<<"没有维护索引"<<endl;
	cout<<"默认单表属性名类型为name的形式"<<endl;
	cout<<"多表为R.name的形式"<<endl;
	cout<<"连接条件的属性名必须相等R.name=R1.name"<<endl;
	cout<<"多表操作时连接条件只能写在选择条件的前面"<<endl;
	cout<<"连接条件之间只能以and的形式连接"<<endl;
	cout<<"连接目前只支持2个表"<<endl;
	cout<<"不支持以零开头的数字"<<endl;
	cout<<"多表操作没有对属性名验证,可能出现未定义错误"<<endl;
	cout<<"为了利用索引方便，主索引也是全部项"<<endl;
	//2015.6.18修改
	cout<<"使用3个表及以上为查询优化处理"<<endl;
	cout<<"查询优化选择条件只能以and连接，不能使用or"<<endl;
	//2015.6.19添加
	cout<<"注册用户未查重"<<endl;
}

//2015.6.16发现bug，如果values的常量为'0',插入失败
