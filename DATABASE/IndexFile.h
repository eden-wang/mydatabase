#pragma once
#include <vector>
using namespace std;
class IndexFile//索引文件
{
public:
	vector< pair<string,int> > IndexFileArray;//索引表,第二个参数是元组的下标
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

