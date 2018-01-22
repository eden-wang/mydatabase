#include <iostream>
#include <string>
#include <fstream>
#include "DataBase.h"
#include <algorithm>
using namespace std;

bool DataBase::CreatTable(string tablename,list<FILEDPTYPE_TYPE> FieldPlusType )//注意，为了方便，我们对于char类型只传入char的数值，如‘char(15)’只传入‘15’
{
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
		cout<<"Table "<<tablename<< " exists."<<endl;
		return false;
	}
	DICELETYPE DicEle;
	DicEle.TableName=tablename;
	FieldPTypeList *pFieldTypeList=new FieldPTypeList();	
	pFieldTypeList->SetFieldPType(FieldPlusType);
	ElementsCol  *pElements=new ElementsCol();
	pElements->SetColumn(pFieldTypeList->GetSize());
	DicEle.pFieldPlusTypeList=pFieldTypeList;
	DicEle.pElementsCol=pElements;	 
	m_DataDic.push_back(DicEle);
    return true;	
}
void DataBase::PrintDictionary()
{
	for(list<DICELETYPE>::iterator p=m_DataDic.begin();p!=m_DataDic.end();++p)
	{
		cout<<std::left<<p->TableName<<":"<<endl;
		p->pFieldPlusTypeList->PrintOnlyType();
		p->pFieldPlusTypeList->PrintOnlyField();
		p->pElementsCol->PrintElements();
	}
	
}
/*
 判断表是否存在
 如果返回值为true， pEle返回指向该表的指针
 */
bool DataBase::TableExist(string tablename,list<DICELETYPE>::iterator &pEle)
{
	for(list<DICELETYPE>::iterator p=m_DataDic.begin();p!=m_DataDic.end();++p)
	{
		if(p->TableName==tablename)
		{
				pEle=p;
				return true;
		}
	}
	return false;
}
void DataBase::WriteToFile()
{
	ofstream outdic;
	outdic.open("Dictionary.txt");
	for(list<DICELETYPE>::iterator p=m_DataDic.begin();p!=m_DataDic.end();++p)
	{
		outdic<<setiosflags(ios_base::left)<<p->TableName<<endl;
		p->pFieldPlusTypeList->WriteFieldToFile(outdic);
	}
//	outdic<<resetiosflags(ios_base::left);//关闭左对齐标志
	outdic.close();
	//表格建立文件
	string filename;
    for(list<DICELETYPE>::iterator p=m_DataDic.begin();p!=m_DataDic.end();++p)
	{
		filename=p->TableName+".txt";
		outdic.open(filename.c_str());
//		outdic<<setiosflags(ios_base::left);
		p->pFieldPlusTypeList->WriteOnlyFieldToFile(outdic);
		p->pElementsCol->WriteElementsToFile(outdic);
		outdic.close();
	}
}
static void ReadFieldPlusType(ifstream &in,list<FILEDPTYPE_TYPE> &FieldPlusType,int &FieldLen)//从in中提取属性名和其类型,属性个数
{
	if(in.is_open())
	{
		in>>FieldLen;
		string field,type;
		for(int i=0;i<FieldLen;i++)
		{
			in>>field>>type;
			FieldPlusType.push_back(make_pair(field,type));
		}
	}
	
}
static void ReadElements(string tablename,int FieldLen,ElementsCol *pElements)
{
	string filename=tablename+".txt";
	ifstream in(filename.c_str());
	if(in.is_open()&&FieldLen>0) //可能不含有属性
	{
		
		int n=0,i=0,j=0;
		string t;
		for(int k=0;k<FieldLen;k++) in>>t;//去除第一行的属性名
		while(in>>t)
		{
//			if(filename=="R.txt") cout<<"duqu"<<t<<endl;
			n++;
			if(n%FieldLen==0)
			{
				pElements->SetElement(i,j,t);
				i++;
				j=0;
			}
			else pElements->SetElement(i,j++,t);
		}
		pElements->SetLineAndColumn(i,FieldLen);
		in.close();
	}
	else 
		cout<<"打开 "<<filename<<" 失败"<<endl;

}
void DataBase::PowerOnDataBase()//启动数据库:从文件中读取信息
{
	ifstream in;
	in.open("Dictionary.txt");
	if(in.is_open())
	{
		string tablename;		
		int FieldLen;//属性个数
		while(in>>tablename)
		{
			list<FILEDPTYPE_TYPE> FieldPlusType;
			DICELETYPE DicEle;
			DicEle.TableName=tablename;
			ReadFieldPlusType(in,FieldPlusType,FieldLen);
			FieldPTypeList *pFieldTypeList=new FieldPTypeList();	
			pFieldTypeList->SetFieldPType(FieldPlusType);
			ElementsCol  *pElements=new ElementsCol();
			ReadElements(tablename,FieldLen,pElements);
			DicEle.pFieldPlusTypeList=pFieldTypeList;
			DicEle.pElementsCol=pElements;	 
			m_DataDic.push_back(DicEle);
		}
	}
	else 
		cout<<"打开数据字典失败"<<endl;
//	PrintDictionary();
	//2015.6.4添加启动索引文件
	ReadTableIndexFromFile();
}
bool DataBase::DropTable(string tablename)//删除表
{
	list<DICELETYPE>::iterator pDelTable=m_DataDic.end();
	for(list<DICELETYPE>::iterator p=m_DataDic.begin();p!=m_DataDic.end();++p)
	{
		if(p->TableName==tablename)
		{
			pDelTable=p;
			break;
		}
	}
	if(pDelTable!=m_DataDic.end())
	{
		delete pDelTable->pFieldPlusTypeList;
		delete pDelTable->pElementsCol;
		string filename=pDelTable->TableName+".txt";
		remove( filename.c_str());//成功返回0，否则返回EOF
		m_DataDic.erase(pDelTable);
	}
	else
	{
		cout<<"表 "<<tablename<<" 不存在"<<endl;
		return false;
	}
	return true;
}
template <class T>
 void PrintVector(vector<T> vec,char sep=' ')
{
	for(int i=0;i<vec.size();i++)
		cout<<std::left<< setw(STRINGWIDTH)<< vec.at(i)<<sep;
	cout<<endl;
}
 void DataBase::AddAllField(list<DICELETYPE>::iterator pEle,vector<string> &Field)
 {
	 if(Field.size()==0)
	{ 
		list<FILEDPTYPE_TYPE>::iterator p;
		for( p=pEle->pFieldPlusTypeList->m_FieldPlusType.begin();p!=pEle->pFieldPlusTypeList->m_FieldPlusType.end();++p)
		{
		      Field.push_back(p->first);
		}
	 }
	 else
	 {
		 cout<<"属性名不为零，不能添加所有属性"<<endl;
	 }
 }
bool DataBase::insert(string tablename,vector<string> Field,vector<string> value)//插入
{
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
		if(Field.size()==0)
			AddAllField(pEle,Field);
		if(isMatchField(pEle,Field)&&isMatchType(pEle,Field,value))//属性名是否在表的属性列表中
		{
			FieldToflags(pEle,Field);//将属性列名转换为表中的下标
//			PrintVector(Field);
//			PrintVector(value);
  			pEle->pElementsCol->Insert(Field,value);//插入d给定的value值，其他赋值为NULL
		}		
		else
		{
			cout<<"属性不存在或类型、个数不匹配"<<endl;
			return false;
		}
	}
	else 
	{
			cout<<"表 "<<tablename<<" 不存在"<<endl;
			return false;
	}
	return true;
}
/*void DataBase::FieldToflags(list<DICELETYPE>::iterator pEle,vector<string> &Field)//将属性名转换为下标，必须保证表中有Field中的所有属性
{
	int j=0;
	int i=0;//属性值的下标
	for(list<FILEDPTYPE_TYPE>::iterator p=pField->m_FieldPlusType.begin();p!=pField->m_FieldPlusType.end();++p)
	{
		if( j<Field.size()&&p->first==Field[j] )
		{
				flags.push_back(i);
				j++;
		}
		i++;
	}
	if( Field.size()==flags.size() )
		return true;
	
}*/
bool DataBase::AlterTableAdd(string tablename,string Field,string Type)
{
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
		
		if(pEle->pElementsCol->AddNewField())
		{
				pEle->pFieldPlusTypeList->m_FieldPlusType.push_back(make_pair(Field,Type));
				return true;
		}
		else 
		{
			cout<<"添加属性列失败"<<endl;
			return false;
		}
	}
	else
	{
		cout<<"表格不存在"<<endl;
		return false;
	}
	return true;
}

bool DataBase::AlterTableDrop(string tablename,vector<string> Field)
{
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
        if(isMatchField(pEle,Field)) //检查属性名是否存在
		{
			pEle->pFieldPlusTypeList->DropField(Field);
			FieldToflags(pEle,Field); //将Field中的属性值转换为下标
//			PrintVector(Field);
			pEle->pElementsCol->DropField(Field);
		}
		else
		{
			cout<<"属性不存在"<<endl;
			return false;
		}

	}
	else
	{
		cout<<"表格不存在"<<endl;
		return false;
	}
	return true;
}
bool DataBase::MyDelete(string tablename,vector<string> Exp)
{
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
		return pEle->pElementsCol->DeleteEle(Exp,pEle->pFieldPlusTypeList->m_FieldPlusType);
	}
	else
	{
		cout<<"表格不存在"<<endl;
		return false;
	}
	return true;
}
extern void PrintVector_Pair(vector < pair<string,string> > vec);
bool DataBase::update(string tablename,vector< pair<string,string> > FieldValue,vector<expression> expstream)
{
//	PrintVector_Pair(FieldValue);
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
		if(isMatch(FieldValue,pEle)) 
		{
			if(isMatch(expstream,pEle)) 
			{
				return pEle->pElementsCol->update(FieldValue,expstream);
			}
			else
			{
				cout<<"表达式中属性名或类型不匹配"<<endl;
				return false;
			}
		}			
		else 
		{
			cout<<"属性名或类型不匹配"<<endl;
			return false;
		}
	}
	else
	{
		cout<<"表格不存在"<<endl;
		return false;
	}
	return true;
}

static inline void ConvertValue(vector< pair<string,string> > arg1,vector<string> &arg2)//将arg1的first赋值给arg2
{
	for(int i=0;i<arg1.size();i++)
		arg2.push_back((arg1[i]).second);
}
static inline void Convert(vector< pair<string,string> > arg1,vector<string> &arg2)//将arg1的first赋值给arg2
{
	for(int i=0;i<arg1.size();i++)
		arg2.push_back((arg1[i]).first);
}
static inline void Assignment(vector<string> arg1,vector< pair<string,string> > &arg2)//将arg1赋值给arg2的first
{
	for(int i=0;i<arg1.size()&&i<arg2.size();i++) 
			(arg2[i]).first=arg1[i];
}

bool DataBase::isMatch(vector< pair<string,string>>& FieldValue,list<DICELETYPE>::iterator pEle) //检测属性名是否匹配，属性类型是否正确；并将 属性名域修改为下标值
{	vector<string> Field;
    Convert(FieldValue,Field); //提取属性名赋给第二个参数
	if(isMatchField(pEle,Field))
	{
		vector<string> Value;
		ConvertValue(FieldValue,Value);//提取属性值赋给第二个参数
		if(isMatchType(pEle,Field,Value))
		{
			FieldToflags(pEle,Field);
			Assignment(Field,FieldValue);//将下标值赋给第二个参数
			return true;
		}
		else
			return false;
	}
	else 
			return false;
	return false;
}
static inline void Convert(vector<expression> arg1,vector<string> &arg2)//将arg1的属性名赋值给arg2
{
	for(int i=0;i<arg1.size();i++)
		arg2.push_back(arg1[i].left);
}
static inline void Assignment(vector<string> arg1,vector<expression> &arg2)//将arg1赋值给arg2的属性名
{
	for(int i=0;i<arg1.size()&&i<arg2.size();i++) 
		arg2[i].left=arg1[i];
}
static inline void ConvertValue(vector<expression> arg1,vector<string> &Value)//提取属性值赋给第二个参数
{
		for(int i=0;i<arg1.size();i++)
			Value.push_back(arg1[i].right);
}
bool DataBase::isMatch(vector< expression >& expstream ,list<DICELETYPE>::iterator pEle)////检测属性名是否匹配，属性类型是否正确；并将 属性名域修改为下标值
{
	vector<string> Field;
    Convert(expstream,Field); //提取属性名赋给第二个参数
	if(isMatchField(pEle,Field))
	{
		vector<string> Value;
		ConvertValue(expstream,Value);//提取属性值赋给第二个参数
		if(isMatchType(pEle,Field,Value))
		{
			FieldToflags(pEle,Field);
			Assignment(Field,expstream);//将下标值赋给第二个参数
			return true;
		}
		else
			return false;
	}
	else return false;
	return true;
}

//单表操作2015.6.15修改
void DataBase::Select(vector<string>  Field,vector<string> Table,vector<expression>  expstream)
{
	//for(int i=0;i<expstream.size();i++)
	//{
	 //	cout<<expstream[i].left<<" "<<expstream[i].op<<" "<<exp,0stream[i].right<<" "<<expstream[i].logop<<endl;
	//}
	//cout<<endl;
	list<DICELETYPE>::iterator pTable;
   if(TableExist(Table[0],pTable))
   {
	  if( isMatch(expstream,pTable) ) 
	  {
		  if("*"==Field[0] &&Field.size()==1)
		  {
			  if(expstream.size()==0)
				 PrintTab(pTable);
			  else if(expstream.size()==1)
			  {
				  LARGE_INTEGER   litmp;  
				  LONGLONG   QPart1,QPart2;  
				  double   dfMinus,   dfFreq,   dfTim;  
				  QueryPerformanceFrequency(&litmp);  
				  dfFreq   =   (double)litmp.QuadPart;  
				  QueryPerformanceCounter(&litmp);  
				  QPart1   =   litmp.QuadPart;  
				  PrintTab(pTable,expstream);
				  QueryPerformanceCounter(&litmp);  
				  QPart2   =   litmp.QuadPart;
				  dfMinus = (double)(QPart2 - QPart1);
				  dfTim   =   dfMinus   /   dfFreq;
				  cout<<"没有利用索引用时 "<<dfTim<<" s"<<endl;
				  PrintTabOnIndex(pTable,expstream[0]);
				  QueryPerformanceCounter(&litmp);  
				  QPart1  =   litmp.QuadPart;
				  dfMinus = (double)(QPart1 - QPart2);
				  dfTim   =   dfMinus   /   dfFreq;
				  cout<<"利用索引用时 "<<dfTim<<" s"<<endl;
			  }
			  else
				  PrintTab(pTable,expstream);
		  }
		  else if (isMatchField(pTable,Field))
		  {
			  FieldToflags(pTable,Field);
			  if(expstream.size()==0)
					  PrintTab(pTable,Field);
			  else
				  PrintTab(pTable,expstream,Field);
		  }
		  else
		  	 cout<<"投影属性名或类型错误"<<endl;		  
	  }
	  else	  
		  cout<<"where表达式中属性名或类型错误"<<endl;	  
   }
   else   
	   cout<<"表不存在"<<endl;   
}
bool DataBase::isMatchField(list<DICELETYPE>::iterator pEle,vector<string> Field)//属性名是否在表的属性列表中
{
	for(int i=0;i<Field.size();i++)
	{
		list<FILEDPTYPE_TYPE>::iterator p;
		for( p=pEle->pFieldPlusTypeList->m_FieldPlusType.begin();p!=pEle->pFieldPlusTypeList->m_FieldPlusType.end();++p)
		{
			if(Field[i]==p->first)
				break;
		}
		if(pEle->pFieldPlusTypeList->m_FieldPlusType.end()==p )
			return false;
	}
	return true;
}
void DataBase::FieldToflags(list<DICELETYPE>::iterator pEle,vector<string> &Field)//将属性名转换为下标，必须保证Field中的所有属性都在pEle指向的表中
{	
	for(int i=0;i<Field.size();i++)
	{
		list<FILEDPTYPE_TYPE>::iterator p;
		int j=0;
		for( p=pEle->pFieldPlusTypeList->m_FieldPlusType.begin();p!=pEle->pFieldPlusTypeList->m_FieldPlusType.end();++p)
		{
			if(Field[i]==p->first)
			{
				char t[20];
				Field[i]=_itoa(j,t,10);
				break;
			}
			j++;
		}
	}
}
bool DataBase::isMatchType(list<DICELETYPE>::iterator pEle,vector<string> &Field,vector<string> value)//判断类型是否一致
{
//	cout<<"here:";
//	PrintVector(Field);
//	cout<<"here:";
//	PrintVector(value);
	for(int i=0;i<Field.size();i++)
	{
			list<FILEDPTYPE_TYPE>::iterator p;
			for( p=pEle->pFieldPlusTypeList->m_FieldPlusType.begin();p!=pEle->pFieldPlusTypeList->m_FieldPlusType.end();++p)
			{
				if(p->first==Field[i])
				{
					if(p->second=="int")
					{
						if( atoi (value[i].c_str())==0) return false;
					}
					break;
				}
			}
	}
//	PrintVector(Field);
//	PrintVector(value);
	return true;
}
bool DataBase::DropAllEle(string tablename)
{
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
		pEle->pElementsCol->DeleteAllEle();
	}
	else
	{
		cout<<"表格不存在"<<endl;
		return false;
	}
	return true;
}
//创建索引,支持一个属性
void DataBase::CreateIndex(string TableName,string IndexName,vector<string> *Filed)
{
	if(Filed->size()!=1)
	{
		cout<<"请输入一个属性名^_^"<<endl;
		return;
	}
     if(isHaveIndex(TableName,Filed->at(0)))
	 {
		 cout<<"已在该属性上创建索引"<<endl;
		 return;
	 }
	 if(!isIndexOnly(IndexName))
	 {
		 cout<<"索引名已存在"<<endl;
		 return;
	 }
	 list<DICELETYPE>::iterator pTable;
	 if(!TableExist(TableName,pTable))
	 {
		 cout<<"表不存在"<<endl;
		 return;
	 }
	 if(isMainField(pTable,Filed->at(0))) //主属性
	 {
//		 cout<<"zhujian"<<endl;
		 //sort()排序
		 SortOnMainField(pTable);//对主属性排序
		 //建主索引
		 TableIndexArray.push_back(IndexStr(TableName,Filed->at(0),IndexName,CreatMainIndex(pTable))); 
	 }
	 else
	 {
		 //建辅助索引
		 int value=-1;//辅助索引域的下标
		 int i=0;
		 for(list<FILEDPTYPE_TYPE>::iterator  p=pTable->pFieldPlusTypeList->m_FieldPlusType.begin();p!=pTable->pFieldPlusTypeList->m_FieldPlusType.end();++p,++i)
		 {
			 if(p->first==Filed->at(0))
				 value=i;
		 }
		 if(-1==value)
		 {
			 cout<<"不存在该属性"<<endl;
			 return;
		 }
		 TableIndexArray.push_back(IndexStr(TableName,Filed->at(0),IndexName,CreateAssistIndex(pTable,value))); 
	 }
	 WriteTableIndexToFile();
	 cout<<"创建索引成功"<<endl;
//	 PrintIndexArray();

}
bool DataBase::isHaveIndex(string TableName,string FieldName)//表是否在某属性上建立过索引
{
	for(int i=0;i<TableIndexArray.size();i++)
	{
//		cout<<TableIndexArray[i].TableName<<" "<<TableIndexArray[i].FiledName<<" *** ";
//		cout<<TableName<<" "<<FieldName<<endl;
		if((TableName == TableIndexArray[i].TableName) && (FieldName == TableIndexArray[i].FiledName) )
		{
			return true;
		}
	}
	return false;
}
bool DataBase::isMainField(list<DICELETYPE>::iterator pTable,string FieldName)//某属性是否在某表的主键
{
	list<FILEDPTYPE_TYPE>::iterator  p=pTable->pFieldPlusTypeList->m_FieldPlusType.begin();
	if(p->first==FieldName)
		return true;
	return false;
}
 //第一个大于第二个,返回真，对整型和字符型有不同的比较方式
static bool Above(list<DICELETYPE>::iterator pEle,string str1,string str2)
{
	if("int"==(pEle->pFieldPlusTypeList->m_FieldPlusType).begin() ->second)
	{
		int a=atoi(str1.c_str());
		int b=atoi(str2.c_str());
//		cout<<a<<" "<<b<<endl;
		return  a<b?false:true;
	}
	else
	{
		if(strcmp(str1.c_str(),str2.c_str())<0)
		    return false;
	}

}
//对主属性排序
void DataBase::SortOnMainField(list<DICELETYPE>::iterator pEle)
{
//	cout<<"对主属性排序中..."<<endl;
   ElementsCol *pElement=pEle->pElementsCol;
   int line=pEle->pElementsCol->m_line;
   int column=pEle->pElementsCol->m_column;
   for(int k=0;k<line-1;k++)
	   for(int i=0;i<line-1-k;i++)
		   if( Above(pEle,pElement->m_Elements[i][0],pElement->m_Elements[i+1][0])) //第一个大于第二个,返回真，对整型和字符型有不同的比较方式
		   {
	//		   cout<<"here"<<endl;
			   string t;
			   for(int j=0;j<column;j++)
			   {
				   t=pElement->m_Elements[i][j];
				   pElement->m_Elements[i][j]=pElement->m_Elements[i+1][j];
				   pElement->m_Elements[i+1][j]=t;
			   }

		   }
 //   for(int i=0;i<pEle->pElementsCol->m_line;i++)
 //  {
	//   for(int j=0;j<pEle->pElementsCol->m_column;j++)
	//	   cout<<setw(STRINGWIDTH)<<pEle->pElementsCol->m_Elements[i][j];
	//	   cout<<endl;
	//}

}
//建主索引
IndexFile * DataBase::CreatMainIndex(list<DICELETYPE>::iterator pEle)
{
	IndexFile *pIndexFile=new IndexFile();
	int line=pEle->pElementsCol->m_line;
	pIndexFile->IndexFileArray.push_back(make_pair(pEle->pElementsCol->m_Elements[0][0],0));
	int step=ceill( sqrt(line)/2 );
//	int step=1;//2015.6.17 修改为主索引也对应为每一项，为了方便利用索引操作
	int next=step;
	while (next<line)
	{
		pIndexFile->IndexFileArray.push_back(make_pair(pEle->pElementsCol->m_Elements[next][0],next));
		next+=step;
	}
	//for(int i=0;i<pIndexFile->IndexFileArray.size();i++)
	//{
	//	cout<<pIndexFile->IndexFileArray[i].first<<"  "<<pIndexFile->IndexFileArray[i].second <<endl;
	//}
	return pIndexFile;
}
class MyJudge
{
	public:
	list<DICELETYPE>::iterator pEle;
	MyJudge(list<DICELETYPE>::iterator pEle){this->pEle=pEle;}
	bool operator()(pair<string,int> Item1,pair<string,int> Item2)
	{
	
		return !Above(pEle,Item1.first,Item2.first);
	}
};
//建辅助索引
IndexFile *DataBase::CreateAssistIndex(list<DICELETYPE>::iterator pEle,int pos)
{
	IndexFile *pIndexFile=new IndexFile();
	int line=pEle->pElementsCol->m_line;
	int next=0;
	while (next<line)
	{
		pIndexFile->IndexFileArray.push_back(make_pair(pEle->pElementsCol->m_Elements[next][pos],next));
		next++;
	}
	//排序
//	cout<<"before"<<endl;
//	pIndexFile->PrintIndexFile();
    sort(pIndexFile->IndexFileArray.begin(),pIndexFile->IndexFileArray.end(),MyJudge(pEle));
	//for(int i=0;i<pIndexFile->IndexFileArray.size();i++)
	//{
	//	cout<<pIndexFile->IndexFileArray[i].first<<"  "<<pIndexFile->IndexFileArray[i].second <<endl;
	//}
//	cout<<"after"<<endl;
 //   pIndexFile->PrintIndexFile();
	return pIndexFile;
}

//输出索引表
void DataBase::PrintIndexArray()
{
	for(int i=0;i<TableIndexArray.size();i++)
	{
		cout<<TableIndexArray[i].TableName<<" "<<TableIndexArray[i].FiledName<<" "<<TableIndexArray[i].IndexName<<endl;
		TableIndexArray[i].pIndexFile->PrintIndexFile();
	}
}
//索引名是否已存在
bool DataBase::isIndexOnly(string IndexName)
{
	for(int i=0;i<TableIndexArray.size();i++)
	{
         if(IndexName == TableIndexArray[i].IndexName)
			 return false;
	}
	return true;
}
//将所有表的索引写入文件中
void DataBase::WriteTableIndexToFile()
{
	ofstream fout;
	ofstream foutIndex;//输出索引文件，以索引名命名
	fout.open("TableIndex.txt");
	for(int i=0;i<TableIndexArray.size();i++)
	{		
		fout<<setw(STRINGWIDTH)<<TableIndexArray[i].TableName<<setw(STRINGWIDTH)<<TableIndexArray[i].FiledName<<setw(STRINGWIDTH)<<TableIndexArray[i].IndexName<<endl;
		foutIndex.open(TableIndexArray[i].IndexName+".txt");
		for (int j=0;j<TableIndexArray[i].pIndexFile->IndexFileArray.size();j++)
		{
			foutIndex<<setw(STRINGWIDTH)<<TableIndexArray[i].pIndexFile->IndexFileArray[j].first<<setw(STRINGWIDTH)<<TableIndexArray[i].pIndexFile->IndexFileArray[j].second<<endl;
		}
		foutIndex.close();
	}
	fout.close();
}
//读取表索引文件，调入内存
void DataBase::ReadTableIndexFromFile()
{
	ifstream fin;
	ifstream finIndex;
	fin.open("TableIndex.txt");
	IndexStr Node;
	while (fin>>Node.TableName>>Node.FiledName>>Node.IndexName)
	{
		finIndex.open(Node.IndexName+".txt");
		IndexFile *p=new IndexFile;
		pair<string,int> IndexNode;
		while(finIndex>>IndexNode.first>>IndexNode.second)
		{
			p->IndexFileArray.push_back(IndexNode);
		}
		Node.pIndexFile=p;
		TableIndexArray.push_back(Node);
		finIndex.close();
	}
	fin.close();
//	PrintIndexArray();
}
//删除索引
void DataBase::DropIndex(string IndexName)
{
	vector<IndexStr>::iterator p;
	for(p=TableIndexArray.begin();p!=TableIndexArray.end();++p)
   {
	   if(p->IndexName==IndexName)
		   break;
   }
	if(p!=TableIndexArray.end())
	{
		delete p->pIndexFile;
		remove( (p->IndexName +".txt").c_str());//成功返回0，否则返回EOF
		TableIndexArray.erase(p);
		cout<<"删除索引完成"<<endl;
	}
	else
	{
		cout<<"该索引不存在"<<endl;
	}
	WriteTableIndexToFile();
}
//输出表格
void DataBase::PrintTab(list<DICELETYPE>::iterator pTable)
{
	cout<<endl;
	cout<<pTable->TableName+":"<<endl;
	pTable->pFieldPlusTypeList->PrintOnlyType();
	pTable->pFieldPlusTypeList->PrintOnlyField();
    pTable->pElementsCol->PrintElements();
}
//功能：输出满足表达式的元组(所有属性)
//参数:pTable有效，expstream不为空
void DataBase::PrintTab(list<DICELETYPE>::iterator pTable,vector<expression> expstream)
{
	cout<<endl;
	cout<<pTable->TableName+":"<<endl;
	pTable->pFieldPlusTypeList->PrintOnlyType();
	pTable->pFieldPlusTypeList->PrintOnlyField();
	//输出满足条件的元组
	pTable->pElementsCol->PrintPartEle(expstream);
}
//功能:输出所有元组的某些属性值
//参数:pTable有效，Field必须是下标
void DataBase::PrintTab(list<DICELETYPE>::iterator pTable,vector<string>  xiabiaoField)
{
	cout<<endl;
	cout<<pTable->TableName+":"<<endl;
	pTable->pFieldPlusTypeList->PrintOnlyType(xiabiaoField);
	pTable->pFieldPlusTypeList->PrintOnlyField(xiabiaoField);
//输出所有元组的某些属性值
	pTable->pElementsCol->PrintPartField(xiabiaoField);
}
//功能:输出满足条件元组的某些属性值
//参数:pTable有效，Field必须是下标
void DataBase::PrintTab(list<DICELETYPE>::iterator pTable,vector<expression> expstream,vector<string>  xiabiaoField)
{
	cout<<endl;
	cout<<pTable->TableName+":"<<endl;
	pTable->pFieldPlusTypeList->PrintOnlyType(xiabiaoField);
	pTable->pFieldPlusTypeList->PrintOnlyField(xiabiaoField);
	pTable->pElementsCol->PrintPartEleField(expstream,xiabiaoField);
}
//功能：输出表
//参数：Tab的每个元素指针有效
void PrintTable(vector< vector<string> *> Tab)
{
	for(int i=0;i<Tab.size();i++)
	{
		vector<string> *p=Tab[i];
		for(int j=0;j<p->size();j++)
			cout<<std::left<<setw(STRINGWIDTH)<<p->at(j);
		cout<<endl;
	}
}
//功能：将表达式中的属性值转换为下标
//参数要求:表达式中的每个属性都在NewField中
void FieldToXiaBiao(vector<expression> *pexpstream,vector<string> NewField)
{
	char t[20];
	for(int i=0;i<pexpstream->size();i++)
	{
		for (int j=0;j<NewField.size();j++)
		{
			if(NewField[j]==pexpstream->at(i).left|| NewField[j].find(pexpstream->at(i).left)!=string::npos)
			{
				pexpstream->at(i).left= _itoa(j,t,10);
				break;
			}
		}
	}
	//for(int i=0;i<pexpstream->size();i++)
	//{
	//	cout<<pexpstream->at(i).left<<" "<<pexpstream->at(i).op<<" "<<pexpstream->at(i).right<<" "<<pexpstream->at(i).logop<<endl;
	//}
}
//功能:验证line元组是否符合单一条件表达式Exp
//参数:line有效，并且大小不小于exp.left的大小； exp的属性值为下标值
//返回值：如果符合，返回真；否则，返回假
bool isOneConditionFit(vector<string> line,expression exp)
{
	int xiabiao=atoi( exp.left.c_str());
	if("="==exp.op) //由于在存储都是字符串，所以右操作数是整形的也不需要转换
	{
		return line[xiabiao]==exp.right;
	}
	else if("!="==exp.op)
	{
		return line[xiabiao]!=exp.right;
	}
	else if(">="==exp.op)
	{
		return atoi(line[xiabiao].c_str())>=atoi(exp.right.c_str());
	}
	else if("<="==exp.op)
	{
		return atoi(line[xiabiao].c_str())<=atoi(exp.right.c_str());
	}
	else if(">"==exp.op)
	{
		return atoi(line[xiabiao].c_str())>atoi(exp.right.c_str());
	}
	else if("<"==exp.op)
	{
		return atoi(line[xiabiao].c_str())<atoi(exp.right.c_str());
	}
	return false;
}
//功能:选择符合表达式的元组输出
//参数:表达式中的左值必须是属性的下标并且下标小于NewTab元素指向的vector的大小
void ChoosePrintTable(vector< vector<string> *> NewTab,vector<expression> xiabiaoexpstream)
{
	for (int j=0;j<NewTab.size();j++)
	{
		int i=0;
		while(i<xiabiaoexpstream.size())
		{
			if( isOneConditionFit(*NewTab[j],xiabiaoexpstream[i]) )
			{
				if("or"==xiabiaoexpstream[i].logop||""==xiabiaoexpstream[i].logop)
				{
					PrintVector(*NewTab[j]);//满足条件
					break;
				}
				else i++;
			}				
			else
			{
				if(""==xiabiaoexpstream[i].logop)//不满足条件
					break;
				else
				{
					while( i<xiabiaoexpstream.size() && xiabiaoexpstream[i].logop!="or")
						i++;
					if(i>=xiabiaoexpstream.size()) //不满足条件
						break;
					else i++;
				}
			}
		}
	}
}
//功能：将pField指向的属性名全部改为对应下标
//参数:pField有效，NewFiled中包含所有pField指向的属性
void FieldToXiaBiao(vector<string> *pField,vector<string> NewField)
{
	char t[20];
	for (int i=0;i<pField->size();i++)
	{
		for (int j=0;j<NewField.size();j++)
		{
			if (pField->at(i)==NewField[j] || NewField[j].find(pField->at(i))!=string::npos)
			{
				(*pField)[i]=_itoa(j,t,10);
				break;
			}
		}
	}
	//cout<<"属性"<<endl;
	//for (int i=0;i<pField->size();i++)
	//{
	//	cout<<(*pField)[i]<<" ";
	//}
}
//功能:输出部分属性值
//参数：Field中的值小于NewField的数组大小
void PrintPartType(vector<string> xiabiaoField,vector<string> NewField)
{
	for(int i=0;i<xiabiaoField.size();i++)
	{
		cout<<std::left<<setw(STRINGWIDTH)<<NewField[atoi(xiabiaoField[i].c_str())];//返回0时表示异常，但事实上下标值可以为零
	}
	cout<<endl;
}
//功能：输出元组的部分属性值
//参数：Ele的数组大小大于xiabiaoField中每一个元素(整型的字符串)
void PrintPartValue(vector<string> Ele,vector<string> xiabiaoField)
{
	for(int i=0;i<xiabiaoField.size();i++)
	{
		cout<<std::left<<setw(STRINGWIDTH)<<Ele[atoi(xiabiaoField[i].c_str())];//返回0时表示异常，但事实上下标值可以为零
	}
	cout<<endl;
}
//功能：选择部分元组的部分属性输出
//参数：xiabiaoexpstream和xiabiaoField中的所有属性都是下标值，且下标值小于NewTab元素所指数组的大小
void ChoosePrintTable(vector< vector<string> *> NewTab,vector<expression> xiabiaoexpstream,vector<string> xiabiaoField)
{
	//cout<<"选择条件"<<endl;
	//for(int i=0;i<xiabiaoexpstream.size();i++)
	//{
	//	cout<<xiabiaoexpstream[i].left<<" "<<xiabiaoexpstream[i].op<<" "<<xiabiaoexpstream[i].right<<" "<<xiabiaoexpstream[i].logop<<endl;
	//}
	for (int j=0;j<NewTab.size();j++)
	{
		int i=0;
		while(i<xiabiaoexpstream.size())
		{
			if( isOneConditionFit(*NewTab[j],xiabiaoexpstream[i]) )
			{
				if("or"==xiabiaoexpstream[i].logop||""==xiabiaoexpstream[i].logop)
				{
					PrintPartValue(*NewTab[j],xiabiaoField);//满足条件
					break;
				}
				else i++;
			}				
			else
			{
				if(""==xiabiaoexpstream[i].logop)//不满足条件
					break;
				else
				{
					while( i<xiabiaoexpstream.size() && xiabiaoexpstream[i].logop!="or")
						i++;
					if(i>=xiabiaoexpstream.size()) //不满足条件
						break;
					else i++;
				}
			}
		}
	}
}
//
void PrintPartValue(vector< vector<string> *> NewTab,vector<string> xiabiaoField)
{
	for (int i=0;i<NewTab.size();i++)
	{
		for(int j=0;j<xiabiaoField.size();j++)
		{
			cout<<std::left<<setw(STRINGWIDTH)<<NewTab[i]->at( atoi(xiabiaoField[j].c_str()) );//返回0时表示异常，但事实上下标值可以为零
		}
		cout<<endl;
	}
}
//功能：多表操作（连接和选择）
//参数：Link不能为空，expstream中的每个元素的right必须是常量
//为了简化程序，假设输入的属性都是正确的
void DataBase::SelectMultiTab(vector<string> Table,vector<string> Field,vector<LinkCondi> Link,vector<expression> expstream) 
{
	vector<string> NewField;
	vector< vector<string> *> NewTab;
	if ("*"==Field[0]&&Field.size()==1) 
	{
		if(expstream.size()==0&&Link.size()>0)//多个表的连接操作
		{
			LARGE_INTEGER   litmp;  
			LONGLONG   QPart1,QPart2;  
			double   dfMinus,   dfFreq,   dfTim;  
			QueryPerformanceFrequency(&litmp);  
			dfFreq   =   (double)litmp.QuadPart;  
			QueryPerformanceCounter(&litmp);  
			QPart1   =   litmp.QuadPart;  
			NewTab=LinkTwoTab(Link[0],NewField);
			PrintVector(NewField);
			PrintTable(NewTab);
			QueryPerformanceCounter(&litmp);  
			QPart2   =   litmp.QuadPart;
			dfMinus = (double)(QPart2 - QPart1);
			dfTim   =   dfMinus   /   dfFreq;
			cout<<"没有利用索引用时 "<<dfTim<<" s"<<endl;
			NewTab=LinkTwoTabOnIndex(Link[0],NewField);
			PrintVector(NewField);
			PrintTable(NewTab);
			QueryPerformanceCounter(&litmp);  
			QPart1  =   litmp.QuadPart;
			dfMinus = (double)(QPart1 - QPart2);
			dfTim   =   dfMinus   /   dfFreq;
			cout<<"利用索引用时 "<<dfTim<<" s"<<endl;
//			cout<<"多个表的连接操作"<<endl;
		}
		else if(expstream.size()>0 &&Link.size()>0)//多个表的选择连接操作
		{
			NewTab=LinkTwoTab(Link[0],NewField);			
			FieldToXiaBiao(&expstream,NewField);
			PrintVector(NewField);
		    ChoosePrintTable(NewTab,expstream);
		}
		else
		{
				cout<<"未定义的查询"<<endl;
				return;
		}
	}
	else
	{
		if(expstream.size()==0&&Link.size()>0)//多个表的连接投影操作
		{
			NewTab=LinkTwoTab(Link[0],NewField);
			FieldToXiaBiao(&Field,NewField);
			PrintPartType(Field,NewField);
			PrintPartValue(NewTab,Field);
//			cout<<"多个表的连接投影操作"<<endl;
		}
		else if(expstream.size()>0 &&Link.size()>0)
		{
			NewTab=LinkTwoTab(Link[0],NewField);
			FieldToXiaBiao(&Field,NewField);
			FieldToXiaBiao(&expstream,NewField);
			PrintPartType(Field,NewField);
			ChoosePrintTable(NewTab,expstream,Field);
//			cout<<"多个表的选择连接投影操作"<<endl;
		}
		else
		{
			cout<<"未定义的查询"<<endl;
			return;
		}
	}

}
//功能：输出Field的下标，如果不存在，输入-1
//参数：pELe存在
int ToXiaBiao(list<DICELETYPE>::iterator pEle,string Field)
{
	list<FILEDPTYPE_TYPE>::iterator p;
	int j=0;
	for( p=pEle->pFieldPlusTypeList->m_FieldPlusType.begin();p!=pEle->pFieldPlusTypeList->m_FieldPlusType.end();++p)
	{
		if(Field==p->first)
		{
           return j;
			break;
		}
		j++;
	}
	return -1;
}
//功能:将pR1和pR2所指向的元组合并（   去掉一个连接属性,连接后的相同属性名=R1.属性(R2.属性)   )
//参数:p有效，pR1和pR2有效，并且linkField1<len1,LinkField2<len2
void Combine(vector<string> *p,
			 string *pR1,int LinkField1,int len1,
			 string * pR2,int LinkField2,int len2)
{
	for(int i=0;i<len1;i++)
	{
		p->push_back( pR1[i] );
	}
	for (int i=0;i<len2;i++)
	{
		if(i!=LinkField2)
			p->push_back(  pR2[i] );
	}
}

//功能：输出pleft的属性名（以R.name的形式）并且不输出两个表重复的
//参数:pleft和pright有效并且Table1X和Table2X不超过属性下标的最大值,[out]NewField
void CreateField(list<DICELETYPE>::iterator pleft,int Table1X,list<DICELETYPE>::iterator pright,int Table2X,vector<string> &NewField)
{
	list<FILEDPTYPE_TYPE> *p=& pleft->pFieldPlusTypeList->m_FieldPlusType;
	int n=0;
	for (list<FILEDPTYPE_TYPE>::iterator q=p->begin();q!=p->end();++q,++n)
	{
		if(n==Table1X)
			NewField.push_back(  pleft->TableName+"."+q->first+"("+pright->TableName+"."+q->first+")" );
		else
			NewField.push_back( pleft->TableName+"."+q->first );
	}
	p= & pright->pFieldPlusTypeList->m_FieldPlusType;
	n=0;
	for (list<FILEDPTYPE_TYPE>::iterator q=p->begin();q!=p->end();++q,++n)
	{
		if(n!=Table2X)
			NewField.push_back( pright->TableName+"."+q->first );
	}
}
//功能：多个表的连接操作,投影所有属性
//参数：无要求
vector< vector<string> * > DataBase::LinkTwoTab(LinkCondi Link,vector<string> &NewField)
{
	list<DICELETYPE>::iterator pleft,pright;
	int Table1X,Table2X;
	vector< vector<string> * > TempTab;//临时表
	if(TableExist(Link.Table1,pleft)  &&TableExist(Link.Table2,pright) )
	{
		Table1X=ToXiaBiao(pleft,Link.Field);
		Table2X=ToXiaBiao(pright,Link.Field);
		if(-1==Table1X||-1==Table2X)
		{
			cout<<"表属性不存在"<<endl;
			return TempTab;
		}
		for(int i=0;i<pleft->pElementsCol->m_line;i++)
			for(int j=0;j<pright->pElementsCol->m_line;j++)
			{
				if(pleft->pElementsCol->m_Elements[i][Table1X] == pright->pElementsCol->m_Elements[j][Table2X])
				{
					vector<string> *p=new vector<string>;
					Combine(p,
						pleft->pElementsCol->m_Elements[i],Table1X,pleft->pElementsCol->m_column,
						pright->pElementsCol->m_Elements[j], Table2X,pright->pElementsCol->m_column);//合并
					TempTab.push_back(p);
				}
			}
	//	cout<<"表存在"<<endl;
	}	
	else
	{
		cout<<"表不存在"<<endl;
		return TempTab;
	}
	CreateField(pleft,Table1X,pright,Table2X,NewField);
	return TempTab;
}
//功能:将对应的下标属性值转化为属性名
//参数：xiabiaoexpstream中的下标值必须小于FieldType的数组大小
string ToField(list<FILEDPTYPE_TYPE> FieldType,expression xiabiaoexpstream)
{
	int i=0;
	for (list<FILEDPTYPE_TYPE>::iterator p=FieldType.begin();p!=FieldType.end();++p,++i)
	{
		if (atoi (xiabiaoexpstream.left.c_str() )==i )
		{
			return p->first;
		}
	}
	return string();
}
//功能：判断Ele是否满足条件表达式
//参数：Ele中的类型和exp.right必须一致
bool isOneConditionFit(string Ele ,expression exp)
{
	if("="==exp.op) //由于在存储都是字符串，所以右操作数是整形的也不需要转换
	{
		return Ele==exp.right;
	}
	else if("!="==exp.op)
	{
		return Ele!=exp.right;
	}
	else if(">="==exp.op)
	{
		return atoi(Ele.c_str())>=atoi(exp.right.c_str());
	}
	else if("<="==exp.op)
	{
		return atoi(Ele.c_str())<=atoi(exp.right.c_str());
	}
	else if(">"==exp.op)
	{
		return atoi(Ele.c_str())>atoi(exp.right.c_str());
	}
	else if("<"==exp.op)
	{
		return atoi(Ele.c_str())<atoi(exp.right.c_str());
	}
	return false;

}
//功能：利用索引选择输出元组，属性值全部输出
//参数：pTable有效，expstream中的属性值为下标值
void DataBase::PrintTabOnIndex(list<DICELETYPE>::iterator pTable,expression  xiabiaoexpstream)
{
	string Field=ToField(pTable->pFieldPlusTypeList->m_FieldPlusType,xiabiaoexpstream);
//	cout<<Field<<endl;
	int i;
	for (i=0;i<TableIndexArray.size();i++)
	{
		if(TableIndexArray[i].TableName==pTable->TableName && TableIndexArray[i].FiledName==Field)
            break;
	}
	if(i==TableIndexArray.size())
	{
		cout<<"未建立索引"<<endl;
		return;
	}
	vector<pair<string,int>> IndexArray=TableIndexArray[i].pIndexFile->IndexFileArray;
	for (int j=0;j<IndexArray.size();j++)
	{
		if( isOneConditionFit(IndexArray[j].first,xiabiaoexpstream) ) //是否满足条件
		{
			for(int k=0;k<pTable->pElementsCol->m_column;k++)
				cout<<std::left<<setw(STRINGWIDTH)<< pTable->pElementsCol->m_Elements[IndexArray[j].second][k];
			cout<<endl;
		}
	}

}
//功能：判断表是否在Field域上建立了索引
//参数：ptable有效
vector<pair<string,int>> DataBase::GetIndexFile(list<DICELETYPE>::iterator pTable,string Field)
{
	int i;
	for (i=0;i<TableIndexArray.size();i++)
	{
		if(TableIndexArray[i].TableName==pTable->TableName && TableIndexArray[i].FiledName==Field)
			break;
	}
	if(i==TableIndexArray.size())
	{
		return vector<pair<string,int>>();
	}
	return TableIndexArray[i].pIndexFile->IndexFileArray;
}
//功能:利用索引连接两个表
//参数:无要求
vector< vector<string> *> DataBase::LinkTwoTabOnIndex(LinkCondi Link,vector<string> NewField)
{
	list<DICELETYPE>::iterator pleft,pright;
	int Table1X,Table2X;
	vector< vector<string> * > TempTab;//临时表
	if(TableExist(Link.Table1,pleft)  &&TableExist(Link.Table2,pright) )
	{
		Table1X=ToXiaBiao(pleft,Link.Field);
		Table2X=ToXiaBiao(pright,Link.Field);
		if(-1==Table1X||-1==Table2X)
		{
			cout<<"表属性不存在"<<endl;
			return TempTab;
		}
		vector<pair<string,int>> Index1=GetIndexFile(pleft,Link.Field), Index2=GetIndexFile(pright,Link.Field);
		if(Index1.size()>0 &&Index2.size()>0)
		{
			for ( int i=0;i<Index1.size();i++)
			{
				for (int j=0;j<Index2.size();j++)
				{
					if(Index1[i].first==Index2[j].first)
					{
						vector<string> *p=new vector<string>;
						Combine(p,
							pleft->pElementsCol->m_Elements[Index1[i].second ],Table1X,pleft->pElementsCol->m_column,
							pright->pElementsCol->m_Elements[Index2[j].second], Table2X,pright->pElementsCol->m_column);//合并
						TempTab.push_back(p);
					}
				}
			}
		}
		else
		{
			cout<<"需要两个表的属性都建立索引"<<endl;
			return TempTab;
		}
	}	
	else
	{
		cout<<"表不存在"<<endl;
		return TempTab;
	}
	CreateField(pleft,Table1X,pright,Table2X,NewField);
	return TempTab;
}
