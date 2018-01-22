#include <iostream>
#include <string>
#include <fstream>
#include "DataBase.h"
#include <algorithm>
using namespace std;

bool DataBase::CreatTable(string tablename,list<FILEDPTYPE_TYPE> FieldPlusType )//ע�⣬Ϊ�˷��㣬���Ƕ���char����ֻ����char����ֵ���确char(15)��ֻ���롮15��
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
 �жϱ��Ƿ����
 �������ֵΪtrue�� pEle����ָ��ñ��ָ��
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
//	outdic<<resetiosflags(ios_base::left);//�ر�������־
	outdic.close();
	//������ļ�
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
static void ReadFieldPlusType(ifstream &in,list<FILEDPTYPE_TYPE> &FieldPlusType,int &FieldLen)//��in����ȡ��������������,���Ը���
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
	if(in.is_open()&&FieldLen>0) //���ܲ���������
	{
		
		int n=0,i=0,j=0;
		string t;
		for(int k=0;k<FieldLen;k++) in>>t;//ȥ����һ�е�������
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
		cout<<"�� "<<filename<<" ʧ��"<<endl;

}
void DataBase::PowerOnDataBase()//�������ݿ�:���ļ��ж�ȡ��Ϣ
{
	ifstream in;
	in.open("Dictionary.txt");
	if(in.is_open())
	{
		string tablename;		
		int FieldLen;//���Ը���
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
		cout<<"�������ֵ�ʧ��"<<endl;
//	PrintDictionary();
	//2015.6.4������������ļ�
	ReadTableIndexFromFile();
}
bool DataBase::DropTable(string tablename)//ɾ����
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
		remove( filename.c_str());//�ɹ�����0�����򷵻�EOF
		m_DataDic.erase(pDelTable);
	}
	else
	{
		cout<<"�� "<<tablename<<" ������"<<endl;
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
		 cout<<"��������Ϊ�㣬���������������"<<endl;
	 }
 }
bool DataBase::insert(string tablename,vector<string> Field,vector<string> value)//����
{
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
		if(Field.size()==0)
			AddAllField(pEle,Field);
		if(isMatchField(pEle,Field)&&isMatchType(pEle,Field,value))//�������Ƿ��ڱ�������б���
		{
			FieldToflags(pEle,Field);//����������ת��Ϊ���е��±�
//			PrintVector(Field);
//			PrintVector(value);
  			pEle->pElementsCol->Insert(Field,value);//����d������valueֵ��������ֵΪNULL
		}		
		else
		{
			cout<<"���Բ����ڻ����͡�������ƥ��"<<endl;
			return false;
		}
	}
	else 
	{
			cout<<"�� "<<tablename<<" ������"<<endl;
			return false;
	}
	return true;
}
/*void DataBase::FieldToflags(list<DICELETYPE>::iterator pEle,vector<string> &Field)//��������ת��Ϊ�±꣬���뱣֤������Field�е���������
{
	int j=0;
	int i=0;//����ֵ���±�
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
			cout<<"���������ʧ��"<<endl;
			return false;
		}
	}
	else
	{
		cout<<"��񲻴���"<<endl;
		return false;
	}
	return true;
}

bool DataBase::AlterTableDrop(string tablename,vector<string> Field)
{
	list<DICELETYPE>::iterator pEle;
	if(TableExist(tablename,pEle))
	{
        if(isMatchField(pEle,Field)) //����������Ƿ����
		{
			pEle->pFieldPlusTypeList->DropField(Field);
			FieldToflags(pEle,Field); //��Field�е�����ֵת��Ϊ�±�
//			PrintVector(Field);
			pEle->pElementsCol->DropField(Field);
		}
		else
		{
			cout<<"���Բ�����"<<endl;
			return false;
		}

	}
	else
	{
		cout<<"��񲻴���"<<endl;
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
		cout<<"��񲻴���"<<endl;
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
				cout<<"���ʽ�������������Ͳ�ƥ��"<<endl;
				return false;
			}
		}			
		else 
		{
			cout<<"�����������Ͳ�ƥ��"<<endl;
			return false;
		}
	}
	else
	{
		cout<<"��񲻴���"<<endl;
		return false;
	}
	return true;
}

static inline void ConvertValue(vector< pair<string,string> > arg1,vector<string> &arg2)//��arg1��first��ֵ��arg2
{
	for(int i=0;i<arg1.size();i++)
		arg2.push_back((arg1[i]).second);
}
static inline void Convert(vector< pair<string,string> > arg1,vector<string> &arg2)//��arg1��first��ֵ��arg2
{
	for(int i=0;i<arg1.size();i++)
		arg2.push_back((arg1[i]).first);
}
static inline void Assignment(vector<string> arg1,vector< pair<string,string> > &arg2)//��arg1��ֵ��arg2��first
{
	for(int i=0;i<arg1.size()&&i<arg2.size();i++) 
			(arg2[i]).first=arg1[i];
}

bool DataBase::isMatch(vector< pair<string,string>>& FieldValue,list<DICELETYPE>::iterator pEle) //����������Ƿ�ƥ�䣬���������Ƿ���ȷ������ ���������޸�Ϊ�±�ֵ
{	vector<string> Field;
    Convert(FieldValue,Field); //��ȡ�����������ڶ�������
	if(isMatchField(pEle,Field))
	{
		vector<string> Value;
		ConvertValue(FieldValue,Value);//��ȡ����ֵ�����ڶ�������
		if(isMatchType(pEle,Field,Value))
		{
			FieldToflags(pEle,Field);
			Assignment(Field,FieldValue);//���±�ֵ�����ڶ�������
			return true;
		}
		else
			return false;
	}
	else 
			return false;
	return false;
}
static inline void Convert(vector<expression> arg1,vector<string> &arg2)//��arg1����������ֵ��arg2
{
	for(int i=0;i<arg1.size();i++)
		arg2.push_back(arg1[i].left);
}
static inline void Assignment(vector<string> arg1,vector<expression> &arg2)//��arg1��ֵ��arg2��������
{
	for(int i=0;i<arg1.size()&&i<arg2.size();i++) 
		arg2[i].left=arg1[i];
}
static inline void ConvertValue(vector<expression> arg1,vector<string> &Value)//��ȡ����ֵ�����ڶ�������
{
		for(int i=0;i<arg1.size();i++)
			Value.push_back(arg1[i].right);
}
bool DataBase::isMatch(vector< expression >& expstream ,list<DICELETYPE>::iterator pEle)////����������Ƿ�ƥ�䣬���������Ƿ���ȷ������ ���������޸�Ϊ�±�ֵ
{
	vector<string> Field;
    Convert(expstream,Field); //��ȡ�����������ڶ�������
	if(isMatchField(pEle,Field))
	{
		vector<string> Value;
		ConvertValue(expstream,Value);//��ȡ����ֵ�����ڶ�������
		if(isMatchType(pEle,Field,Value))
		{
			FieldToflags(pEle,Field);
			Assignment(Field,expstream);//���±�ֵ�����ڶ�������
			return true;
		}
		else
			return false;
	}
	else return false;
	return true;
}

//�������2015.6.15�޸�
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
				  cout<<"û������������ʱ "<<dfTim<<" s"<<endl;
				  PrintTabOnIndex(pTable,expstream[0]);
				  QueryPerformanceCounter(&litmp);  
				  QPart1  =   litmp.QuadPart;
				  dfMinus = (double)(QPart1 - QPart2);
				  dfTim   =   dfMinus   /   dfFreq;
				  cout<<"����������ʱ "<<dfTim<<" s"<<endl;
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
		  	 cout<<"ͶӰ�����������ʹ���"<<endl;		  
	  }
	  else	  
		  cout<<"where���ʽ�������������ʹ���"<<endl;	  
   }
   else   
	   cout<<"������"<<endl;   
}
bool DataBase::isMatchField(list<DICELETYPE>::iterator pEle,vector<string> Field)//�������Ƿ��ڱ�������б���
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
void DataBase::FieldToflags(list<DICELETYPE>::iterator pEle,vector<string> &Field)//��������ת��Ϊ�±꣬���뱣֤Field�е��������Զ���pEleָ��ı���
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
bool DataBase::isMatchType(list<DICELETYPE>::iterator pEle,vector<string> &Field,vector<string> value)//�ж������Ƿ�һ��
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
		cout<<"��񲻴���"<<endl;
		return false;
	}
	return true;
}
//��������,֧��һ������
void DataBase::CreateIndex(string TableName,string IndexName,vector<string> *Filed)
{
	if(Filed->size()!=1)
	{
		cout<<"������һ��������^_^"<<endl;
		return;
	}
     if(isHaveIndex(TableName,Filed->at(0)))
	 {
		 cout<<"���ڸ������ϴ�������"<<endl;
		 return;
	 }
	 if(!isIndexOnly(IndexName))
	 {
		 cout<<"�������Ѵ���"<<endl;
		 return;
	 }
	 list<DICELETYPE>::iterator pTable;
	 if(!TableExist(TableName,pTable))
	 {
		 cout<<"������"<<endl;
		 return;
	 }
	 if(isMainField(pTable,Filed->at(0))) //������
	 {
//		 cout<<"zhujian"<<endl;
		 //sort()����
		 SortOnMainField(pTable);//������������
		 //��������
		 TableIndexArray.push_back(IndexStr(TableName,Filed->at(0),IndexName,CreatMainIndex(pTable))); 
	 }
	 else
	 {
		 //����������
		 int value=-1;//������������±�
		 int i=0;
		 for(list<FILEDPTYPE_TYPE>::iterator  p=pTable->pFieldPlusTypeList->m_FieldPlusType.begin();p!=pTable->pFieldPlusTypeList->m_FieldPlusType.end();++p,++i)
		 {
			 if(p->first==Filed->at(0))
				 value=i;
		 }
		 if(-1==value)
		 {
			 cout<<"�����ڸ�����"<<endl;
			 return;
		 }
		 TableIndexArray.push_back(IndexStr(TableName,Filed->at(0),IndexName,CreateAssistIndex(pTable,value))); 
	 }
	 WriteTableIndexToFile();
	 cout<<"���������ɹ�"<<endl;
//	 PrintIndexArray();

}
bool DataBase::isHaveIndex(string TableName,string FieldName)//���Ƿ���ĳ�����Ͻ���������
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
bool DataBase::isMainField(list<DICELETYPE>::iterator pTable,string FieldName)//ĳ�����Ƿ���ĳ�������
{
	list<FILEDPTYPE_TYPE>::iterator  p=pTable->pFieldPlusTypeList->m_FieldPlusType.begin();
	if(p->first==FieldName)
		return true;
	return false;
}
 //��һ�����ڵڶ���,�����棬�����ͺ��ַ����в�ͬ�ıȽϷ�ʽ
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
//������������
void DataBase::SortOnMainField(list<DICELETYPE>::iterator pEle)
{
//	cout<<"��������������..."<<endl;
   ElementsCol *pElement=pEle->pElementsCol;
   int line=pEle->pElementsCol->m_line;
   int column=pEle->pElementsCol->m_column;
   for(int k=0;k<line-1;k++)
	   for(int i=0;i<line-1-k;i++)
		   if( Above(pEle,pElement->m_Elements[i][0],pElement->m_Elements[i+1][0])) //��һ�����ڵڶ���,�����棬�����ͺ��ַ����в�ͬ�ıȽϷ�ʽ
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
//��������
IndexFile * DataBase::CreatMainIndex(list<DICELETYPE>::iterator pEle)
{
	IndexFile *pIndexFile=new IndexFile();
	int line=pEle->pElementsCol->m_line;
	pIndexFile->IndexFileArray.push_back(make_pair(pEle->pElementsCol->m_Elements[0][0],0));
	int step=ceill( sqrt(line)/2 );
//	int step=1;//2015.6.17 �޸�Ϊ������Ҳ��ӦΪÿһ�Ϊ�˷���������������
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
//����������
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
	//����
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

//���������
void DataBase::PrintIndexArray()
{
	for(int i=0;i<TableIndexArray.size();i++)
	{
		cout<<TableIndexArray[i].TableName<<" "<<TableIndexArray[i].FiledName<<" "<<TableIndexArray[i].IndexName<<endl;
		TableIndexArray[i].pIndexFile->PrintIndexFile();
	}
}
//�������Ƿ��Ѵ���
bool DataBase::isIndexOnly(string IndexName)
{
	for(int i=0;i<TableIndexArray.size();i++)
	{
         if(IndexName == TableIndexArray[i].IndexName)
			 return false;
	}
	return true;
}
//�����б������д���ļ���
void DataBase::WriteTableIndexToFile()
{
	ofstream fout;
	ofstream foutIndex;//��������ļ���������������
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
//��ȡ�������ļ��������ڴ�
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
//ɾ������
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
		remove( (p->IndexName +".txt").c_str());//�ɹ�����0�����򷵻�EOF
		TableIndexArray.erase(p);
		cout<<"ɾ���������"<<endl;
	}
	else
	{
		cout<<"������������"<<endl;
	}
	WriteTableIndexToFile();
}
//������
void DataBase::PrintTab(list<DICELETYPE>::iterator pTable)
{
	cout<<endl;
	cout<<pTable->TableName+":"<<endl;
	pTable->pFieldPlusTypeList->PrintOnlyType();
	pTable->pFieldPlusTypeList->PrintOnlyField();
    pTable->pElementsCol->PrintElements();
}
//���ܣ����������ʽ��Ԫ��(��������)
//����:pTable��Ч��expstream��Ϊ��
void DataBase::PrintTab(list<DICELETYPE>::iterator pTable,vector<expression> expstream)
{
	cout<<endl;
	cout<<pTable->TableName+":"<<endl;
	pTable->pFieldPlusTypeList->PrintOnlyType();
	pTable->pFieldPlusTypeList->PrintOnlyField();
	//�������������Ԫ��
	pTable->pElementsCol->PrintPartEle(expstream);
}
//����:�������Ԫ���ĳЩ����ֵ
//����:pTable��Ч��Field�������±�
void DataBase::PrintTab(list<DICELETYPE>::iterator pTable,vector<string>  xiabiaoField)
{
	cout<<endl;
	cout<<pTable->TableName+":"<<endl;
	pTable->pFieldPlusTypeList->PrintOnlyType(xiabiaoField);
	pTable->pFieldPlusTypeList->PrintOnlyField(xiabiaoField);
//�������Ԫ���ĳЩ����ֵ
	pTable->pElementsCol->PrintPartField(xiabiaoField);
}
//����:�����������Ԫ���ĳЩ����ֵ
//����:pTable��Ч��Field�������±�
void DataBase::PrintTab(list<DICELETYPE>::iterator pTable,vector<expression> expstream,vector<string>  xiabiaoField)
{
	cout<<endl;
	cout<<pTable->TableName+":"<<endl;
	pTable->pFieldPlusTypeList->PrintOnlyType(xiabiaoField);
	pTable->pFieldPlusTypeList->PrintOnlyField(xiabiaoField);
	pTable->pElementsCol->PrintPartEleField(expstream,xiabiaoField);
}
//���ܣ������
//������Tab��ÿ��Ԫ��ָ����Ч
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
//���ܣ������ʽ�е�����ֵת��Ϊ�±�
//����Ҫ��:���ʽ�е�ÿ�����Զ���NewField��
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
//����:��֤lineԪ���Ƿ���ϵ�һ�������ʽExp
//����:line��Ч�����Ҵ�С��С��exp.left�Ĵ�С�� exp������ֵΪ�±�ֵ
//����ֵ��������ϣ������棻���򣬷��ؼ�
bool isOneConditionFit(vector<string> line,expression exp)
{
	int xiabiao=atoi( exp.left.c_str());
	if("="==exp.op) //�����ڴ洢�����ַ����������Ҳ����������ε�Ҳ����Ҫת��
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
//����:ѡ����ϱ��ʽ��Ԫ�����
//����:���ʽ�е���ֵ���������Ե��±겢���±�С��NewTabԪ��ָ���vector�Ĵ�С
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
					PrintVector(*NewTab[j]);//��������
					break;
				}
				else i++;
			}				
			else
			{
				if(""==xiabiaoexpstream[i].logop)//����������
					break;
				else
				{
					while( i<xiabiaoexpstream.size() && xiabiaoexpstream[i].logop!="or")
						i++;
					if(i>=xiabiaoexpstream.size()) //����������
						break;
					else i++;
				}
			}
		}
	}
}
//���ܣ���pFieldָ���������ȫ����Ϊ��Ӧ�±�
//����:pField��Ч��NewFiled�а�������pFieldָ�������
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
	//cout<<"����"<<endl;
	//for (int i=0;i<pField->size();i++)
	//{
	//	cout<<(*pField)[i]<<" ";
	//}
}
//����:�����������ֵ
//������Field�е�ֵС��NewField�������С
void PrintPartType(vector<string> xiabiaoField,vector<string> NewField)
{
	for(int i=0;i<xiabiaoField.size();i++)
	{
		cout<<std::left<<setw(STRINGWIDTH)<<NewField[atoi(xiabiaoField[i].c_str())];//����0ʱ��ʾ�쳣������ʵ���±�ֵ����Ϊ��
	}
	cout<<endl;
}
//���ܣ����Ԫ��Ĳ�������ֵ
//������Ele�������С����xiabiaoField��ÿһ��Ԫ��(���͵��ַ���)
void PrintPartValue(vector<string> Ele,vector<string> xiabiaoField)
{
	for(int i=0;i<xiabiaoField.size();i++)
	{
		cout<<std::left<<setw(STRINGWIDTH)<<Ele[atoi(xiabiaoField[i].c_str())];//����0ʱ��ʾ�쳣������ʵ���±�ֵ����Ϊ��
	}
	cout<<endl;
}
//���ܣ�ѡ�񲿷�Ԫ��Ĳ����������
//������xiabiaoexpstream��xiabiaoField�е��������Զ����±�ֵ�����±�ֵС��NewTabԪ����ָ����Ĵ�С
void ChoosePrintTable(vector< vector<string> *> NewTab,vector<expression> xiabiaoexpstream,vector<string> xiabiaoField)
{
	//cout<<"ѡ������"<<endl;
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
					PrintPartValue(*NewTab[j],xiabiaoField);//��������
					break;
				}
				else i++;
			}				
			else
			{
				if(""==xiabiaoexpstream[i].logop)//����������
					break;
				else
				{
					while( i<xiabiaoexpstream.size() && xiabiaoexpstream[i].logop!="or")
						i++;
					if(i>=xiabiaoexpstream.size()) //����������
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
			cout<<std::left<<setw(STRINGWIDTH)<<NewTab[i]->at( atoi(xiabiaoField[j].c_str()) );//����0ʱ��ʾ�쳣������ʵ���±�ֵ����Ϊ��
		}
		cout<<endl;
	}
}
//���ܣ������������Ӻ�ѡ��
//������Link����Ϊ�գ�expstream�е�ÿ��Ԫ�ص�right�����ǳ���
//Ϊ�˼򻯳��򣬼�����������Զ�����ȷ��
void DataBase::SelectMultiTab(vector<string> Table,vector<string> Field,vector<LinkCondi> Link,vector<expression> expstream) 
{
	vector<string> NewField;
	vector< vector<string> *> NewTab;
	if ("*"==Field[0]&&Field.size()==1) 
	{
		if(expstream.size()==0&&Link.size()>0)//���������Ӳ���
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
			cout<<"û������������ʱ "<<dfTim<<" s"<<endl;
			NewTab=LinkTwoTabOnIndex(Link[0],NewField);
			PrintVector(NewField);
			PrintTable(NewTab);
			QueryPerformanceCounter(&litmp);  
			QPart1  =   litmp.QuadPart;
			dfMinus = (double)(QPart1 - QPart2);
			dfTim   =   dfMinus   /   dfFreq;
			cout<<"����������ʱ "<<dfTim<<" s"<<endl;
//			cout<<"���������Ӳ���"<<endl;
		}
		else if(expstream.size()>0 &&Link.size()>0)//������ѡ�����Ӳ���
		{
			NewTab=LinkTwoTab(Link[0],NewField);			
			FieldToXiaBiao(&expstream,NewField);
			PrintVector(NewField);
		    ChoosePrintTable(NewTab,expstream);
		}
		else
		{
				cout<<"δ����Ĳ�ѯ"<<endl;
				return;
		}
	}
	else
	{
		if(expstream.size()==0&&Link.size()>0)//����������ͶӰ����
		{
			NewTab=LinkTwoTab(Link[0],NewField);
			FieldToXiaBiao(&Field,NewField);
			PrintPartType(Field,NewField);
			PrintPartValue(NewTab,Field);
//			cout<<"����������ͶӰ����"<<endl;
		}
		else if(expstream.size()>0 &&Link.size()>0)
		{
			NewTab=LinkTwoTab(Link[0],NewField);
			FieldToXiaBiao(&Field,NewField);
			FieldToXiaBiao(&expstream,NewField);
			PrintPartType(Field,NewField);
			ChoosePrintTable(NewTab,expstream,Field);
//			cout<<"������ѡ������ͶӰ����"<<endl;
		}
		else
		{
			cout<<"δ����Ĳ�ѯ"<<endl;
			return;
		}
	}

}
//���ܣ����Field���±꣬��������ڣ�����-1
//������pELe����
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
//����:��pR1��pR2��ָ���Ԫ��ϲ���   ȥ��һ����������,���Ӻ����ͬ������=R1.����(R2.����)   )
//����:p��Ч��pR1��pR2��Ч������linkField1<len1,LinkField2<len2
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

//���ܣ����pleft������������R.name����ʽ�����Ҳ�����������ظ���
//����:pleft��pright��Ч����Table1X��Table2X�����������±�����ֵ,[out]NewField
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
//���ܣ����������Ӳ���,ͶӰ��������
//��������Ҫ��
vector< vector<string> * > DataBase::LinkTwoTab(LinkCondi Link,vector<string> &NewField)
{
	list<DICELETYPE>::iterator pleft,pright;
	int Table1X,Table2X;
	vector< vector<string> * > TempTab;//��ʱ��
	if(TableExist(Link.Table1,pleft)  &&TableExist(Link.Table2,pright) )
	{
		Table1X=ToXiaBiao(pleft,Link.Field);
		Table2X=ToXiaBiao(pright,Link.Field);
		if(-1==Table1X||-1==Table2X)
		{
			cout<<"�����Բ�����"<<endl;
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
						pright->pElementsCol->m_Elements[j], Table2X,pright->pElementsCol->m_column);//�ϲ�
					TempTab.push_back(p);
				}
			}
	//	cout<<"�����"<<endl;
	}	
	else
	{
		cout<<"������"<<endl;
		return TempTab;
	}
	CreateField(pleft,Table1X,pright,Table2X,NewField);
	return TempTab;
}
//����:����Ӧ���±�����ֵת��Ϊ������
//������xiabiaoexpstream�е��±�ֵ����С��FieldType�������С
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
//���ܣ��ж�Ele�Ƿ������������ʽ
//������Ele�е����ͺ�exp.right����һ��
bool isOneConditionFit(string Ele ,expression exp)
{
	if("="==exp.op) //�����ڴ洢�����ַ����������Ҳ����������ε�Ҳ����Ҫת��
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
//���ܣ���������ѡ�����Ԫ�飬����ֵȫ�����
//������pTable��Ч��expstream�е�����ֵΪ�±�ֵ
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
		cout<<"δ��������"<<endl;
		return;
	}
	vector<pair<string,int>> IndexArray=TableIndexArray[i].pIndexFile->IndexFileArray;
	for (int j=0;j<IndexArray.size();j++)
	{
		if( isOneConditionFit(IndexArray[j].first,xiabiaoexpstream) ) //�Ƿ���������
		{
			for(int k=0;k<pTable->pElementsCol->m_column;k++)
				cout<<std::left<<setw(STRINGWIDTH)<< pTable->pElementsCol->m_Elements[IndexArray[j].second][k];
			cout<<endl;
		}
	}

}
//���ܣ��жϱ��Ƿ���Field���Ͻ���������
//������ptable��Ч
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
//����:������������������
//����:��Ҫ��
vector< vector<string> *> DataBase::LinkTwoTabOnIndex(LinkCondi Link,vector<string> NewField)
{
	list<DICELETYPE>::iterator pleft,pright;
	int Table1X,Table2X;
	vector< vector<string> * > TempTab;//��ʱ��
	if(TableExist(Link.Table1,pleft)  &&TableExist(Link.Table2,pright) )
	{
		Table1X=ToXiaBiao(pleft,Link.Field);
		Table2X=ToXiaBiao(pright,Link.Field);
		if(-1==Table1X||-1==Table2X)
		{
			cout<<"�����Բ�����"<<endl;
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
							pright->pElementsCol->m_Elements[Index2[j].second], Table2X,pright->pElementsCol->m_column);//�ϲ�
						TempTab.push_back(p);
					}
				}
			}
		}
		else
		{
			cout<<"��Ҫ����������Զ���������"<<endl;
			return TempTab;
		}
	}	
	else
	{
		cout<<"������"<<endl;
		return TempTab;
	}
	CreateField(pleft,Table1X,pright,Table2X,NewField);
	return TempTab;
}
