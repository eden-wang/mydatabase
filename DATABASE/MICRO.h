#pragma once
//命令宏标志
enum {CREATETABLE,DROPTABLE,ALTERTABLE_ADD,ALTERTABLE_DROP,INSERT,MYDELETE,UPDATE,SELECT,CREATEINDEX,DROPINDEX,CREATEUSER,GRANT,REVOKE};
//string和char *的相互转换
#define COVTOCSTRING(P) P.c_str()   //string-》char *
#define CCOVTOSTRING(CCHARARRAY) string(CCHARARRAY)  //将C字符串数组转化为String类型


//与正则表达式有关的宏
//基本宏
#define MATCHLEFT CCOVTOSTRING("\\(") //表示输入的需要匹配的左括号
#define MIDDLELEFT CCOVTOSTRING("\(") //表示在正则表达式中的左括号
#define MATCHRIGHT CCOVTOSTRING("\\)") //表示输入的需要匹配的右括号
#define MIDDLERIGHT CCOVTOSTRING("\)") //表示在正则表达式中的右括号
#define SPACE CCOVTOSTRING("\\x20+")  //至少一个空格
#define EXSPACE CCOVTOSTRING("\\s*")  //可以没有空白符,空白符包括
#define IDENTIFIER CCOVTOSTRING("[a-zA-Z]\\w*")  //以字母开头的标识符
#define NUMBER  CCOVTOSTRING("\([1-9]\\d*|0\)")//整数
#define VARCHAR CCOVTOSTRING("char\\(")+NUMBER+"\\)"  //char(n)类型
#define TYPE CCOVTOSTRING("\(int")+"|"+VARCHAR+"\)"   //支持的类型为int,char(n)
//扩展宏
#define RELATIONNAME IDENTIFIER //关系名
#define WIDEIDENTIFIER  MIDDLELEFT+IDENTIFIER+"."+IDENTIFIER+MIDDLERIGHT  //R.name 2015.6.5添加
#define FIELDNAME  MIDDLELEFT+WIDEIDENTIFIER+"|"+IDENTIFIER+MIDDLERIGHT//属性名 2015.6.5修改为支持R.name和name两种形式 
#define IDENTIFIER_LIST MIDDLELEFT+IDENTIFIER+","+MIDDLERIGHT+"*"+IDENTIFIER  //标识符列表
#define COLUMN_LIST  IDENTIFIER_LIST  //列名
#define FIELDNAME_LIST MIDDLELEFT+FIELDNAME+","+MIDDLERIGHT+"*"+FIELDNAME  //属性列表 2015.6.5添加R.name
#define BEGINPOS CCOVTOSTRING("^")  //起点位置匹配
#define ENDPOS CCOVTOSTRING("$")  //终点位置匹配
#define CONSTVALUE MIDDLELEFT+"\'"+IDENTIFIER+"\'"+"|"+"\'"+NUMBER+"\'" +MIDDLERIGHT//常量
#define CONSTVALUE_LIST  MIDDLELEFT+CONSTVALUE+","+MIDDLERIGHT+"*"+CONSTVALUE  //常量列表
#define LINKCONDITION  FIELDNAME+"="+FIELDNAME //连接条件
#define CONDITIONEXP  MIDDLELEFT+ FIELDNAME+MIDDLELEFT+"="+"|"+"!="+MIDDLERIGHT+CONSTVALUE+"|"+FIELDNAME+MIDDLELEFT+"<|>|<=|>=|!=|="+MIDDLERIGHT+NUMBER+"|"+LINKCONDITION+MIDDLERIGHT //一个条件表达式
#define LOGEXP MIDDLELEFT+CONDITIONEXP+SPACE+"\(and|or\)"+SPACE+MIDDLERIGHT+"*"+CONDITIONEXP//逻辑表达式
#define ASSIGNMENT COLUMN_LIST+"="+CONSTVALUE //将常量赋给列名
#define ASSIGNMENT_LIST  MIDDLELEFT+ASSIGNMENT+","+MIDDLERIGHT+"*"+ASSIGNMENT  //赋值列表
#define  RELATIONNAME_LIST  MIDDLELEFT+RELATIONNAME+","+MIDDLERIGHT+"*"+RELATIONNAME  //关系名列表
#define INDEXNAME IDENTIFIER //索引名
#define USERNAME IDENTIFIER//用户名
#define PASSWORD "\\w+" //密码
#define AUTHORITY  MIDDLELEFT+"insert|delete|update|select"+MIDDLERIGHT //权限
#define AUTHORITY_LIST MIDDLELEFT+AUTHORITY+","+MIDDLERIGHT+"*"+AUTHORITY  //权限列表
#define USERNAME_LIST MIDDLELEFT+USERNAME+","+MIDDLERIGHT+"*"+USERNAME  //用户列表

#define CHARLEN_MAX 100 //char *数组的最大长度
//与表结构有关的宏
#define  ELEMENTMAX 100//表的元组的最大个数    
#define FIELDMAX 50//表的属性的最大个数

#define STRINGWIDTH 15//字符串宽度



