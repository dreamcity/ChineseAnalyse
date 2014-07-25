#include "iostream"
#include "fstream"
#include "BaumWelch.h"
using namespace std;

class HMM :public BaumWelch
// class HMM 
{
public:
	HMM();
	~HMM();
	//int getM();
	//int getN();
	int getT();
	void setData(int t, int* o, int m ,int n, double* pi, double** tm, double** cm );
	void initial(const char* inputfile);
	void forward(double& sum_prob);
	void viterbi(int* path);

private:
	int T;	//观测序列的长度
	int* O;	//观测序列
	int M;	//隐藏的状态数目
	int N;	//可观察的行为数目
	double* Pi;	// 初始化概率矩阵 M*1
	double** TMatrix;	// 转移概率矩阵 M*M
	double** CMatrix;	// 混合概率矩阵 M*N,表示为在某一个状态可能的行为

};