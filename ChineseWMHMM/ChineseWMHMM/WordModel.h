#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include "map"
#include <regex>

using namespace std;
class WordModel
{
public:
	WordModel();
	~WordModel();
	map<string , int> getStateMap();
	map<string , int> getWordMap();
	void filePreprocess(const char* inputfile, const char* outputfile);	
	// void getMapData(const char* inputfile, map<string, int>& statemap, 
	// 			map<string,	int>& wordemap,	const char* outputfile);
	void getMapData(const char* inputfile, const char* outputfile);

	void initialModel(); 
	void getIniPro(const char* inputfile);
	void getTranPro(const char* inputfile);
	void getConPro(const char* inputfile);
	// void getIniPro(const char* inputfile, map<string, int> statemap);
	// void getTranPro(const char* inputfile, map<string, int> statemap);
	// void getConPro(const char* inputfile, map<string, int> wordmap, 
	// 				map<string, int>& wordemap);
	void WordModel::saveMapData(const char* outputfile, map<string, int> MapData);
	void saveModelData(const char* outputfile);
private:
	map<string , int> statemap;
	map<string , int> wordmap;
	int M;
	int N;
	double* Pi;
	double** TranMatrix;
	double** ConMatrix; 
	
};
