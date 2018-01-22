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
	if(k<XiaBiao.size()&&m_column>0)//������һ������
	{
		for(int i=0;i<m_line;i++)
			if(m_Elements[i][0]==value[k])
			{
				cout<<"������ͬ����Ԫ�ش���"<<endl;
				return true;
			}
	}
	return false;
}
bool ElementsCol::Insert(vector<string> XiaBiao,vector<string> value)//��xiabiao��ŵ��±����value��Ӧ��ֵ�������Ѿ�ƥ�����
{	
//	cout<<m_line<<"�У�"<<m_column<<endl;
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
/*bool ElementsCol::InsertAccordingField(vector<unsigned int> flags,vector<string> value)//Field��value�Ķ�Ӧ����Ӧ����ȣ�û��ָ����������NULL��ֵ
{
	if(flags.size()==value.size() )
	{
		 if(m_line<MaxLine)
		 {
			 int pos;
			 if(!isEleExist(value[0],pos))
			 {
				 int j=0;//��¼flags��д��
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
					 cout<<"�Ѵ�����ͬ������Ԫ��"<<endl;
					 return false;
			 }
		 }
		 else
		 {
			 cout<<"�Ѵﵽ���ֵ���޷�����"<<endl;
			 return false;
		 }
	}
	else 
	{
		cout<<"����Ԫ��ʱ�������Ϸ�"<<endl;
		return false;
	}
	return true;
}*/
bool ElementsCol::isEleExist(string EleName,int &pos)//�ж�Ԫ���Ƿ����
{
	for(int i=0;i<m_line;i++) //��������һ�����ԣ�����û��Ԫ��
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
void ElementsCol::DropField( vector<string> Xiabiao) //���뱣֤Xiabiao�д�ŵĶ�����Ҫɾ�����е��±�
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

vector<expression> SeperateOp(vector<string> Exp)//����ֺ�ı��ʽ���  �� ������� op �Ҳ����� and|or ��   ����
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
		while(j<oneexp.size()&& !isalpha(oneexp[j])&&!isdigit(oneexp[j])&&!('.'==oneexp[j]) )//������ĸ������
			t+=oneexp[j++];
		exp_t.op=t;
		t="";
		while(j<oneexp.size())
			t+=oneexp[j++];
		if(t.empty())
		{
			exp_t.right=Exp[++i];
			++i;
			if( i<Exp.size()&& ("and"==Exp[i]||"or"==Exp[i]) ) {exp_t.logop=Exp[i++];} //�������һ��ʱ��ÿ�����ʽ���涼��һ��and ��or
			else 
			{
				expstream.push_back(exp_t);
				break;
			}
		}
		else 
		{	
			exp_t.right=t;
			if( Exp.size()-1==i)//���һ��
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
*/    if(DetectFieldType(Exp,FieldType))//�������������ȷ�ԣ��Ҳ������������Ƿ�ƥ��,����Exp�е�������������Ϊ�±꣬���㴦��
	{
		vector<int> xiabiaolist;
		for(int i=0;i<m_line;i++)
		{
			if(isFit(m_Elements[i],m_column,expstream,FieldType))
				xiabiaolist.push_back(i);
		}

		for(int j=0;j<xiabiaolist.size();j++)
		{
//				cout<<"ɾ�����±� "<<xiabiaolist[j]<<endl;
				DeleteLine(xiabiaolist[j]);
		}
		m_line-=xiabiaolist.size();
	}
	else
	{
		cout<<"�����������ڻ��Ҳ�������ƥ��"<<endl;
		return false;
	}
	return true;
}
bool ElementsCol::isFit(string *a,int n,vector<expression> expstream,list<FILEDPTYPE_TYPE> FieldType)//�ж��Ƿ���ϱ��ʽ,
{
	if(a)
	{
		int i=0;
//		cout<<"Ԫ��"<<a[0]<<endl;
		while(i<expstream.size())
		{
			if(isOneConditionFit(a,n,expstream[i],FieldType) )
			{
				if("or"==expstream[i].logop||""==expstream[i].logop)
				{
//					cout<<"�ɹ� "<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl;
					return true;
				}
				else i++;
			}				
			else
			{
				if(""==expstream[i].logop)   
				{
//					cout<<"ʧ�� "<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl; 
					return false;//û���������ʽ��
				}
				else
				{
					while( i<expstream.size() && expstream[i].logop!="or")
						 i++;
					if(i>=expstream.size()) 
					{
//					    cout<<"û��������or"<<endl;
							return false; //�ҵ�ĩβ��û��
					}
					else i++;
				}
			}
				
		}
	}
	return true;
}
void ElementsCol::DeleteLine(int n)//ɾ����n�У��ӵ�0�п�ʼ��
{
	if(n>=0&&n<m_line)
	{
		for(int i=n+1;i<m_line;i++)
			for(int j=0;j<m_column;j++)
			m_Elements[i-1][j]=m_Elements[i][j];
	}
	else 
		cout<<"ɾ����ʱ��������"<<endl;
	
}
bool ElementsCol::isOneConditionFit(string *line,int n,expression exp,list<FILEDPTYPE_TYPE> FieldType)//�ж��Ƿ���ϵ�һ���������ʽ
{
	int xiabiao=0;
	for(list<FILEDPTYPE_TYPE>::iterator p=FieldType.begin();p!=FieldType.end();++p)
	{
		if(exp.left==p->first)
			break;
		xiabiao++;
	}
	if("="==exp.op) //�����ڴ洢�����ַ����������Ҳ����������ε�Ҳ����Ҫת��
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
bool ElementsCol::isFit(string *line,vector<expression> xiabiaostream)
{
		int i=0;
//		cout<<"Ԫ��"<<a[0]<<endl;
		while(i<xiabiaostream.size())
		{
			if(isOneConditionFit(line,xiabiaostream[i]) )
			{
				if("or"==xiabiaostream[i].logop||""==xiabiaostream[i].logop)
				{
//					cout<<"�ɹ� "<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl;
					return true;
				}
				else i++;
			}				
			else
			{
				if(""==xiabiaostream[i].logop)   
				{
//					cout<<"ʧ�� "<<expstream[i].left<<" "<<expstream[i].op<<" "<<expstream[i].right<<" "<<expstream[i].logop<<endl; 
					return false;//û���������ʽ��
				}
				else
				{
					while( i<xiabiaostream.size() && xiabiaostream[i].logop!="or")
						 i++;
					if(i>=xiabiaostream.size()) 
					{
//					    cout<<"û��������or"<<endl;
							return false; //�ҵ�ĩβ��û��
					}
					else i++;
				}
			}				
		}
	return true;
}
bool ElementsCol::update(vector< pair<string,string> > XiaBiaoValue,vector<expression> xiabiaoexpstream) //��ʱ�������Ѿ�ת��Ϊ��Ӧ���±�
{
	if(xiabiaoexpstream.size()==0)//�޸�ȫ��Ԫ��
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

//����:�������������Ԫ��(��������)
//����:����ֵ��Ϊ�±�ֵ
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
//���ܣ��������Ԫ��Ĳ�������
//����:����ֵ���������±�ֵ
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
//���ܣ������������Ԫ��Ĳ�������
//����:����ֵ������ת��Ϊ�±�ֵ
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
