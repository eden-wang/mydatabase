#pragma once
#include <vector>
using namespace std;
class IndexFile//�����ļ�
{
public:
	vector< pair<string,int> > IndexFileArray;//������,�ڶ���������Ԫ����±�
	IndexFile(void);
	~IndexFile(void);
	void PrintIndexFile()
	{
		//for(int i=0;i<IndexFileArray.size();i++)
		//{
		//	cout<<IndexFileArray[i].first<<"  "<<IndexFileArray[i].second <<endl;
		//}
	}
};

