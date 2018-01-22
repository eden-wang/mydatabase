//��MySystem�д����﷨
//��DataBase�д�������

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
	string command; //����
public:
	inline void ClearCommand(){command.clear();};//�������
	void InputCommand();
	bool AnalysisCommand(int &flag);//������� flag[out]Ϊ��Ӧ������ĺ��־ ;����ֵΪ��,����������ȷ
	void DealWithCommand(int flag);//���ݺ��־��������
//��������
public:
	inline string GetCommand(){return command;}
//	˽�и�������
private:
	void    CreateTable();//�������
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
	//�������������ĺ����ͳ�Ա����
private:
	DataBase database;
public:
	inline void WriteToFile(){database.WriteToFile();}//����������д���ļ���
	inline void PowerOnDataBase(){database.PowerOnDataBase();}
	inline void PrintTables(){database.PrintDictionary();}
private:
	vector<string> SpliteCommand();//�ָ��ַ���
	//�������������ӵĺ���
public:
	//���ܣ���Exp�е��ַ����ָ�Ϊ����������ѡ������
	//����:[out]Link,[out]Choose
	bool Seperate(vector<string>Exp,vector<LinkCondi> &Link,vector<string> &Choose);


//Ȩ��
public:
	//���ܣ�ʹ���û����������½
	void LogOn();
//��ѯ�Ż�
public:
	//���ܣ���ѯ�Ż�
	//����:Table.size()>=3,Field����*,���������ܴ��ڱ���,��expstream������������ѡ����������ȷ
	void QueryOptimize(vector<string> Table,vector<string> Field,vector<string> expstream);

};

//********************��ѯ�Ż�********************//


//����:��ʼ��ϵ��
//������Table.size()>=3,Field����*,���������ܴ��ڱ���,��expstream������������ѡ����������ȷ
void InitilizeTree(vector<pair<string,int> > &Tree, vector<string> Table, vector<string> expstream, vector<string> Field);
