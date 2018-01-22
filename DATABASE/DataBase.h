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
//�������������ӵ��ࡢ�ṹ��ͺ���
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
typedef struct  _DICELETYPE //�����ֵ䵥��Ԫ�ض���
{
	string TableName; //����
	ElementsCol *pElementsCol; //ָ��Ԫ�鼯��
	FieldPTypeList  *pFieldPlusTypeList; //ָ�����ԣ��������͡��б�
	_DICELETYPE(){pElementsCol=NULL;pFieldPlusTypeList=NULL;}

}DICELETYPE;

 class DataBase
{
private:
    list<DICELETYPE> m_DataDic; //����
public:
	bool CreatTable(string tablename,list<FILEDPTYPE_TYPE> FieldPlusType );//ע�⣬Ϊ�˷��㣬���Ƕ���char����ֻ����char����ֵ���确char(15)��ֻ���롮15��
	void WriteToFile();//������д���ļ���
	void PowerOnDataBase();//�������ݿ�:���ļ��ж�ȡ��Ϣ
	bool DropTable(string tablename);//ɾ����
	bool insert(string tablename,vector<string> Field,vector<string> value);//����
	bool AlterTableAdd(string tablename,string Field,string Type);
	bool AlterTableDrop(string tablename,vector<string> Field);
	bool MyDelete(string tablename,vector<string> Exp);
	bool update(string tablename,vector< pair<string,string> > FieldValue,vector<expression> expstream);
	void Select(vector<string>  Field,vector<string> Table,vector<expression>  expstream);
	bool DropAllEle(string tablename);

//����ʹ��
public:
	void PrintDictionary();
//��������,����Ҫ����κ���Ϣ
private:
	bool TableExist(string tablename,list<DICELETYPE>::iterator &pTable); //�жϱ���Ƿ����
	bool isMatch(vector< pair<string,string> >& FieldValue ,list<DICELETYPE>::iterator pEle); ////����������Ƿ�ƥ�䣬���������Ƿ���ȷ������ ���������޸�Ϊ�±�ֵ
	bool isMatch(vector< expression >& FieldValue ,list<DICELETYPE>::iterator pEle); ////����������Ƿ�ƥ�䣬���������Ƿ���ȷ������ ���������޸�Ϊ�±�ֵ
	bool isMatchField(list<DICELETYPE>::iterator pEle,vector<string> Field);//�������Ƿ��ڱ�������б���
	bool isMatchType(list<DICELETYPE>::iterator pEle,vector<string> &Field,vector<string> value);//�ж������Ƿ�һ�� 
	void FieldToflags(list<DICELETYPE>::iterator pEle,vector<string> &Field);//��������ת��Ϊ�±꣬���뱣֤Field�е��������Զ���pEleָ��ı���
    //��pEleָ����������Ը�������Field
	void AddAllField(list<DICELETYPE>::iterator pEle,vector<string> &Field);


private:
	vector<IndexStr> TableIndexArray;//���б������
	//����������ӵĺ���
public:
	//��������,֧��һ������
	void CreateIndex(string TableName,string IndexName,vector<string> *Filed);
	//ɾ������
	void DropIndex(string IndexName);
	//���������
	void PrintIndexArray();

private:
	bool isHaveIndex(string TableName,string FieldName);//���Ƿ���ĳ�����Ͻ���������
	//�������Ƿ��Ѵ���
	bool isIndexOnly(string IndexName);
	bool isMainField(list<DICELETYPE>::iterator pEle,string FieldName);//ĳ�����Ƿ��Ǹñ������
	//������������
	void SortOnMainField(list<DICELETYPE>::iterator pEle);
	 //��������,����ָ�������ļ���ָ��
	IndexFile * CreatMainIndex(list<DICELETYPE>::iterator pEle);
	//����������,����ָ�������ļ���ָ��
	IndexFile *  CreateAssistIndex(list<DICELETYPE>::iterator pEle,int pos);
	//�����б������д���ļ���
	void WriteTableIndexToFile();
	//��ȡ�������ļ��������ڴ�
	void ReadTableIndexFromFile();

//Select��ӵĺ���
public:
	//���ܣ������������Ӻ�ѡ��
	//��������Ҫ��
	void SelectMultiTab(vector<string> Table,vector<string> Field,vector<LinkCondi> Link,vector<expression> expstream);//������
private:
	//���ܣ����������Ӳ�����ͶӰ��������
	//������Link��ֻ��2���������Դ��� ,[out]NewField(�±������б�)
	//����ֵ�����Ӻ�ı�
	vector< vector<string> * > LinkTwoTab(LinkCondi Link,vector<string> &NewField);
	//���ܣ���������ѡ�����Ԫ�飬����ֵȫ�����
	//������pTable��Ч��expstream��
	void PrintTabOnIndex(list<DICELETYPE>::iterator pTable,expression  xiabiaoexpstream);
private:
	//����:������
	//����Ҫ��:pTable��Ч����Ϊ��
	void PrintTab(list<DICELETYPE>::iterator pTable);
	//���ܣ����������ʽ��Ԫ��
	//����:pTable��Ч��expstream��Ϊ��(��������)
	void PrintTab(list<DICELETYPE>::iterator pTable,vector<expression> expstream);
	//����:�������Ԫ���ĳЩ����ֵ
	//����:pTable��Ч��Field�������±�
	void PrintTab(list<DICELETYPE>::iterator pTable,vector<string>  Field);
	//����:�����������Ԫ���ĳЩ����ֵ
	//����:pTable��Ч��Field�������±�
	void PrintTab(list<DICELETYPE>::iterator pTable,vector<expression> expstream,vector<string>  Field);

//����
public:
	//����:������������������
	//����:��Ҫ��
	vector< vector<string> *> LinkTwoTabOnIndex(LinkCondi Link,vector<string> NewField);
	//���ܣ��жϱ��Ƿ���Field���Ͻ���������
	//������ptable��Ч
	vector< pair<string,int> > GetIndexFile(list<DICELETYPE>::iterator pTable,string Field);
};

 //���ܣ����Field���±꣬��������ڣ�����-1
 //������pELe����
 int ToXiaBiao(list<DICELETYPE>::iterator pEle,string Field);
 //���ܣ����pleft������������R.name����ʽ�����Ҳ�����������ظ���
 //����:pleft��pright��Ч����Table1X��Table2X�����������±�����ֵ,[out]NewField
 void CreateField(list<DICELETYPE>::iterator pleft,int Table1X,list<DICELETYPE>::iterator pright,int Table2X,vector<string> &NewField);
 //���ܣ������
 //������Tab��ÿ��Ԫ��ָ����Ч
 void PrintTable(vector<vector<string> *> Tab);
 //����:ѡ����ϱ��ʽ��Ԫ�����
 //����:���ʽ�е���ֵ���������Ե��±겢���±�С��NewTabԪ��ָ���vector�Ĵ�С
 void ChoosePrintTable(vector< vector<string> *> NewTab,vector<expression> xiabiaoexpstream);
 //����:��֤lineԪ���Ƿ���ϵ�һ�������ʽExp
 //����:line��Ч�����Ҵ�С��С��exp.left�Ĵ�С�� exp������ֵΪ�±�ֵ
 //����ֵ��������ϣ������棻���򣬷��ؼ�
 bool isOneConditionFit(vector<string> line,expression exp);
 //���ܣ���pFieldָ���������ȫ����Ϊ��Ӧ�±�
 //����:pField��Ч��NewFiled�а�������pFieldָ�������
 void FieldToXiaBiao(vector<string> *pField,vector<string> NewField);
 //����:�����������ֵ
 //������Field�е�ֵС��NewField�������С
 void PrintPartType(vector<string> xiabiaoField,vector<string> NewField);
 //���ܣ�ѡ�񲿷�Ԫ��Ĳ����������
 //������xiabiaoexpstream��xiabiaoField�е��������Զ����±�ֵ�����±�ֵС��NewTabԪ����ָ����Ĵ�С
 void ChoosePrintTable(vector< vector<string> *> NewTab,vector<expression> xiabiaoexpstream,vector<string> xiabiaoField);
 //���ܣ����Ԫ��Ĳ�������ֵ
 //������Ele�������С����xiabiaoField��ÿһ��Ԫ��(���͵��ַ���)
 void PrintPartValue(vector<string> Ele,vector<string> xiabiaoField);



 //����������ɲ�ѯ
 //����:����Ӧ���±�����ֵת��Ϊ������
 //������xiabiaoexpstream�е��±�ֵ����С��FieldType�������С
 string ToField(list<FILEDPTYPE_TYPE> FieldType,expression xiabiaoexpstream);
