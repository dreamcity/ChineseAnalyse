#include "dictionary.h"
using namespace std;

//���ʵ��ļ����벢����Ϊһ����ϣ�ʵ�
CDictionary::CDictionary()
{
	ifstream infile("dictionarytest.dict"); // �򿪴ʵ�
	//ifstream infile("dict.xdb"); // �򿪴ʵ�
 	if (!infile.is_open()) // �򿪴ʵ�ʧ�����˳�����
	{
	cerr << "Unable to open input file: " << "wordlexicon"
	<< " -- bailing out!" << endl;
	exit(0);
	}
	while (getline(infile, strtmp, '\n')) // ����ʵ��ÿһ�в�����������ϣ��
	{
		istringstream istr(strtmp);		
		istr >> word; //����ÿ�е�һ����
		wordhash.insert(sipair(word, 1)); //���뵽��ϣ��
	}
}

CDictionary::~CDictionary()
{
}

//�ڹ�ϣ�ʵ��в��Ҵʣ����ҵ����򷵻أ����򷵻�
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