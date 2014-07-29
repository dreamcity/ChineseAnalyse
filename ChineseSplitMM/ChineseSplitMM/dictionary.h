#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <hash_map>
#include <unordered_map>
using namespace std;

class CDictionary
{
public:
	CDictionary();
	~CDictionary();
	CDictionary(const char* inputfile);	//将词典文件读入并构造为一个哈希词典
	int findWord(string str);//在哈希词典中查找词

private:
	string strtmp;	//读取词典的每一行
	string word;	//保存每个词
	hash_map<string, int> wordhash;	// 用于读取词典后的哈希
	hash_map<string, int>::iterator worditer;
	typedef pair<string, int> sipair;
};