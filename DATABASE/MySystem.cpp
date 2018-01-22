#include "MySystem.h"
#include "deelx.h"
#include <iostream>
using namespace std;
inline void EraseEndFlag(string &t)
{
	t.erase(t.end()-1);
}
void MySystem::InputCommand()
{
	string t; 
	int line=1;
	cout<<"SQL>"<<line++<<" ";
	getline(cin,t);
	while (1)
	{
		if(!t.empty())   
		{
			if(t.at(t.length()-1)==';')
				break;
			command+=t;
			command+='\n'; 
		}
		cout<<"    "<<line++<<" ";
		getline(cin,t);
	}
	if(1==t.length())
		EraseEndFlag(command);//ȥ��ֻ��һ���ֺ�ռ��һ�ж�����һ���س��������
	EraseEndFlag(t);
	command+=t;

}
static void InitREGEXP(int flag,string &REGEXP) ;//ͨ����ͬ�ı�־��ʼ��REGEXP��REGEXP����������ʽ�ַ���
bool MySystem::AnalysisCommand(int &flag)//������� flag[out]Ϊ��Ӧ������ĺ��־ ;����ֵΪ��,����������ȷ
{
	string REGEXP;
	CRegexpT <char> regexp;
	MatchResult result ;
	for(int i=CREATETABLE;i<=REVOKE;i++)
	{
		InitREGEXP(i,REGEXP); //ͨ����ͬ�ı�־��ʼ��REGEXP��REGEXP����������ʽ�ַ���	
		regexp.Compile ( (COVTOCSTRING(REGEXP)) );
		result= regexp.Match(COVTOCSTRING(command));
		if( result.IsMatched() )
		{
			flag=i;
			return true;
		}
	}
	return false;
}
void MySystem::DealWithCommand(int flag)//���ݺ��־��������
{
    
	switch (flag)
	{
	case CREATETABLE:
		CreateTable();
		WriteToFile();//ÿִ����һ����䣬�������ļ����ݿ�
		break;
	case DROPTABLE:
		DropTable();
		WriteToFile();//ÿִ����һ����䣬�������ļ����ݿ�
		break;
	case ALTERTABLE_ADD:
		AlterTableAdd();
		WriteToFile();//ÿִ����һ����䣬�������ļ����ݿ�
		break;
	case ALTERTABLE_DROP:
		AlterTableDrop();
		WriteToFile();//ÿִ����һ����䣬�������ļ����ݿ�
		break;
	case INSERT:
		Insert();
		WriteToFile();//ÿִ����һ����䣬�������ļ����ݿ�
		break;
	case MYDELETE:
		myDelete();
		WriteToFile();//ÿִ����һ����䣬�������ļ����ݿ�
		break;
	case UPDATE:
		Update();
		WriteToFile();//ÿִ����һ����䣬�������ļ����ݿ�
		break;
	case SELECT:
		Select();
		break;
	case CREATEINDEX:
		CreateIndex();
		break;
	case DROPINDEX:
		DropIndex();
		break;
	case CREATEUSER:
		CreateUser();
		break;
	case GRANT:
		Grant();
		break;
	case REVOKE:
		Revoke();
		break;
	default:
		break;
	}
}

static void InitREGEXP(int flag,string &REGEXP) //ͨ����ͬ�ı�־��ʼ��REGEXP��REGEXP����������ʽ�ַ���
{
	REGEXP=BEGINPOS	+MIDDLELEFT; //��ʼ
	switch (flag)
	{
	case CREATETABLE:
		REGEXP+="create"+SPACE+"table"+SPACE+RELATIONNAME+EXSPACE
										+MATCHLEFT+EXSPACE
												        +MIDDLELEFT
														       +FIELDNAME+SPACE+TYPE+","+EXSPACE
													    +MIDDLERIGHT
														+"*"
										                +FIELDNAME+SPACE+TYPE
									   +MATCHRIGHT;		
		break;
	case DROPTABLE:
		REGEXP+= "drop"+SPACE+"table"+SPACE+RELATIONNAME;					
		break;
	case ALTERTABLE_ADD:
				REGEXP+="alter"+SPACE+"table"+SPACE+RELATIONNAME+SPACE
									      +"add"+SPACE+FIELDNAME+SPACE+TYPE;							 
		break;
	case ALTERTABLE_DROP:
						REGEXP+="alter"+SPACE+"table"+SPACE+RELATIONNAME+SPACE
									 +"drop"+SPACE+FIELDNAME_LIST;							 
	   break;
	case INSERT:
				REGEXP+="insert"+SPACE+"into"+SPACE+RELATIONNAME
									      +MIDDLELEFT
													+EXSPACE+"|"+EXSPACE+ MATCHLEFT+COLUMN_LIST+MATCHRIGHT 
										  +MIDDLERIGHT 
										  +EXSPACE+"values"+MATCHLEFT+CONSTVALUE_LIST+MATCHRIGHT;							
		break;
	case MYDELETE:
				REGEXP+="delete"+SPACE+"from"+SPACE+RELATIONNAME
										 +MIDDLELEFT
											  + EXSPACE+ "|"+EXSPACE+"where"+ SPACE+LOGEXP
										 +MIDDLERIGHT;	
		//		cout<<REGEXP<<endl;
		break;
	case UPDATE:
				REGEXP+="update"+SPACE+RELATIONNAME+EXSPACE
								    + "set"+SPACE+ASSIGNMENT_LIST
								    +MIDDLELEFT
											  + EXSPACE+ "|"+EXSPACE+"where"+ SPACE+LOGEXP
									+MIDDLERIGHT;				
		break;
	case SELECT:
				REGEXP+="select"+SPACE
									 +MIDDLELEFT
									     +"\\*|"
										 +MIDDLELEFT
										     +FIELDNAME_LIST
										 +MIDDLERIGHT
									 +MIDDLERIGHT
									 +EXSPACE+"from"+SPACE+RELATIONNAME_LIST
									 +MIDDLELEFT
									    +EXSPACE+ "|"+EXSPACE+"where"+ SPACE+LOGEXP
									 +MIDDLERIGHT;							
		break;
	case CREATEINDEX:
				REGEXP+="create"+SPACE+"index"+SPACE+INDEXNAME+SPACE+"on"+SPACE+RELATIONNAME+MATCHLEFT+FIELDNAME_LIST+MATCHRIGHT;
		break;
	case DROPINDEX:
				REGEXP+= "drop"+SPACE+"index"+SPACE+INDEXNAME;
		break;
	case CREATEUSER:
				REGEXP+="create"+SPACE+"user"+SPACE+USERNAME+SPACE+"identified"+SPACE+"by"+SPACE+PASSWORD;
		break;
	case GRANT:
				REGEXP+="grant"+SPACE+AUTHORITY_LIST+EXSPACE
								+"on"+SPACE+RELATIONNAME+EXSPACE
								+"to"+SPACE+USERNAME_LIST;
		break;
	case REVOKE:
				REGEXP+="revoke"+SPACE+AUTHORITY_LIST+EXSPACE
								+"on"+SPACE+RELATIONNAME+EXSPACE
								+"from"+SPACE+USERNAME_LIST;
		break;
	default:
		break;
	}
	REGEXP+=ENDPOS+MIDDLERIGHT; //����
}
template <class T>
 void PrintVector(vector<T> vec,char sep=' ')
{
	for(int i=0;i<vec.size();i++)
		cout<<vec.at(i)<<sep;
	cout<<endl;
}
 void PrintVector_Pair(vector < pair<string,string> > vec)
{
	for(int i=0;i<vec.size();i++)
		cout<<vec.at(i).first<<" "<<vec.at(i).second<<endl;
	cout<<endl;
}
static void GetTablePlusField(vector<string> FilterStream,string &tablename,list <FILEDPTYPE_TYPE> &FieldPlusType)//��ʼ���������������б�
{
	//���ô˺���ǰ����ȷ��create table ��������ȷ��,������ܻ����δ�������Ϊ
	if( (FilterStream.at(0)=="create" )&& (FilterStream.at(1)=="table"))
	{
		tablename=FilterStream.at(2);
		pair<string,string> FieldType; 
		for(unsigned int i=3;i<FilterStream.size();)
		{
			FieldType.first=FilterStream.at(i++);
			if(FilterStream.at(i)=="char")  //char ��һ��������
			{
				FieldType.second=FilterStream.at(++i);//����
				i++;//��һ������
			}
			else FieldType.second=FilterStream.at(i++);
			FieldPlusType.push_back(FieldType);
		}
	}
 //   PrintVector_Pair(FieldPlusType);
}
void MySystem::CreateTable()//�������
{
	list <FILEDPTYPE_TYPE> FieldPlusType;
	string tablename;
	vector<string> FilterStream=SpliteCommand();
//   PrintVector(FilterStream);
   GetTablePlusField(FilterStream,tablename,FieldPlusType);//��ʼ���������������б�
   if(database.CreatTable(tablename,FieldPlusType))//����������tablename �ģ��������б�����FieldPlusType��
	   cout<<"������ "<<tablename<<" �ɹ�"<<endl;
   else 
	   cout<<"������ "<<tablename<<" ʧ��"<<endl;

}

void 	MySystem::DropTable()
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
	if(FilterStream[0]=="drop" &&FilterStream[1]=="table")
		tablename=FilterStream[2];
	if(database.DropTable(tablename))
		cout<<"ɾ����� "<<tablename<<" �ɹ�"<<endl;
	else 
   		cout<<"ɾ����� "<<tablename<<" ʧ��"<<endl;
}

void 	MySystem::AlterTableAdd()
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
	string Field;
	string Type;
	if(FilterStream.size()>=6)
	{
		if(FilterStream[0]=="alter"&&FilterStream[1]=="table"&&FilterStream[3]=="add")
		{
			tablename=FilterStream[2];
			Field=FilterStream[4];
			Type=FilterStream.size()==6 ?FilterStream[5]:FilterStream[6];
			if(database.AlterTableAdd(tablename,Field,Type))
					cout<<"���� "<<tablename<<" ������� "<<Field<<" �ɹ�"<<endl;
			else 
   					 cout<<"���� "<<tablename<<" ������� "<<Field<<" ʧ��"<<endl;
		}
		else 
		{
				cout<<"���Ǹ�����������"<<endl;
				return;
		}
	}
	else
			cout<<"������Եķָ��ʧ��"<<endl;
}

void 	MySystem::AlterTableDrop()
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
	vector<string> Field;
	if(FilterStream.size()>=5)
	{
		if(FilterStream[0]=="alter"&&FilterStream[1]=="table"&&FilterStream[3]=="drop")
		{
			tablename=FilterStream[2];
			for(int i=4;i<FilterStream.size();i++)
				Field.push_back(FilterStream[i]);
			if(database.AlterTableDrop(tablename,Field))
					cout<<"�Ա�� "<<tablename<<" ɾ������ "<<" �ɹ�"<<endl;
			else 
   					 cout<<"�Ա�� "<<tablename<<" ɾ������ "<<" ʧ��"<<endl;
		}
		else 
		{
				cout<<"����ɾ����������"<<endl;
				return;
		}
	}
	else
			cout<<"������Եķָ��ʧ��"<<endl;
}

void 	MySystem::Insert() //���ô˺��������������ȷ�ģ���������
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
	vector<string> Field;
	vector<string> value;
	if(FilterStream[0]=="insert" &&FilterStream[1]=="into")
	{
			tablename=FilterStream[2];
			int i=3;
			while(FilterStream[i]!="values")
			{
				Field.push_back(FilterStream[i++]);
			}
			i++;//����values
			while(i<FilterStream.size())
			{
				value.push_back(FilterStream[i++]);
			}
	}

	if(database.insert(tablename,Field,value))
	    cout<<"������ "<<tablename<<" �ɹ�"<<endl;
    else 
	    cout<<"������ "<<tablename<<" ʧ��"<<endl;
}

void 	MySystem::myDelete()
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
	if(FilterStream.size()==3)
	{
		tablename=FilterStream[2];
		if(database.DropAllEle(tablename))
			cout<<"ɾ�� �ɹ�"<<endl;
		else 
			cout<<"ɾ��ʧ��"<<endl;
	}
	else if(FilterStream.size()>3)
	{
		tablename=FilterStream[2];
		vector<string> Exp;
		for(int i=4;i<FilterStream.size();i++)
			Exp.push_back(FilterStream[i]);
//		PrintVector(Exp,'\n'); 
		database.MyDelete(tablename,Exp);
		cout<<"ɾ�����"<<endl;

	}
}
extern vector<expression> SeperateOp(vector<string> Exp);
void 	MySystem::Update()
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
//	PrintVector(FilterStream);
	vector< pair<string,string> > FieldValue;
	if(FilterStream[0]=="update" &&FilterStream[2]=="set")
	{
			tablename=FilterStream[1];
			int i=3;
			while(i<FilterStream.size()&& FilterStream[i]!="where")
			{
				(FilterStream[i]).erase(FilterStream[i].end()-1); //ȥ�����һ���ַ�"="
				FieldValue.push_back( make_pair( FilterStream[i],FilterStream[i+1]));
				i+=2;
			}
//			PrintVector_Pair(FieldValue);
			vector<string> Exp;
			if(i<FilterStream.size())//����where
			{
				i++;//����where				
				for(int j=i;j<FilterStream.size();j++)
				    	Exp.push_back(FilterStream[j]);
			}
//			PrintVector(Exp);
			if(database.update(tablename,FieldValue,SeperateOp(Exp)))
			    cout<<"��ɸ���"<<endl;
			else
				cout<<"����ʧ��"<<endl;
	}
}

void 	MySystem::Select()
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
//	cout<<"dadfadsf"<<endl;
//	PrintVector(FilterStream,'\n');
	vector<string>  Field;
	vector<string>  Exp;
	vector<string> Table;
	if( FilterStream[0]=="select" ) //select * from R
	{
		int i=1;
		while(FilterStream[i]!="from")
		{
			Field.push_back(FilterStream[i++]);
		}
		i++;
		while (i<FilterStream.size()&&FilterStream[i]!="where")
		{
			Table.push_back(FilterStream[i++]);
		}
		if(i<FilterStream.size())
		{			
    			i++;
				while(i<FilterStream.size())
				{
					Exp.push_back(FilterStream[i++]);
				}			
		}
		if(Table.size()==1) //����
			database.Select(Field,Table,SeperateOp(Exp));
		else if(Table.size()==2)//������Ϊ�˷��㣬ԭ��������֧��2���������ϵĺ���Ŀǰ����ֻ֧��2����
		{	
			vector<LinkCondi> Link;
			vector<string> Choose;
			if(Seperate(Exp,Link,Choose))
				database.SelectMultiTab(Table,Field,Link,SeperateOp(Choose) );
		}
		else
		{
			QueryOptimize(Table,Field,Exp);
		}
	}
//	PrintVector(Field);
//	PrintVector(Table);
//	PrintVector(SeperateOp(Exp));
}

void 	MySystem::CreateIndex()
{
//	cout<<"������"<<command<<endl;
	vector<string> FilterStream=SpliteCommand();
//	PrintVector(FilterStream,'\n');
	string IndexName,Table;
	vector<string> Field;
	if(FilterStream.size()>=5)
	{ 
		IndexName=FilterStream[2];
		Table=FilterStream[4];
		for(unsigned int i=5;i<FilterStream.size();i++)
		{
			Field.push_back(FilterStream[i]);
		}
//		cout<<"Index= "<<Index<<endl;
//		cout<<"Table= "<<Table<<endl;
//		PrintVector(Field,' ');
//		cout<<endl;
		database.CreateIndex(Table,IndexName,&Field);
	}
	else
	{
		cout<<"�ָ����������"<<endl;
	}
}

void 	MySystem::DropIndex()
{
	vector<string> FilterStream=SpliteCommand();
	database.DropIndex(FilterStream[2]);
}

void 	MySystem::CreateUser()
{
	vector<string> FilterStream=SpliteCommand();
	string User=FilterStream[2],Password=FilterStream[5];
	ofstream fout("User.txt",ios_base::app);
	fout<<std::left<<setw(STRINGWIDTH)<<User<<Password<<endl;
	fout.close();
	cout<<"ע�����"<<endl;
}
//ȱ�ݣ�δ���������飻δ����
void 	MySystem::Grant()
{
	vector<string> FilterStream=SpliteCommand();
	string tablename;
	vector< string > author;
	int i=1;
	while(FilterStream[i]!="on")
	{

		author.push_back(FilterStream[i++]);
	}
	i++;//on
	tablename=FilterStream[i++];
	i++;//to
	vector<string> user;
	for(int j=i;j<FilterStream.size();j++)
		user.push_back(FilterStream[j]);
 //   PrintVector(author);
	//cout<<tablename<<endl;
	//PrintVector(user);	
    ofstream fout("Author.txt",ios_base::app);
	for (int i=0;i<user.size();i++)
		for (int j=0;j<author.size();j++)
			fout<<std::left<<setw(STRINGWIDTH)<<user[i]<<setw(STRINGWIDTH)<<tablename<<setw(STRINGWIDTH)<<author[j]<<endl;	
	fout.close();
	cout<<"���"<<endl;
}
//�ٶ����ڲ���û���������
void MySystem::Revoke()
{
	vector<string> FilterStream=SpliteCommand();
	string tablename;
	vector< string > author;
	int i=1;
	while(FilterStream[i]!="on")
	{

		author.push_back(FilterStream[i++]);
	}
	i++;//on
	tablename=FilterStream[i++];
	i++;//to
	vector<string> user;
	for(int j=i;j<FilterStream.size();j++)
		user.push_back(FilterStream[j]);
	//  PrintVector(author);
	//cout<<tablename<<endl;
	//PrintVector(user);	
    ifstream fin("Author.txt");
	AuthorStr T;
	string str;
	vector<AuthorStr> AuthorList;
	while (fin>>T.User>>T.Table>>T.Author)
	{
		AuthorList.push_back(T);
	}
	fin.close();
	vector<AuthorStr>::iterator  p;
	for (int i=0;i<user.size();i++)
        for(int j=0;j<author.size();j++)
		{
				for (p=AuthorList.begin();p!=AuthorList.end();++p)
				{
					if (p->User==user[i] &&p->Table==tablename &&p->Author==author[j])
							break;
				}
				if(p!=AuthorList.end())
						AuthorList.erase(p);
		}
	ofstream fout("Author.txt");
	for (int i=0;i<AuthorList.size();i++)
	{
		fout<<std::left<<setw(STRINGWIDTH)<<AuthorList[i].User<<setw(STRINGWIDTH)<<AuthorList[i].Table<<setw(STRINGWIDTH)<<AuthorList[i].Author<<endl;	
	}
	fout.close();
	cout<<"��� "<<endl;
}
vector<string> MySystem::SpliteCommand()
{	
	vector<string> FilterStream;
	//������ֵ��ַ������� 
	string T=command;
	char *token=new char[CHARLEN_MAX];
	char *Deli=" ,()\'\n";//�пո�
	token = strtok((char *)T.c_str() , Deli);
   while( token != NULL )
   {
      /* While there are tokens in "string" */
	   FilterStream.push_back(CCOVTOSTRING( token ));
      /* Get next token: */
      token = strtok( NULL, Deli );
   }
     delete[] token;
   return FilterStream;
}
//���ܣ�����ַ����������ַ�������
//������s��Ч��Deli��Ч
vector<string> Split(char *s,char* Deli)
{
	vector<string> Temp;
	char *token=new char[CHARLEN_MAX];
	token=strtok(s,Deli);
	while( token != NULL )
	{
		/* While there are tokens in "string" */
		Temp.push_back(CCOVTOSTRING( token ));
		/* Get next token: */
		token = strtok( NULL, Deli );
	}
	delete token;
	return Temp;
}
//���ܣ���Exp�е��ַ����ָ�Ϊ����������ѡ������
//����:[out]Link,[out]Choose
//����ֵ:Ϊ���ʾû�д���
bool MySystem::Seperate(vector<string>Exp,vector<LinkCondi> &Link,vector<string> &Choose)
{
	char *Deli=".=";//�пո�
	string T;
	for(int i=0;i<Exp.size();)
	{
		if(Exp[i]!="and"&&Exp[i]!="or")
		{
			T=Exp[i];
			vector<string> Temp=Split((char *)T.c_str(),Deli);
			if(Temp.size()==4 )//��������
			{
				if(Temp[1]==Temp[3])
				{
					if(Exp.size() != (i+1)&&"and"!=Exp[i+1])
					{
						cout<<"����������ӦΪand���ӷ���"<<endl;
						return false;
					}
				  Link.push_back(LinkCondi(Temp[0],Temp[2],Temp[1]));
				  i++;
				}
				else
				{
					cout<<"������������ƥ��"<<endl;
					return false;
				}
			}
			else
			{
				//ʣ�µķָ��С��4����Ϊѡ����������Ȼ����
				Choose.push_back(Exp[i++]);
				while (i<Exp.size())
				{
					T=Exp[i];
					Temp=Split((char *)T.c_str(),Deli);
					if(Temp.size()<4)
						Choose.push_back(Exp[i++]);
					else
					{
						cout<<Exp[i]<<"Ϊ��������,Ӧ��ѡ������֮ǰ"<<endl;
						return false;
					}
				}
				break;
			}
		}
		else
			i++;
	}
	//for (int i = 0; i < Link.size(); i++)
	//{
	//	cout<<Link[i].Table1<<" "<<Link[i].Table2<<" "<<Link[i].Field<<endl;
	//}
	//for(int i=0;i<Choose.size();i++)
	//{
	//	cout<<Choose[i]<<endl;
	//}
 //  cout<<"here"<<endl;
   	return true;
}
//���ܣ�ʹ���û����������½
void MySystem::LogOn()
{
	ifstream  fin;
	string User,Password;
    string CUser,CPassword;
	bool suc=false;
	while (1)
	{
		cout<<"User: ";
		cin>>User;
		cout<<"Password: ";
		cin>>Password;
		fin.open("User.txt");
		while(fin>>CUser>>CPassword)
		{
			if (CUser==User &&CPassword==Password)
			{
				cout<<"��½�ɹ�"<<endl;
				suc=true;
				break;
			}
		}
		fin.close();
		if (true==suc)
		{
			break;
		}
		else
			cout<<"�û������������"<<endl;
 	}
	getchar();
}
//���ܣ������
//��������Ҫ��
 void PrintVectorTree(vector<pair<string,int> > Tree)
 {
	 for (int i=0;i<Tree.size();i++)
	 {
		 cout<<std::left<<setw(STRINGWIDTH)<<i<<setw(STRINGWIDTH)<<Tree[i].first<<setw(STRINGWIDTH)<<Tree[i].second<<endl;
	 }
 }
 //����:��ʼ��ϵ��
 //������Table.size()>=3,Field����*,���������ܴ��ڱ���,��expstream������������ѡ����������ȷ
 void InitilizeTree(vector<pair<string,int> > &Tree, vector<string> Table, vector<string> expstream, vector<string> Field)
 {
	 Tree.push_back( make_pair(Table[0],2) );
	 Tree.push_back( make_pair(Table[1],2) );
	 int index=2;
	 for (int i=2;i<Table.size();i++)
	 {
		 index+=2;
		 Tree.push_back( make_pair("X",index));
		 Tree.push_back( make_pair(Table[i],index));
	 }
	 Tree.push_back(make_pair("X",++index));
	 string T;
	 for(int i=0;i<expstream.size();i++)
		 T+=expstream[i]+" ";
	 Tree.push_back(make_pair(T,++index));
	 T="";
	 for(int i=0;i<Field.size();i++)
	 {
		 T+=Field[i];
		 if(i+1<Field.size())
			 T+=",";
	 }
	 Tree.push_back(make_pair(T,-1));
 }
 //���ܣ��ҵ���ĸ��ڵ���±�ֵ
 //������pTree��pFather1��pFather2��Ч��
 //����ֵ����out��pFather1����out��pFather2����������ڣ�����-1
 void FindFather(vector<pair<string,int> >* pTree, string Table1,string Table2,int *pFather1, int *pFather2)
 {
	 *pFather1=*pFather2=-1;
	 for (int j=0;j<pTree->size();j++)
	 {
		 if (pTree->at(j).first==Table1)
		 {
			 *pFather1=pTree->at(j).second;
		 }
		 else if(pTree->at(j).first==Table2)
		 {
			 *pFather2=pTree->at(j).second;
		 }
	 }
 }
 //���ܣ��ҵ���n��Ԫ�صĵ����������㿪ʼ����
 //����������С����Ϊn+1��pTree��Ϊ��
  vector<pair<string,int> >::iterator FindInsertPos(vector<pair<string,int> >*pTree,int n)
 {
	 int i=0;
	 for (vector<pair<string,int> >::iterator p=pTree->begin();p!=pTree->end();++p,++i)
	 {
		 if(i==n)
			 return p;
	 }
	 return pTree->end();
 }
 //���ܣ�//������������
 //������Tree��Ϊ�գ���Link�еı������ĸ��ڵ���
 void DownLink(vector<pair<string,int> >*pTree,vector<LinkCondi> Link,vector<string> Choose)
 {
	 for (int i = 0; i < Link.size(); i++)
	 {
		 int Father1,Father2;
		 FindFather(pTree,Link[i].Table1,Link[i].Table2,&Father1,&Father2);
		 if (-1==Father1||-1==Father2)
		 {
			 cout<<"������"<<endl;
			 return;
		 }
		 while(Father1!=Father2)//�ҵ���С�������ڵ�
		 {
			 if (Father1<Father2)
				 Father1=pTree->at( Father1).second;//���丸�ڵ�
			 else
				 Father2=pTree->at( Father2).second;//���丸�ڵ�
		 }
//		 pTree->at(Father1).second=Father1+1;
//		 PrintVectorTree(*pTree);
		 for (int j=Father1;j<pTree->size()-1;j++)
			 pTree->at(j).second++;//���������һ���ڵ㣬��father���������нڵ�ĸ��ڵ㶼����(���˸��ڵ�)		 
		 //cout<<endl;
		 //PrintVectorTree(*pTree);
		 //cout<<endl;
		 pair<string,int> T;
		 T.first=Link[i].Table1+"."+Link[i].Field+"="+Link[i].Table2+"."+Link[i].Field;
		 T.second=pTree->at(Father1).second;		 
		 pTree->insert( FindInsertPos(pTree, Father1+1 ),T); 
		 pTree->at(Father1).second=Father1+1; //�޸���С���ڵ�ĸ��ڵ�
	//	 PrintVectorTree(*pTree);
	 }
	 //��ѡ������
	 string T;
	 for (int i=0;i<Choose.size();i++)
	 {
		 T+=Choose[i]+" ";
	 }
	 pTree->at( pTree->size()-2).first=T;
	 //cout<<endl;
	 //PrintVectorTree(*pTree);
	 //cout<<endl;
 }
 //���ܣ�//ѡ����������
 //������pTree��Ϊ�գ���Link�еı������ĸ��ڵ���
 void DownChoose(vector<pair<string,int> >*pTree,vector<expression> expstream)
 {
//	 PrintVector(Choose,'\n');
	 int n;//��ʾ��Ӧ�ı��λ��
	 for(int i=0;i<expstream.size();i++)
	 {
		 n=-1;
		 for(int j=0;j<pTree->size();j++)
			if( expstream[i].left.find(pTree->at(j).first)!=string::npos)
			{
				n=j;
				break;
			}
		if(-1==n)
		{
			cout<<"��񲻴���"<<endl;
			return;
		}
		for (int j=n;j<pTree->size()-1;j++)
			pTree->at(j).second++;//���������һ���ڵ㣬��father���������нڵ�ĸ��ڵ㶼����(���˸��ڵ�)		 
		//cout<<endl;
		//PrintVectorTree(*pTree);
		//cout<<endl;
		pair<string,int> T;
		T.first=expstream[i].left+expstream[i].op+expstream[i].right;
		T.second=pTree->at(n).second;		 
		pTree->insert( FindInsertPos(pTree, n+1 ),T); 
		pTree->at(n).second=n+1; //�޸���С���ڵ�ĸ��ڵ�
	 }
    pTree->erase(pTree->end()-2);//ɾ��ѡ������
 }
 //���ܣ�//�ѿ�������ѡ���Ϊ����
 //������pTree��Ϊ��
 void ConvertToLink(vector<pair<string,int> >*pTree)
 {
	 vector<pair<string,int> >::iterator p;
	 int i;
	 while(1)
	 {	 
	    i=0;
		 for(p=pTree->begin();p!=pTree->end()-1;++p,++i)
		 {
			 if ("X"==pTree->at(i).first &&pTree->at( pTree->at(i).second ).first.find("=")!=string::npos) //���Ӻ͵ѿ�����
			 {
				 break;
			 }
			 //	 cout<<"dddd "<<p->first<<" "<<p->second<<endl;
		 }
		 if(p!=pTree->end()-1)
		 {
			 for (vector<pair<string,int> >::iterator q=p+1;q!=pTree->end()-1;++q)
			 {
				 q->second--;
//				 cout<<"--   "<<q->first<<" "<<q->second<<endl;
			 }
			 pTree->erase(p);
//			 PrintVectorTree(*pTree);
		 }
		 else
			 break;
	 }
 }
//���ܣ���ѯ�Ż�
//����:Table.size()>=3,Field����*,���������ܴ��ڱ���,��expstream������������ѡ����������ȷ
void MySystem::QueryOptimize(vector<string> Table,vector<string> Field,vector<string> expstream)
{
	 vector<pair<string,int> > Tree;
	 InitilizeTree(Tree, Table, expstream, Field);
     cout<<endl<<"�������:"<<endl<<endl;
	 PrintVectorTree(Tree);
	 vector<LinkCondi> Link;
	 vector<string> Choose;
	 Seperate(expstream,Link,Choose);
	 DownLink(&Tree,Link,Choose);//������������
	 cout<<endl<<"�����������ƺ����:"<<endl<<endl;
	 PrintVectorTree(Tree);
	 vector<expression> onlyexpstream= SeperateOp(Choose);//�ָ�ѡ�������Ĳ�������������,����
	 DownChoose(&Tree,onlyexpstream);//ѡ����������
	 cout<<endl<<"ѡ����������:"<<endl<<endl;
	 PrintVectorTree(Tree);
	 ConvertToLink(&Tree);
	 cout<<endl<<"���յ���:"<<endl<<endl;
	 PrintVectorTree(Tree);
	//PrintVector(Table);
	//PrintVector(Field);
	//for (int i = 0; i < Link.size(); i++)
	//{
	//	cout<<Link[i].Table1<<" "<<Link[i].Table2<<" "<<Link[i].Field<<endl;
	//}

}
