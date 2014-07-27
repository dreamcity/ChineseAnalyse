// Dictionary.h
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <hash_map>
#include <unordered_map>

//#include <ext/hash_map>
//using namespace __gnu_cxx;

using namespace std;
using namespace stdext;

class CDictionary
{
public:
	CDictionary(); //���ʵ��ļ����벢����Ϊһ����ϣ�ʵ�
	~CDictionary();
	int FindWord(string w); //�ڹ�ϣ�ʵ��в��Ҵ�

private:
	string strtmp; //��ȡ�ʵ��ÿһ��
	string word; //����ÿ����
	hash_map<string, int> wordhash; // ���ڶ�ȡ�ʵ��Ĺ�ϣ
	hash_map<string, int >::iterator worditer; //
	typedef pair<string, int> sipair;
};



