#include "iostream"
#include "fstream"
using namespace std;
class BaumWelch 
{
public:
	BaumWelch();
	~BaumWelch();
	int getM();
	int getN();
	double* getPi();
	double** getTMatrix();
	double** getCMatrix();

	void initParameters(const char* inputfile);
	void updateAlpha();
	void updateBeta() ;
	void updateXi();
	void updateGamma();
	void updatePi();
	void updateTMatrix();
	void updateCMatrix();
	void run(const char* inputfile);
private:
	int T; 	//序列的长度
	int* O;	//观测序列
	int* AC;// AC为剔除O中的重复元素之后的集合，即所有观测状态集合
	//int* S; //观测序列对应的状态序列
	int M;	//隐藏的状态数目
	int N;	//可观察的行为数目
	double* Pi;	// 初始化概率矩阵 M*1
	double** TMatrix;	// 转移概率矩阵 M*M
	double** CMatrix;	// 混合概率矩阵 M*N,表示为在某一个状态可能的行为
	double** alpha;		// alpha T*M 向前变量
	double** beta;		// beta T*M 向后变量	
	double*** xi;		// xi T*M*M 为t时刻处于状态i，t+1时刻处于状态j的概率。
						// xi[t](i,j) = P(T=Si,T+1=Sj|O,M) M为模型
	double** gamma;		// gama[t][i] = sum(xi[t][i][j]);
						//			   = (alpha[t][i]*beta[t][i])/sum(alpha[t][i]*beta[t][i])
};