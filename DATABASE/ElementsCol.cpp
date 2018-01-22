#include <iostream>
#include <string>
#include "ElementsCol.h"
using namespace std;
void ElementsCol::PrintElements()
{

	for(int i=0;i<m_line;i++)
	{
		for(int j=0;j<m_column;j++)
			cout<<std::left<<setw(STRINGWIDTH)<<m_Elements[i][j];
		cout<<endl;
	}
}
void ElementsCol::WriteElementsToFile(ofstream &out)
{
	if(out.is_open())
	{
		for(int i=0;i<m_line;i++)
		{
			for(int j=0;j<m_column;j++)
				out<<setw(STRINGWIDTH)<<m_Elements[i][j]<<" ";
			out<<endl;
		}
	}

}
bool ElementsCol::isKeyChong(vector<string> XiaBiao,vector<string> value)
{	
	int k;
	for( k=0;k<XiaBiao.size();k++)
		if(XiaBiao[k]=="0")
			break;
	if(k<XiaBiao.size()&&m_column>0)//至少有一列属性
	{
		for(int i=0;i<m_line;i++)
			if(m_Elements[i][0]==value[k])
			{
				cout<<"已有相同主键元素存在"<<endl;
				return true;
			}
	}
	return false;
}
bool ElementsCol::Insert(vector<string> XiaBiao,vector<string> value)//向xiabiao存放的下标插入value对应的值，类型已经匹配过了
{	
//	cout<<m_line<<"列："<<m_column<<endl;
	if(XiaBiao.size()==0 ) 
	{
		vector<string> XiaBiao;XiaBiao.push_back("0");
		if(isKeyChong(XiaBiao,value)) return false;
		for(int i=0;i<value.size();i++) 
				m_Elements[m_line][i]=value[i];
		m_line++;
	}
	else
	{
		if(isKeyChong(XiaBiao,value)) return false;
	    for(int i=0;i<m_column;i++)
		{
			int j;
			for( j=0;j<XiaBiao.size();j++)
			{
				if(atoi(XiaBiao[j].c_str())==i)
				{					
						break;
				}
			}
			if(j>=XiaBiao.size()) m_Elements[m_line][i]="NULL";
			else m_Elements[m_line][i]=value[j];
		}
		m_line++;
	}
	 return true;
}
/*bool ElementsCol::InsertAccordingField(vector<unsigned int> flags,vector<string> value)//Field和value的对应个数应该相等，没有指明的属性用NULL赋值
{
	if(flags.size()==value.size() )
	{
		 if(m_line<MaxLine)
		 {
			 int pos;
			 if(!isEleExist(value[0],pos))
			 {
				 int j=0;//记录flags被写入
				 for(int i=0;i<m_column;i++)
				 {
					 if(j<flags.size() && flags[j]==i)
					 {
						 m_Elements[m_line][i]=value[j];
						 j++;
					 }
					 else 
						 m_Elements[m_line][i]="NULL";
				 }				     
				 m_line++;
			 }
			 else 
			{
					 cout<<"已存在相同的主键元素"<<endl;
					 return false;
			 }
		 }
		 else
		 {
			 cout<<"已达到最大值，无法插入"<<endl;
			 return false;
		 }
	}
	else 
	{
		cout<<"插入元素时参数不合法"<<endl;
		return false;
	}
	return true;
}*/
bool ElementsCol::isEleExist(string EleName,int &pos)//判断元组是否存在
{
	for(int i=0;i<m_line;i++) //表至少有一个属性，可以没有元组
			if(m_Elements[i][0]==EleName)
			{
				pos=i;
				return true;
			}
    return false;
}
bool ElementsCol::AddNewField()
{
	if(m_column>=MaxColumn) return false;
	for(int i=0;i<m_line;i++)
		m_Elements[i][m_column]="NULL";
	m_column++;
	return true;
}
void ElementsCol::DropField( vector<string> Xiabiao) //必须保证Xiabiao中存放的都是需要删除的列的下标
{
	for(int k=0;k<Xiabiao.size();k++)
	{
		int pos=atoi( Xiabiao[k].c_str() );
		for(int i=pos+1;i<m_column;i++)
		{
			for(int j=0;j<m_line;j++)
				m_Elements[j][i-1]=m_Elements[j][i];
		}
	}
	m_column-=Xiabiao.size();	
}

vector<expression> SeperateOp(vector<string> Exp)//将拆分后的表达式变成  （ 左操作数 op 右操作数 and|or ）   类型
{
	vector<expression> expstream;
//	int rof=-1;
	for(int i=0;i<Exp.size();)
	{
//		if(rof==i)
//			cout<<i<<endl;
//		rof=i;
		string oneexp=Exp[i],t;
		expression exp_t;
		t="";
		int j=0;
		while(j<oneexp.size()&&( isalpha(oneexp[j]) || isdigit(oneexp[j])||'.'==oneexp[j] )  )
			t+=oneexp[j++];
		exp_t.left=t;//
		t="";
		while(j<oneexp.size()&& !isalpha(oneexp[j])&&!isdigit(oneexp[j])&&!('.'==oneexp[j]) )//不是字母和数字
			t+=oneexp[j++];
		exp_t.op=t;
		t="";
		while(j<oneexp.size())
			t+=oneexp[j++];
		if(t.empty())
		{
			exp_t.right=Exp[++i];
			++i;
			if( i<Exp.size()&& ("and"==Exp[i]||"or"==Exp[i]) ) {exp_t.logop=Exp[i++];} //不是最后一个时，每个表达式后面都有一个and 或or
			else 
			{
				expstream.push_back(exp_t);
				break;
			}
		}
		else 
		{	
			exp_t.right=t;
			if( Exp.size()-1==i)//最后一个
			{
				expstream.push_back(exp_t);
				break;
			}
			else {exp_t.logop=Exp[++i];++i;} 
			
		}
		expstream.push_back(exp_t);
	}
	return expstream;
}
bool ElementsCol::DeleteEle(vector<string> Exp,list<FILEDPTYPE_TYPE> FieldType)
{
	vector<expression> expstream=SeperateOp(Exp);
/*	for(int i=0;i<expstream.size();i++)
		cout<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl;
	cout<<endl;
*/    if(DetectFieldType(Exp,FieldType))//检测属性名的正确性，右操作数的类型是否匹配,并将Exp中的所有属性名改为下标，方便处理
	{
		vector<int> xiabiaolist;
		for(int i=0;i<m_line;i++)
		{
			if(isFit(m_Elements[i],m_column,expstream,FieldType))
				xiabiaolist.push_back(i);
		}

		for(int j=0;j<xiabiaolist.size();j++)
		{
//				cout<<"删除行下标 "<<xiabiaolist[j]<<endl;
				DeleteLine(xiabiaolist[j]);
		}
		m_line-=xiabiaolist.size();
	}
	else
	{
		cout<<"属性名不存在或右操作数不匹配"<<endl;
		return false;
	}
	return true;
}
bool ElementsCol::isFit(string *a,int n,vector<expression> expstream,list<FILEDPTYPE_TYPE> FieldType)//判断是否符合表达式,
{
	if(a)
	{
		int i=0;
//		cout<<"元素"<<a[0]<<endl;
		while(i<expstream.size())
		{
			if(isOneConditionFit(a,n,expstream[i],FieldType) )
			{
				if("or"==expstream[i].logop||""==expstream[i].logop)
				{
//					cout<<"成功 "<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl;
					return true;
				}
				else i++;
			}				
			else
			{
				if(""==expstream[i].logop)   
				{
//					cout<<"失败 "<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl; 
					return false;//没有条件表达式了
				}
				else
				{
					while( i<expstream.size() && expstream[i].logop!="or")
						 i++;
					if(i>=expstream.size()) 
					{
//					    cout<<"没有其他的or"<<endl;
							return false; //找到末尾都没有
					}
					else i++;
				}
			}
				
		}
	}
	return true;
}
void ElementsCol::DeleteLine(int n)//删除第n行，从第0行开始算
{
	if(n>=0&&n<m_line)
	{
		for(int i=n+1;i<m_line;i++)
			for(int j=0;j<m_column;j++)
			m_Elements[i-1][j]=m_Elements[i][j];
	}
	else 
		cout<<"删除行时参数有误"<<endl;
	
}
bool ElementsCol::isOneConditionFit(string *line,int n,expression exp,list<FILEDPTYPE_TYPE> FieldType)//判断是否符合单一的条件表达式
{
	int xiabiao=0;
	for(list<FILEDPTYPE_TYPE>::iterator p=FieldType.begin();p!=FieldType.end();++p)
	{
		if(exp.left==p->first)
			break;
		xiabiao++;
	}
	if("="==exp.op) //由于在存储都是字符串，所以右操作数是整形的也不需要转换
	{
		if(xiabiao<n)
			return line[xiabiao]==exp.right;
	}
	else if("!="==exp.op)
	{
		if(xiabiao<n)
			return line[xiabiao]!=exp.right;
	}
	else if(">="==exp.op)
	{
		if(xiabiao<n)
			return atoi(line[xiabiao].c_str())>=atoi(exp.right.c_str());
	}
	else if("<="==exp.op)
	{
		if(xiabiao<n)
			return atoi(line[xiabiao].c_str())<=atoi(exp.right.c_str());
	}
	else if(">"==exp.op)
	{
		if(xiabiao<n)
			return atoi(line[xiabiao].c_str())>atoi(exp.right.c_str());
	}
	else if("<"==exp.op)
	{
		if(xiabiao<n)
			return atoi(line[xiabiao].c_str())<atoi(exp.right.c_str());
	}
	return false;
}

bool ElementsCol::isOneConditionFit(string *line,expression exp)
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
bool ElementsCol::isFit(string *line,vector<expression> xiabiaostream)
{
		int i=0;
//		cout<<"元素"<<a[0]<<endl;
		while(i<xiabiaostream.size())
		{
			if(isOneConditionFit(line,xiabiaostream[i]) )
			{
				if("or"==xiabiaostream[i].logop||""==xiabiaostream[i].logop)
				{
//					cout<<"成功 "<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl;
					return true;
				}
				else i++;
			}				
			else
			{
				if(""==xiabiaostream[i].logop)   
				{
//					cout<<"失败 "<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl; 
					return false;//没有条件表达式了
				}
				else
				{
					while( i<xiabiaostream.size() && xiabiaostream[i].logop!="or")
						 i++;
					if(i>=xiabiaostream.size()) 
					{
//					    cout<<"没有其他的or"<<endl;
							return false; //找到末尾都没有
					}
					else i++;
				}
			}				
		}
	return true;
}
bool ElementsCol::update(vector< pair<string,string> > XiaBiaoValue,vector<expression> xiabiaoexpstream) //此时属性名已经转换为对应的下标
{
	if(xiabiaoexpstream.size()==0)//修改全部元组
	{
		for(int i=0;i<XiaBiaoValue.size();i++)
			for(int j=0;j<m_line;j++)
			{
				m_Elements[j][ atoi(XiaBiaoValue[i].first.c_str()) ]=XiaBiaoValue[i].second;
			}
	}
	else
	{
		 for(int i=0;i<m_line;i++)
		{
			if(isFit(m_Elements[i],xiabiaoexpstream))
				for(int j=0;j<XiaBiaoValue.size();j++)
					m_Elements[i][ atoi(XiaBiaoValue[j].first.c_str()) ]=XiaBiaoValue[j].second;
		}

	}
	return true;
}


static bool ToXiaBiao(vector<pair<string,string>> FieldValue,list<FILEDPTYPE_TYPE> FieldType)
{
	for(int i=0;i<FieldValue.size();i++)
	{
		int j=0;
		list<FILEDPTYPE_TYPE>::iterator p;
		for(p=FieldType.begin();p!=FieldType.end();++p)
		{
			if(p->first==FieldValue[i].first)
			{
					FieldValue[i].first=j;
					break;
			}
			j++;
		}
		if(p==FieldType.end()) return false;
	}
	return true;
}
bool  ElementsCol::DetectFieldType(vector<string> Exp,list<FILEDPTYPE_TYPE> FieldType)
{
	return true;
}

//功能:输出满足条件的元组(所有属性)
//参数:属性值已为下标值
void ElementsCol::PrintPartEle(vector<expression> xiabiaoexpstream)
{
	for(int i=0;i<m_line;i++)
		if(isFit(m_Elements[i],xiabiaoexpstream))
        {
			for(int j=0;j<m_column;j++)
                cout<<std::left<<setw(STRINGWIDTH)<<m_Elements[i][j];
			cout<<endl;
		}	
}
//功能：输出所有元组的部分属性
//参数:属性值必须已是下标值
void ElementsCol::PrintPartField(vector<string>  xiabiaoField)
{
	for(int i=0;i<m_line;i++)
	{
		for (int j=0;j<xiabiaoField.size();j++)
		{
			cout<<std::left<<setw(STRINGWIDTH)<<m_Elements[i][ atoi(xiabiaoField[j].c_str()) ];
		}
		cout<<endl;
	}
}
//功能：输出满足条件元组的部分属性
//参数:属性值必须已转换为下标值
void ElementsCol::PrintPartEleField(vector<expression> xiabiaoexpstream,vector<string>  xiabiaoField)
{
	for(int i=0;i<m_line;i++)
		if(isFit(m_Elements[i],xiabiaoexpstream))
		{
			for(int j=0;j<xiabiaoField.size();j++)
				cout<<std::left<<setw(STRINGWIDTH)<<m_Elements[i][ atoi(xiabiaoField[j].c_str())  ];
			cout<<endl;
		}	
}
