#pragma once
#include "MICRO.h"
#include <string>
#include <vector>
#include <list>
#include "ElementsCol.h"
#include "TableIndex.h"
//#include <windows.h>
#include <time.h>
using namespace std;
//连接条件而增加的类、结构体和函数
struct LinkCondi
{
	string Table1;
	string Table2;
	string Field;
	LinkCondi(){}
	LinkCondi(string Table1,string Table2,string Field)
	{
		this->Table1=Table1;
		this->Table2=Table2;
		this->Field=Field;
	}
};
typedef struct  _DICELETYPE //数据字典单个元素定义
{
	string TableName; //表名
	ElementsCol *pElementsCol; //指向元组集合
	FieldPTypeList  *pFieldPlusTypeList; //指向《属性，属性类型》列表
	_DICELETYPE(){pElementsCol=NULL;pFieldPlusTypeList=NULL;}

}DICELETYPE;

 class DataBase
{
private:
    list<DICELETYPE> m_DataDic; //表集合
public:
	bool CreatTable(string tablename,list<FILEDPTYPE_TYPE> FieldPlusType );//注意，为了方便，我们对于char类型只传入char的数值，如‘char(15)’只传入‘15’
	void WriteToFile();//将数据写入文件中
	void PowerOnDataBase();//启动数据库:从文件中读取信息
	bool DropTable(string tablename);//删除表
	bool insert(string tablename,vector<string> Field,vector<string> value);//插入
	bool AlterTableAdd(string tablename,string Field,string Type);
	bool AlterTableDrop(string tablename,vector<string> Field);
	bool MyDelete(string tablename,vector<string> Exp);
	bool update(string tablename,vector< pair<string,string> > FieldValue,vector<expression> expstream);
	void Select(vector<string>  Field,vector<string> Table,vector<expression>  expstream);
	bool DropAllEle(string tablename);

//调试使用
public:
	void PrintDictionary();
//辅助函数,不需要输出任何信息
private:
	bool TableExist(string tablename,list<DICELETYPE>::iterator &pTable); //判断表格是否存在
	bool isMatch(vector< pair<string,string> >& FieldValue ,list<DICELETYPE>::iterator pEle); ////检测属性名是否匹配，属性类型是否正确；并将 属性名域修改为下标值
	bool isMatch(vector< expression >& FieldValue ,list<DICELETYPE>::iterator pEle); ////检测属性名是否匹配，属性类型是否正确；并将 属性名域修改为下标值
	bool isMatchField(list<DICELETYPE>::iterator pEle,vector<string> Field);//属性名是否在表的属性列表中
	bool isMatchType(list<DICELETYPE>::iterator pEle,vector<string> &Field,vector<string> value);//判断类型是否一致 
	void FieldToflags(list<DICELETYPE>::iterator pEle,vector<string> &Field);//将属性名转换为下标，必须保证Field中的所有属性都在pEle指向的表中
    //将pEle指向的所有属性赋给参数Field
	void AddAllField(list<DICELETYPE>::iterator pEle,vector<string> &Field);


private:
	vector<IndexStr> TableIndexArray;//所有表的索引
	//创建索引添加的函数
public:
	//创建索引,支持一个属性
	void CreateIndex(string TableName,string IndexName,vector<string> *Filed);
	//删除索引
	void DropIndex(string IndexName);
	//输出索引表
	void PrintIndexArray();

private:
	bool isHaveIndex(string TableName,string FieldName);//表是否在某属性上建立过索引
	//索引名是否已存在
	bool isIndexOnly(string IndexName);
	bool isMainField(list<DICELETYPE>::iterator pEle,string FieldName);//某属性是否是该表的主键
	//对主属性排序
	void SortOnMainField(list<DICELETYPE>::iterator pEle);
	 //建主索引,返回指向索引文件的指针
	IndexFile * CreatMainIndex(list<DICELETYPE>::iterator pEle);
	//建辅助索引,返回指向索引文件的指针
	IndexFile *  CreateAssistIndex(list<DICELETYPE>::iterator pEle,int pos);
	//将所有表的索引写入文件中
	void WriteTableIndexToFile();
	//读取表索引文件，调入内存
	void ReadTableIndexFromFile();

//Select添加的函数
public:
	//功能：多表操作（连接和选择）
	//参数：无要求
	void SelectMultiTab(vector<string> Table,vector<string> Field,vector<LinkCondi> Link,vector<expression> expstream);//多表操作
private:
	//功能：多个表的连接操作，投影所有属性
	//参数：Link中只有2个表、表属性存在 ,[out]NewField(新表属性列表)
	//返回值：连接后的表
	vector< vector<string> * > LinkTwoTab(LinkCondi Link,vector<string> &NewField);
	//功能：利用索引选择输出元组，属性值全部输出
	//参数：pTable有效，expstream中
	void PrintTabOnIndex(list<DICELETYPE>::iterator pTable,expression  xiabiaoexpstream);
private:
	//功能:输出表格
	//参数要求:pTable有效，不为空
	void PrintTab(list<DICELETYPE>::iterator pTable);
	//功能：输出满足表达式的元组
	//参数:pTable有效，expstream不为空(所有属性)
	void PrintTab(list<DICELETYPE>::iterator pTable,vector<expression> expstream);
	//功能:输出所有元组的某些属性值
	//参数:pTable有效，Field必须是下标
	void PrintTab(list<DICELETYPE>::iterator pTable,vector<string>  Field);
	//功能:输出满足条件元组的某些属性值
	//参数:pTable有效，Field必须是下标
	void PrintTab(list<DICELETYPE>::iterator pTable,vector<expression> expstream,vector<string>  Field);

//索引
public:
	//功能:利用索引连接两个表
	//参数:无要求
	vector< vector<string> *> LinkTwoTabOnIndex(LinkCondi Link,vector<string> NewField);
	//功能：判断表是否在Field域上建立了索引
	//参数：ptable有效
	vector< pair<string,int> > GetIndexFile(list<DICELETYPE>::iterator pTable,string Field);
};

 //功能：输出Field的下标，如果不存在，输入-1
 //参数：pELe存在
 int ToXiaBiao(list<DICELETYPE>::iterator pEle,string Field);
 //功能：输出pleft的属性名（以R.name的形式）并且不输出两个表重复的
 //参数:pleft和pright有效并且Table1X和Table2X不超过属性下标的最大值,[out]NewField
 void CreateField(list<DICELETYPE>::iterator pleft,int Table1X,list<DICELETYPE>::iterator pright,int Table2X,vector<string> &NewField);
 //功能：输出表
 //参数：Tab的每个元素指针有效
 void PrintTable(vector<vector<string> *> Tab);
 //功能:选择符合表达式的元组输出
 //参数:表达式中的左值必须是属性的下标并且下标小于NewTab元素指向的vector的大小
 void ChoosePrintTable(vector< vector<string> *> NewTab,vector<expression> xiabiaoexpstream);
 //功能:验证line元组是否符合单一条件表达式Exp
 //参数:line有效，并且大小不小于exp.left的大小； exp的属性值为下标值
 //返回值：如果符合，返回真；否则，返回假
 bool isOneConditionFit(vector<string> line,expression exp);
 //功能：将pField指向的属性名全部改为对应下标
 //参数:pField有效，NewFiled中包含所有pField指向的属性
 void FieldToXiaBiao(vector<string> *pField,vector<string> NewField);
 //功能:输出部分属性值
 //参数：Field中的值小于NewField的数组大小
 void PrintPartType(vector<string> xiabiaoField,vector<string> NewField);
 //功能：选择部分元组的部分属性输出
 //参数：xiabiaoexpstream和xiabiaoField中的所有属性都是下标值，且下标值小于NewTab元素所指数组的大小
 void ChoosePrintTable(vector< vector<string> *> NewTab,vector<expression> xiabiaoexpstream,vector<string> xiabiaoField);
 //功能：输出元组的部分属性值
 //参数：Ele的数组大小大于xiabiaoField中每一个元素(整型的字符串)
 void PrintPartValue(vector<string> Ele,vector<string> xiabiaoField);



 //利用索引完成查询
 //功能:将对应的下标属性值转化为属性名
 //参数：xiabiaoexpstream中的下标值必须小于FieldType的数组大小
 string ToField(list<FILEDPTYPE_TYPE> FieldType,expression xiabiaoexpstream);
