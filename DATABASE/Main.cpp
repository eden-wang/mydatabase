// DATABASE.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <iostream>
#include <string>
#include "MySystem.h"
using namespace std;
void Version(); //�汾�ţ�����˵��
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
		    cout<<"�������"<<endl;
		}
		else
		{
//			cout<<"��ȷ����  "<<endl;
			S.DealWithCommand(flag);
		}
		
	}
	return 0;
}
void Version()
{
	cout<<"֧�ֻ��У��ԷֺŽ�������"<<endl;
	cout<<"��������Сд"<<endl;
	cout<<"����\"exit;\"�˳�"<<endl;
	cout<<"֧�ֵ����Ԫ���� "<<ElementsCol::MaxLine<<endl;
	cout<<"֧�ֵ�������Ը��� "<<ElementsCol::MaxColumn<<endl;
	cout<<"insert ��֧�ֽ��渳ֵ"<<endl;
	cout<<"Ĭ�ϵ�һ�����Ե�����"<<endl;
	cout<<"insert��һ�����Ա���������"<<endl;
	cout<<"����������ʱ���Ե����λ�ò��ܷ����仯"<<endl;
	cout<<"��ʵ�����ݽṹ����ʧ�󣬵��ºܶ��������൱����(�����Ԫ��ͱ�����Զ���Ϊ�����࣬����Ϊ�����ˣ�ʵ���ϸ�����)"<<endl;
	cout<<"��������֧��һ������ֵ����֪���������ֵ��ɶ�ã������Ƕ�������԰�"<<endl;
	cout<<"�����ڽ�����������ʱ���������Ը�������Ҫ������������֮ǰ����Ȼ�ڽ�����������ʱ���ı�Ԫ��Ĵ���;������˵���������ǲ�Ӧ�øı�Ԫ�鼯�ϵ�"<<endl;
	cout<<"û��ά������"<<endl;
	cout<<"Ĭ�ϵ�������������Ϊname����ʽ"<<endl;
	cout<<"���ΪR.name����ʽ"<<endl;
	cout<<"�����������������������R.name=R1.name"<<endl;
	cout<<"������ʱ��������ֻ��д��ѡ��������ǰ��"<<endl;
	cout<<"��������֮��ֻ����and����ʽ����"<<endl;
	cout<<"����Ŀǰֻ֧��2����"<<endl;
	cout<<"��֧�����㿪ͷ������"<<endl;
	cout<<"������û�ж���������֤,���ܳ���δ�������"<<endl;
	cout<<"Ϊ�������������㣬������Ҳ��ȫ����"<<endl;
	//2015.6.18�޸�
	cout<<"ʹ��3��������Ϊ��ѯ�Ż�����"<<endl;
	cout<<"��ѯ�Ż�ѡ������ֻ����and���ӣ�����ʹ��or"<<endl;
	//2015.6.19���
	cout<<"ע���û�δ����"<<endl;
}

//2015.6.16����bug�����values�ĳ���Ϊ'0',����ʧ��
