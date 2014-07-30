#include "iostream"
#include "string"
#include "fstream"
#include "vector"
#include <map>

using namespace std;
class ChineseSplit
{
public:
	ChineseSplit();
	~ChineseSplit();
	void initialModel();
	map<string , int > getMapData();
	//outputfile 为观测样本集添加标注
	// eg 今B天E是S星B期M一E
	// outputfile1 保存观测样本集的标注
	// eg BESBME
	void getSplitSegFile(const char* inputfile, const char* outputfile);
	void getMarkSentenceFile(const char* inputfile, const char* outputfile1, const char* outputfile2);
	void countFre(int* count, double* markfre, int m);
	//void getHMMModelFile(const char* inputfile, const char* outputfile);
	void get2DTMatrix(double* markfre);
	void getInitMatrix(const char* inputfile);
	void getTranMatrix(const char* inputfile);
	void getConMatrix(const char* inputfile);
	void saveHMMModel(const char* outputfile);
	void saveMapData(const char* outputfile);
private:
	int M;	//隐藏的状态数目
	int N;	//可观察的行为数目
	double* Pi;	// 初始化概率矩阵 M*1
	double** TMatrix;	// 转移概率矩阵 M*M
	double** CMatrix;	// 混合概率矩阵 M*N,表示为在某一个状态可能的行为
	map<string , int > MapData;
		
};
