#pragma once
#include "MICRO.h"
#include <string>
#include <vector>
#include <list>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
typedef pair<string,string> FILEDPTYPE_TYPE; //《属性，属性类型》
struct expression
{
	string left;//左操作数
	string op;//操作类型
	string right;//右操作数
	string logop;//是否与下一个表达式有操作关系 and or 
};
class ElementsCol //元组集合
{
public://在做索引的时候发现数据结构定义有问题，应该设置为public 修改于2015.5.25
	string m_Elements[ELEMENTMAX][FIELDMAX];//元组的最大值为ELEMENTMAX，属性的最大值为FIELDMAX
	int m_line;
	int m_column;
public://静态成员变量
   static const int MaxLine=ELEMENTMAX;
	static const int MaxColumn=FIELDMAX;
public:	
	ElementsCol(){	m_line=0;m_column=0;};
	void SetColumn(int column){m_column=column;}
	void PrintElements();
	void WriteElementsToFile(ofstream &out);//将元组写入文件中
	inline void SetElement(int i,int j,string t){m_Elements[i][j]=t;}
	inline void SetLineAndColumn(int x,int y){m_line=x;m_column=y;}
	bool Insert(vector<string> XiaBiao,vector<string> value);//插入一个元组，包含所有属性的值
//	bool InsertAccordingField(vector<unsigned int> flags,vector<string> value);//Field和value的对应个数应该相等，没有指明的属性用NULL赋值
 	bool AddNewField();
	void DropField( vector<string> Xiabiao);
	bool DeleteEle(vector<string> Exp,list<FILEDPTYPE_TYPE> FieldType);
	void DeleteAllEle(){m_line=0;}
	bool update(vector< pair<string,string> > XiaBiaoValue,vector<expression> xiabiaoexpstream);


private:
	bool isEleExist(string EleName,int &pos);//判断元组是否存在
	bool isFit(string *a,int n,vector<expression> expstream,list<FILEDPTYPE_TYPE> FieldType);//判断是否符合表达式
	bool isFit(string *line,vector<expression> xiabiaostream);
	//删除第n行，从第0行开始算
	void DeleteLine(int n);
	bool isOneConditionFit(string *line,int n,expression exp,list<FILEDPTYPE_TYPE> FieldType);//判断是否符合单一的条件表达式
	bool isOneConditionFit(string *line,expression exp);
	//20120515函数体为空
	bool  DetectFieldType(vector<string> Exp,list<FILEDPTYPE_TYPE> FieldType);//检测属性名的正确性，右操作数的类型是否匹配
	bool isKeyChong(vector<string> XiaBiao,vector<string> value);
public:
	//功能:输出满足条件的元组(所有属性)
	//参数:属性值已为下标值
	void PrintPartEle(vector<expression> expstream);
	//功能：输出所有元组的部分属性
	//参数:属性值必须已是下标值
	void PrintPartField(vector<string>  xiabiaoField);
	//功能：输出满足条件元组的部分属性
	//参数:属性值必须已转换为下标值
	void PrintPartEleField(vector<expression> expstream,vector<string>  xiabiaoField);
};
class DataBase;
class  FieldPTypeList
{
public:
	//2015.5.27改为公有成员
	list<FILEDPTYPE_TYPE> m_FieldPlusType;
	inline int GetSize(){return m_FieldPlusType.size();}
	inline	void SetFieldPType(list<FILEDPTYPE_TYPE> FieldPlusType)//对m_FieldPlusType赋值
	{
		m_FieldPlusType=FieldPlusType;
	};
    inline	void PrintOnlyField()
	{
		for(list<FILEDPTYPE_TYPE>::iterator p=m_FieldPlusType.begin();p!=m_FieldPlusType.end();++p)
		{
			cout<<std::left<<setw(STRINGWIDTH)<<p->first;
		}
		cout<<endl;
	}
	inline void PrintOnlyField(vector<string> xiabiaoField)
	{
		int flag;
		int n;
		for(int i=0;i<xiabiaoField.size();i++)
		{
			flag=atoi(xiabiaoField[i].c_str()); 
			n=0;
			for(list<FILEDPTYPE_TYPE>::iterator p=m_FieldPlusType.begin();p!=m_FieldPlusType.end();++p)
			{
				if(n==flag)
				{
						cout<<std::left<<setw(STRINGWIDTH)<<p->first;
						break;
				}
				else n++;
			}
			
		}
		cout<<endl;
	}
	  inline	void  PrintOnlyType()
	  {
		 for(list<FILEDPTYPE_TYPE>::iterator p=m_FieldPlusType.begin();p!=m_FieldPlusType.end();++p)
		{
			if(p->second!="int")
				cout<<std::left<<setw(STRINGWIDTH)<<"char";
			else
				cout<<std::left<<setw(STRINGWIDTH)<<p->second;
		}
		cout<<endl;
	  }
	  inline void PrintOnlyType(vector<string> xiabiaoField)
	  {
		  
		  int flag;
		  int n;
		  for(int i=0;i<xiabiaoField.size();i++)
		  {
//			  cout<<xiabiaoField[i]<<endl;
			  flag=atoi(xiabiaoField[i].c_str()); 
			  n=0;
			  for(list<FILEDPTYPE_TYPE>::iterator p=m_FieldPlusType.begin();p!=m_FieldPlusType.end();++p)
			  {
				  if(n==flag)
				 {
//					 cout<<n<<flag<<endl;
					 if(p->second!="int")
						cout<<std::left<<setw(STRINGWIDTH)<<"char";
					  else
						  cout<<std::left<<setw(STRINGWIDTH)<<p->second;
					 break;
				  }
				  else n++;
			  }
			  
		  }
		  cout<<endl;
	  }
	inline void WriteFieldToFile(ofstream &out)
	{
		if(out.is_open())
		{
			out<<m_FieldPlusType.size()<<endl;
			for(list<FILEDPTYPE_TYPE>::iterator p=m_FieldPlusType.begin();p!=m_FieldPlusType.end();++p)
			{
				out<<setw(STRINGWIDTH)<<p->first<<" "<<setw(STRINGWIDTH)<<p->second<<endl;
			}
		}
	}
	inline void WriteOnlyFieldToFile(ofstream &out)
	{
		if(out.is_open())
		{
			for(list<FILEDPTYPE_TYPE>::iterator p=m_FieldPlusType.begin();p!=m_FieldPlusType.end();++p)
			{
				out<<setw(STRINGWIDTH)<<p->first<<" ";
			}
			out<<endl;
		}
	}
	void DropField(vector<string> Field) //保证field中的所有属性都是在该数组中
	{
		for(int i=0;i<Field.size();i++)
		{
			list<FILEDPTYPE_TYPE>::iterator p;
			for( p=m_FieldPlusType.begin();p!=m_FieldPlusType.end();++p)
			{
				if(Field[i]==p->first)
				{
						m_FieldPlusType.erase(p);
						break;
				}
			}
		}
	}
};
