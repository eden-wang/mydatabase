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
		EraseEndFlag(command);//去除只有一个分号占据一行而多了一个回车符的情况
	EraseEndFlag(t);
	command+=t;

}
static void InitREGEXP(int flag,string &REGEXP) ;//通过不同的标志初始化REGEXP，REGEXP代表正则表达式字符串
bool MySystem::AnalysisCommand(int &flag)//分析命令， flag[out]为对应的命令的宏标志 ;返回值为真,表明命令正确
{
	string REGEXP;
	CRegexpT <char> regexp;
	MatchResult result ;
	for(int i=CREATETABLE;i<=REVOKE;i++)
	{
		InitREGEXP(i,REGEXP); //通过不同的标志初始化REGEXP，REGEXP代表正则表达式字符串	
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
void MySystem::DealWithCommand(int flag)//根据宏标志处理命令
{
    
	switch (flag)
	{
	case CREATETABLE:
		CreateTable();
		WriteToFile();//每执行完一条语句，都更新文件数据库
		break;
	case DROPTABLE:
		DropTable();
		WriteToFile();//每执行完一条语句，都更新文件数据库
		break;
	case ALTERTABLE_ADD:
		AlterTableAdd();
		WriteToFile();//每执行完一条语句，都更新文件数据库
		break;
	case ALTERTABLE_DROP:
		AlterTableDrop();
		WriteToFile();//每执行完一条语句，都更新文件数据库
		break;
	case INSERT:
		Insert();
		WriteToFile();//每执行完一条语句，都更新文件数据库
		break;
	case MYDELETE:
		myDelete();
		WriteToFile();//每执行完一条语句，都更新文件数据库
		break;
	case UPDATE:
		Update();
		WriteToFile();//每执行完一条语句，都更新文件数据库
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

static void InitREGEXP(int flag,string &REGEXP) //通过不同的标志初始化REGEXP，REGEXP代表正则表达式字符串
{
	REGEXP=BEGINPOS	+MIDDLELEFT; //开始
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
	REGEXP+=ENDPOS+MIDDLERIGHT; //结束
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
static void GetTablePlusField(vector<string> FilterStream,string &tablename,list <FILEDPTYPE_TYPE> &FieldPlusType)//初始化表名和属性名列表
{
	//调用此函数前必须确保create table 命令是正确的,否则可能会出现未定义的行为
	if( (FilterStream.at(0)=="create" )&& (FilterStream.at(1)=="table"))
	{
		tablename=FilterStream.at(2);
		pair<string,string> FieldType; 
		for(unsigned int i=3;i<FilterStream.size();)
		{
			FieldType.first=FilterStream.at(i++);
			if(FilterStream.at(i)=="char")  //char 后一定有数字
			{
				FieldType.second=FilterStream.at(++i);//数字
				i++;//下一个属性
			}
			else FieldType.second=FilterStream.at(i++);
			FieldPlusType.push_back(FieldType);
		}
	}
 //   PrintVector_Pair(FieldPlusType);
}
void MySystem::CreateTable()//创建表格
{
	list <FILEDPTYPE_TYPE> FieldPlusType;
	string tablename;
	vector<string> FilterStream=SpliteCommand();
//   PrintVector(FilterStream);
   GetTablePlusField(FilterStream,tablename,FieldPlusType);//初始化表名和属性名列表
   if(database.CreatTable(tablename,FieldPlusType))//创建表名是tablename 的，属性名列表存放在FieldPlusType中
	   cout<<"创建表 "<<tablename<<" 成功"<<endl;
   else 
	   cout<<"创建表 "<<tablename<<" 失败"<<endl;

}

void 	MySystem::DropTable()
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
	if(FilterStream[0]=="drop" &&FilterStream[1]=="table")
		tablename=FilterStream[2];
	if(database.DropTable(tablename))
		cout<<"删除表格 "<<tablename<<" 成功"<<endl;
	else 
   		cout<<"删除表格 "<<tablename<<" 失败"<<endl;
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
					cout<<"向表格 "<<tablename<<" 添加属性 "<<Field<<" 成功"<<endl;
			else 
   					 cout<<"向表格 "<<tablename<<" 添加属性 "<<Field<<" 失败"<<endl;
		}
		else 
		{
				cout<<"不是更改属性命令"<<endl;
				return;
		}
	}
	else
			cout<<"添加属性的分割函数失败"<<endl;
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
					cout<<"对表格 "<<tablename<<" 删除属性 "<<" 成功"<<endl;
			else 
   					 cout<<"对表格 "<<tablename<<" 删除属性 "<<" 失败"<<endl;
		}
		else 
		{
				cout<<"不是删除属性命令"<<endl;
				return;
		}
	}
	else
			cout<<"添加属性的分割函数失败"<<endl;
}

void 	MySystem::Insert() //调用此函数命令必须是正确的，否则会出错
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
			i++;//跳过values
			while(i<FilterStream.size())
			{
				value.push_back(FilterStream[i++]);
			}
	}

	if(database.insert(tablename,Field,value))
	    cout<<"插入表格 "<<tablename<<" 成功"<<endl;
    else 
	    cout<<"插入表格 "<<tablename<<" 失败"<<endl;
}

void 	MySystem::myDelete()
{
	string tablename;
	vector<string> FilterStream=SpliteCommand();
	if(FilterStream.size()==3)
	{
		tablename=FilterStream[2];
		if(database.DropAllEle(tablename))
			cout<<"删除 成功"<<endl;
		else 
			cout<<"删除失败"<<endl;
	}
	else if(FilterStream.size()>3)
	{
		tablename=FilterStream[2];
		vector<string> Exp;
		for(int i=4;i<FilterStream.size();i++)
			Exp.push_back(FilterStream[i]);
//		PrintVector(Exp,'\n'); 
		database.MyDelete(tablename,Exp);
		cout<<"删除完成"<<endl;

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
				(FilterStream[i]).erase(FilterStream[i].end()-1); //去除最后一个字符"="
				FieldValue.push_back( make_pair( FilterStream[i],FilterStream[i+1]));
				i+=2;
			}
//			PrintVector_Pair(FieldValue);
			vector<string> Exp;
			if(i<FilterStream.size())//存在where
			{
				i++;//跳过where				
				for(int j=i;j<FilterStream.size();j++)
				    	Exp.push_back(FilterStream[j]);
			}
//			PrintVector(Exp);
			if(database.update(tablename,FieldValue,SeperateOp(Exp)))
			    cout<<"完成更新"<<endl;
			else
				cout<<"更新失败"<<endl;
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
		if(Table.size()==1) //单表
			database.Select(Field,Table,SeperateOp(Exp));
		else if(Table.size()==2)//两个表，为了方便，原本可用于支持2个表及其以上的函数目前用于只支持2个表
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
//	cout<<"索引："<<command<<endl;
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
		cout<<"分割函数分析出错"<<endl;
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
	cout<<"注册完成"<<endl;
}
//缺陷：未进行语义检查；未查重
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
	cout<<"完成"<<endl;
}
//假定存在并且没有语义错误
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
	cout<<"完成 "<<endl;
}
vector<string> MySystem::SpliteCommand()
{	
	vector<string> FilterStream;
	//经过拆分的字符串数组 
	string T=command;
	char *token=new char[CHARLEN_MAX];
	char *Deli=" ,()\'\n";//有空格
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
//功能：拆分字符串，返回字符串数组
//参数：s有效，Deli有效
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
//功能：将Exp中的字符流分割为连接条件和选择条件
//参数:[out]Link,[out]Choose
//返回值:为真表示没有错误
bool MySystem::Seperate(vector<string>Exp,vector<LinkCondi> &Link,vector<string> &Choose)
{
	char *Deli=".=";//有空格
	string T;
	for(int i=0;i<Exp.size();)
	{
		if(Exp[i]!="and"&&Exp[i]!="or")
		{
			T=Exp[i];
			vector<string> Temp=Split((char *)T.c_str(),Deli);
			if(Temp.size()==4 )//连接条件
			{
				if(Temp[1]==Temp[3])
				{
					if(Exp.size() != (i+1)&&"and"!=Exp[i+1])
					{
						cout<<"连接条件后应为and连接符号"<<endl;
						return false;
					}
				  Link.push_back(LinkCondi(Temp[0],Temp[2],Temp[1]));
				  i++;
				}
				else
				{
					cout<<"连接属性名不匹配"<<endl;
					return false;
				}
			}
			else
			{
				//剩下的分割后小于4个，为选择条件，不然错误
				Choose.push_back(Exp[i++]);
				while (i<Exp.size())
				{
					T=Exp[i];
					Temp=Split((char *)T.c_str(),Deli);
					if(Temp.size()<4)
						Choose.push_back(Exp[i++]);
					else
					{
						cout<<Exp[i]<<"为连接条件,应在选择条件之前"<<endl;
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
//功能：使用用户名和密码登陆
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
				cout<<"登陆成功"<<endl;
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
			cout<<"用户名或密码错误"<<endl;
 	}
	getchar();
}
//功能：输出树
//参数：无要求
 void PrintVectorTree(vector<pair<string,int> > Tree)
 {
	 for (int i=0;i<Tree.size();i++)
	 {
		 cout<<std::left<<setw(STRINGWIDTH)<<i<<setw(STRINGWIDTH)<<Tree[i].first<<setw(STRINGWIDTH)<<Tree[i].second<<endl;
	 }
 }
 //功能:初始关系树
 //参数：Table.size()>=3,Field不含*,所有属性能存在表中,且expstream中连接条件和选择条件都正确
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
 //功能：找到表的父节点的下标值
 //参数：pTree，pFather1，pFather2有效；
 //返回值：【out】pFather1，【out】pFather2；如果表不存在，返回-1
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
 //功能：找到第n个元素的迭代器，从零开始计数
 //参数：树大小至少为n+1；pTree不为空
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
 //功能：//连接条件下移
 //参数：Tree不为空，且Link中的表在树的根节点上
 void DownLink(vector<pair<string,int> >*pTree,vector<LinkCondi> Link,vector<string> Choose)
 {
	 for (int i = 0; i < Link.size(); i++)
	 {
		 int Father1,Father2;
		 FindFather(pTree,Link[i].Table1,Link[i].Table2,&Father1,&Father2);
		 if (-1==Father1||-1==Father2)
		 {
			 cout<<"表不存在"<<endl;
			 return;
		 }
		 while(Father1!=Father2)//找到最小公共父节点
		 {
			 if (Father1<Father2)
				 Father1=pTree->at( Father1).second;//找其父节点
			 else
				 Father2=pTree->at( Father2).second;//找其父节点
		 }
//		 pTree->at(Father1).second=Father1+1;
//		 PrintVectorTree(*pTree);
		 for (int j=Father1;j<pTree->size()-1;j++)
			 pTree->at(j).second++;//由于添加了一个节点，故father及其后的所有节点的父节点都后移(除了根节点)		 
		 //cout<<endl;
		 //PrintVectorTree(*pTree);
		 //cout<<endl;
		 pair<string,int> T;
		 T.first=Link[i].Table1+"."+Link[i].Field+"="+Link[i].Table2+"."+Link[i].Field;
		 T.second=pTree->at(Father1).second;		 
		 pTree->insert( FindInsertPos(pTree, Father1+1 ),T); 
		 pTree->at(Father1).second=Father1+1; //修改最小父节点的父节点
	//	 PrintVectorTree(*pTree);
	 }
	 //改选择条件
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
 //功能：//选择条件下移
 //参数：pTree不为空，且Link中的表在树的根节点上
 void DownChoose(vector<pair<string,int> >*pTree,vector<expression> expstream)
 {
//	 PrintVector(Choose,'\n');
	 int n;//表示对应的表的位置
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
			cout<<"表格不存在"<<endl;
			return;
		}
		for (int j=n;j<pTree->size()-1;j++)
			pTree->at(j).second++;//由于添加了一个节点，故father及其后的所有节点的父节点都后移(除了根节点)		 
		//cout<<endl;
		//PrintVectorTree(*pTree);
		//cout<<endl;
		pair<string,int> T;
		T.first=expstream[i].left+expstream[i].op+expstream[i].right;
		T.second=pTree->at(n).second;		 
		pTree->insert( FindInsertPos(pTree, n+1 ),T); 
		pTree->at(n).second=n+1; //修改最小父节点的父节点
	 }
    pTree->erase(pTree->end()-2);//删除选择条件
 }
 //功能：//笛卡尔积和选择变为连接
 //参数：pTree不为空
 void ConvertToLink(vector<pair<string,int> >*pTree)
 {
	 vector<pair<string,int> >::iterator p;
	 int i;
	 while(1)
	 {	 
	    i=0;
		 for(p=pTree->begin();p!=pTree->end()-1;++p,++i)
		 {
			 if ("X"==pTree->at(i).first &&pTree->at( pTree->at(i).second ).first.find("=")!=string::npos) //连接和笛卡尔积
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
//功能：查询优化
//参数:Table.size()>=3,Field不含*,所有属性能存在表中,且expstream中连接条件和选择条件都正确
void MySystem::QueryOptimize(vector<string> Table,vector<string> Field,vector<string> expstream)
{
	 vector<pair<string,int> > Tree;
	 InitilizeTree(Tree, Table, expstream, Field);
     cout<<endl<<"最初的树:"<<endl<<endl;
	 PrintVectorTree(Tree);
	 vector<LinkCondi> Link;
	 vector<string> Choose;
	 Seperate(expstream,Link,Choose);
	 DownLink(&Tree,Link,Choose);//连接条件下移
	 cout<<endl<<"连接条件下移后的树:"<<endl<<endl;
	 PrintVectorTree(Tree);
	 vector<expression> onlyexpstream= SeperateOp(Choose);//分割选择条件的操作数，操作符,常量
	 DownChoose(&Tree,onlyexpstream);//选择条件下移
	 cout<<endl<<"选择条件下移:"<<endl<<endl;
	 PrintVectorTree(Tree);
	 ConvertToLink(&Tree);
	 cout<<endl<<"最终的树:"<<endl<<endl;
	 PrintVectorTree(Tree);
	//PrintVector(Table);
	//PrintVector(Field);
	//for (int i = 0; i < Link.size(); i++)
	//{
	//	cout<<Link[i].Table1<<" "<<Link[i].Table2<<" "<<Link[i].Field<<endl;
	//}

}
