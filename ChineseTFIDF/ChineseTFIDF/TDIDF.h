#include "iostream"
#include "vector"
#include "map"
#include "string"
#include "fstream"
#include <io.h> 
#include <cstring> 
using namespace std;
#define separate "/ "
class TDIDF
{
public:
	TDIDF();
	~TDIDF();
	void getWordMap(const char* inputfile);
	void getDocList(string, const char* outputfile);
	void getTFData(const char* inputfile);
	void getIDFData(const char* inputfile);
	void loadIDFMap(const char* inputfile);
	void saveIDFMap(const char* outputfile);
private:
	map<string, int> wordmap;
	map<string, float> tfmap;
	map<string, float>idfmap;
	int MaxVal;	  //�����map������ݸ����������ظ�
	int WordNum;  //�������ݵ������Ŀ�������ظ�Ԫ��
	int DocNum;	  //���ĵ�����Ŀ

};