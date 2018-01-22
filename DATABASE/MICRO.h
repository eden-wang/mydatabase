#pragma once
//������־
enum {CREATETABLE,DROPTABLE,ALTERTABLE_ADD,ALTERTABLE_DROP,INSERT,MYDELETE,UPDATE,SELECT,CREATEINDEX,DROPINDEX,CREATEUSER,GRANT,REVOKE};
//string��char *���໥ת��
#define COVTOCSTRING(P) P.c_str()   //string-��char *
#define CCOVTOSTRING(CCHARARRAY) string(CCHARARRAY)  //��C�ַ�������ת��ΪString����


//��������ʽ�йصĺ�
//������
#define MATCHLEFT CCOVTOSTRING("\\(") //��ʾ�������Ҫƥ���������
#define MIDDLELEFT CCOVTOSTRING("\(") //��ʾ��������ʽ�е�������
#define MATCHRIGHT CCOVTOSTRING("\\)") //��ʾ�������Ҫƥ���������
#define MIDDLERIGHT CCOVTOSTRING("\)") //��ʾ��������ʽ�е�������
#define SPACE CCOVTOSTRING("\\x20+")  //����һ���ո�
#define EXSPACE CCOVTOSTRING("\\s*")  //����û�пհ׷�,�հ׷�����
#define IDENTIFIER CCOVTOSTRING("[a-zA-Z]\\w*")  //����ĸ��ͷ�ı�ʶ��
#define NUMBER  CCOVTOSTRING("\([1-9]\\d*|0\)")//����
#define VARCHAR CCOVTOSTRING("char\\(")+NUMBER+"\\)"  //char(n)����
#define TYPE CCOVTOSTRING("\(int")+"|"+VARCHAR+"\)"   //֧�ֵ�����Ϊint,char(n)
//��չ��
#define RELATIONNAME IDENTIFIER //��ϵ��
#define WIDEIDENTIFIER  MIDDLELEFT+IDENTIFIER+"."+IDENTIFIER+MIDDLERIGHT  //R.name 2015.6.5���
#define FIELDNAME  MIDDLELEFT+WIDEIDENTIFIER+"|"+IDENTIFIER+MIDDLERIGHT//������ 2015.6.5�޸�Ϊ֧��R.name��name������ʽ 
#define IDENTIFIER_LIST MIDDLELEFT+IDENTIFIER+","+MIDDLERIGHT+"*"+IDENTIFIER  //��ʶ���б�
#define COLUMN_LIST  IDENTIFIER_LIST  //����
#define FIELDNAME_LIST MIDDLELEFT+FIELDNAME+","+MIDDLERIGHT+"*"+FIELDNAME  //�����б� 2015.6.5���R.name
#define BEGINPOS CCOVTOSTRING("^")  //���λ��ƥ��
#define ENDPOS CCOVTOSTRING("$")  //�յ�λ��ƥ��
#define CONSTVALUE MIDDLELEFT+"\'"+IDENTIFIER+"\'"+"|"+"\'"+NUMBER+"\'" +MIDDLERIGHT//����
#define CONSTVALUE_LIST  MIDDLELEFT+CONSTVALUE+","+MIDDLERIGHT+"*"+CONSTVALUE  //�����б�
#define LINKCONDITION  FIELDNAME+"="+FIELDNAME //��������
#define CONDITIONEXP  MIDDLELEFT+ FIELDNAME+MIDDLELEFT+"="+"|"+"!="+MIDDLERIGHT+CONSTVALUE+"|"+FIELDNAME+MIDDLELEFT+"<|>|<=|>=|!=|="+MIDDLERIGHT+NUMBER+"|"+LINKCONDITION+MIDDLERIGHT //һ���������ʽ
#define LOGEXP MIDDLELEFT+CONDITIONEXP+SPACE+"\(and|or\)"+SPACE+MIDDLERIGHT+"*"+CONDITIONEXP//�߼����ʽ
#define ASSIGNMENT COLUMN_LIST+"="+CONSTVALUE //��������������
#define ASSIGNMENT_LIST  MIDDLELEFT+ASSIGNMENT+","+MIDDLERIGHT+"*"+ASSIGNMENT  //��ֵ�б�
#define  RELATIONNAME_LIST  MIDDLELEFT+RELATIONNAME+","+MIDDLERIGHT+"*"+RELATIONNAME  //��ϵ���б�
#define INDEXNAME IDENTIFIER //������
#define USERNAME IDENTIFIER//�û���
#define PASSWORD "\\w+" //����
#define AUTHORITY  MIDDLELEFT+"insert|delete|update|select"+MIDDLERIGHT //Ȩ��
#define AUTHORITY_LIST MIDDLELEFT+AUTHORITY+","+MIDDLERIGHT+"*"+AUTHORITY  //Ȩ���б�
#define USERNAME_LIST MIDDLELEFT+USERNAME+","+MIDDLERIGHT+"*"+USERNAME  //�û��б�

#define CHARLEN_MAX 100 //char *�������󳤶�
//���ṹ�йصĺ�
#define  ELEMENTMAX 100//���Ԫ���������    
#define FIELDMAX 50//������Ե�������

#define STRINGWIDTH 15//�ַ������



