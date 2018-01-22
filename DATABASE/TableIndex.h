#pragma once
#include <string>
#include "IndexFile.h"
#include <vector>
using namespace  std;

struct IndexStr //表索引结构
{
	string TableName; //表名
	string FiledName;//属性名
	string IndexName;//索引名
	 IndexFile *pIndexFile;//指向索引文件
	 IndexStr()
	 {
		 pIndexFile=NULL;
	 }
	 IndexStr(string TableName,string FiledName,string IndexName,IndexFile *p):TableName(TableName),FiledName(FiledName),IndexName(IndexName),pIndexFile(p)
	 {
	 }
};


