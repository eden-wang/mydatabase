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
typedef pair<string,string> FILEDPTYPE_TYPE; //�����ԣ��������͡�
struct expression
{
	string left;//�������
	string op;//��������
	string right;//�Ҳ�����
	string logop;//�Ƿ�����һ�����ʽ�в�����ϵ and or 
};
class ElementsCol //Ԫ�鼯��
{
public://����������ʱ�������ݽṹ���������⣬Ӧ������Ϊpublic �޸���2015.5.25
	string m_Elements[ELEMENTMAX][FIELDMAX];//Ԫ������ֵΪELEMENTMAX�����Ե����ֵΪFIELDMAX
	int m_line;
	int m_column;
public://��̬��Ա����
   static const int MaxLine=ELEMENTMAX;
	static const int MaxColumn=FIELDMAX;
public:	
	ElementsCol(){	m_line=0;m_column=0;};
	void SetColumn(int column){m_column=column;}
	void PrintElements();
	void WriteElementsToFile(ofstream &out);//��Ԫ��д���ļ���
	inline void SetElement(int i,int j,string t){m_Elements[i][j]=t;}
	inline void SetLineAndColumn(int x,int y){m_line=x;m_column=y;}
	bool Insert(vector<string> XiaBiao,vector<string> value);//����һ��Ԫ�飬�����������Ե�ֵ
//	bool InsertAccordingField(vector<unsigned int> flags,vector<string> value);//Field��value�Ķ�Ӧ����Ӧ����ȣ�û��ָ����������NULL��ֵ
 	bool AddNewField();
	void DropField( vector<string> Xiabiao);
	bool DeleteEle(vector<string> Exp,list<FILEDPTYPE_TYPE> FieldType);
	void DeleteAllEle(){m_line=0;}
	bool update(vector< pair<string,string> > XiaBiaoValue,vector<expression> xiabiaoexpstream);


private:
	bool isEleExist(string EleName,int &pos);//�ж�Ԫ���Ƿ����
	bool isFit(string *a,int n,vector<expression> expstream,list<FILEDPTYPE_TYPE> FieldType);//�ж��Ƿ���ϱ��ʽ
	bool isFit(string *line,vector<expression> xiabiaostream);
	//ɾ����n�У��ӵ�0�п�ʼ��
	void DeleteLine(int n);
	bool isOneConditionFit(string *line,int n,expression exp,list<FILEDPTYPE_TYPE> FieldType);//�ж��Ƿ���ϵ�һ���������ʽ
	bool isOneConditionFit(string *line,expression exp);
	//20120515������Ϊ��
	bool  DetectFieldType(vector<string> Exp,list<FILEDPTYPE_TYPE> FieldType);//�������������ȷ�ԣ��Ҳ������������Ƿ�ƥ��
	bool isKeyChong(vector<string> XiaBiao,vector<string> value);
public:
	//����:�������������Ԫ��(��������)
	//����:����ֵ��Ϊ�±�ֵ
	void PrintPartEle(vector<expression> expstream);
	//���ܣ��������Ԫ��Ĳ�������
	//����:����ֵ���������±�ֵ
	void PrintPartField(vector<string>  xiabiaoField);
	//���ܣ������������Ԫ��Ĳ�������
	//����:����ֵ������ת��Ϊ�±�ֵ
	void PrintPartEleField(vector<expression> expstream,vector<string>  xiabiaoField);
};
class DataBase;
class  FieldPTypeList
{
public:
	//2015.5.27��Ϊ���г�Ա
	list<FILEDPTYPE_TYPE> m_FieldPlusType;
	inline int GetSize(){return m_FieldPlusType.size();}
	inline	void SetFieldPType(list<FILEDPTYPE_TYPE> FieldPlusType)//��m_FieldPlusType��ֵ
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
	void DropField(vector<string> Field) //��֤field�е��������Զ����ڸ�������
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
