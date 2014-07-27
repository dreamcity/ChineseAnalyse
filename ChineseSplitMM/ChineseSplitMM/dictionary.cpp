#include "dictionary.h"
using namespace std;

//将词典文件读入并构造为一个哈希词典
CDictionary::CDictionary()
{
	ifstream infile("dictionarytest.dict"); // 打开词典
	//ifstream infile("dict.xdb"); // 打开词典
 	if (!infile.is_open()) // 打开词典失败则退出程序
	{
	cerr << "Unable to open input file: " << "wordlexicon"
	<< " -- bailing out!" << endl;
	exit(0);
	}
	while (getline(infile, strtmp, '\n')) // 读入词典的每一行并将其添加入哈希中
	{
		istringstream istr(strtmp);		
		istr >> word; //读入每行第一个词
		wordhash.insert(sipair(word, 1)); //插入到哈希中
	}
}

CDictionary::~CDictionary()
{
}

//在哈希词典中查找词，若找到，则返回，否则返回
int CDictionary::FindWord(string w)
{
	if (wordhash.find(w) != wordhash.end())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}