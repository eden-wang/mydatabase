#pragma once
#include <string>
#include "IndexFile.h"
#include <vector>
using namespace  std;

struct IndexStr //�������ṹ
{
	string TableName; //����
	string FiledName;//������
	string IndexName;//������
	 IndexFile *pIndexFile;//ָ�������ļ�
	 IndexStr()
	 {
		 pIndexFile=NULL;
	 }
	 IndexStr(string TableName,string FiledName,string IndexName,IndexFile *p):TableName(TableName),FiledName(FiledName),IndexName(IndexName),pIndexFile(p)
	 {
	 }
};


