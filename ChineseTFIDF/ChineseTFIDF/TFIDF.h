#include "iostream"
#include "vector"
#include "map"
#include "string"
#include "fstream"
#include <io.h> 
#include <cstring> 
using namespace std;
#define separate "/ "
class TFIDF
{
public:
	TFIDF();
	~TFIDF();
//	map<string, float> getTFMap();
//	map<string, float> getTFMap();

	void getWordMap(const char* inputfile);
	void getDocList(string, const char* outputfile);
	void getTFData(const char* inputfile);
	void getIDFData(const char* inputfile);
	void loadIDFMap(const char* inputfile);
	void saveIDFMap(const char* outputfile);
	void getKeyWords(const char* inputfile, int n=6);
private:
	map<string, int> wordmap;
	map<string, float> tfmap;
	map<string, float>idfmap;
	int MaxVal;	  //输入的map表的数据个数，不含重复
	int WordNum;  //输入数据的最大数目，包含重复元素
	int DocNum;	  //逆文档的数目

	//const char* inputfile = "../data/segresult.txt";
	const char* docbase; 
	const char* doclist; 
	const char* idfmapfile ; 

};