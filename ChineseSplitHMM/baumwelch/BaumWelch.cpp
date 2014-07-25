#include "BaumWelch.h"
using namespace std;
BaumWelch::BaumWelch()
{
}

BaumWelch::~BaumWelch()
{
}

int BaumWelch::getM()
{
	return M;
}
int BaumWelch::getN()
{
	return N;
}
double* BaumWelch::getPi()
{
	return Pi;
}

double** BaumWelch::getTMatrix()
{
	return TMatrix;
}

double** BaumWelch::getCMatrix()
{
	return CMatrix;
}

// inputfile 格式
// 隐藏状态数 M
// 可观察状态数 N
// 初始概率矩阵 Pi M*1
// 转移概率矩阵 TMatrix M*M
// 混淆概率矩阵 CMatrix M*N
void BaumWelch::initParameters(const char* inputfile)
{
	// alpha T*M 向前变量
	// beta T*M 向后变量
	// xi T*M*M 为t时刻处于状态i，t+1时刻处于状态j的概率。
	// xi[t](i,j) = P(T=Si,T+1=Sj|O,M) M为模型
	// gama[t][i] = sum(xi[t][i][j]);
	//			   = (alpha[t][i]*beta[t][i])/sum(alpha[t][i]*beta[t][i])

	ifstream fin;
	fin.open(inputfile);
	if (!fin.is_open())
	{
		cout<<"can not open inputfile"<<endl;
		return ;
	}
	//赋值
	fin >> M >> N;
	Pi = new double [M];
	TMatrix = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		TMatrix[i] = new double [M];
	}
	CMatrix = new double *[M];
	for (int i = 0; i < M; ++i)
	{
		CMatrix[i] = new double [N];
	}
	

	//赋值
	//fin >> M >> N;
	for (int i = 0; i < M; ++i)
	{
		fin >> Pi[i];
		//cout<<Pi[i]<<endl;
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			fin >> TMatrix[i][j];
		}
	}
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			fin>> CMatrix[i][j];
			//cout<<" CMatrix[i][j]"<< CMatrix[i][j]<<endl;
		}
	}
	fin >> T;

	alpha = new double* [T];
	for (int i = 0; i < T; ++i)
	{
		alpha[i] = new double [M];
	}
	beta = new double* [T];
	for (int i = 0; i < T; ++i)
	{
		beta[i] = new double [M];
	}
	//beta赋初值为0
	for (int i = 0; i < T; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			beta[i][j] = 0;
		}
	}

	xi = new double** [T -1];
	for (int t = 0; t < T -1; ++t)
	{
		xi[t] = new double * [M];
		for (int i = 0; i < M; ++i)
		{
			xi[t][i] = new double [M];
		}
	}
	gamma = new double* [T];
	for (int i = 0; i < T; ++i)
	{
		gamma[i] = new double [M];
	}
	O = new int [T];
	for (int i = 0; i < T; ++i)
	{
		fin >> O[i];
	}
	// O为观测序列
	// tmp为观测序列中，将重复元素赋值为-1；
	// AC为剔除O中的重复元素之后的集合，即所有状态集合

	//这里不能直接 tmp = O,
	//因为这是指针的赋值，地址的传递，对tmp的改变将直接影响O
	int* tmp = new int [T];
	for( int i = 0; i < T; i++)
	{
		tmp[i] = O[i];
	}
	
	AC = new int [N];
	for (int i = 0; i < T -1; ++i)
	{
		for (int j = i+1; j < T; ++j)
		{
			if(tmp[i] == tmp[j])
			{
				tmp[j] = -1;
			}
		}
	}
	int t1 =0, t2 = 0;
	for (t2; t2 < T; ++t2)
	{
		if (tmp[t2] == -1)
		{
			continue ;
		}
		AC[t1] = tmp [t2];
		t1++;
	}
	//S = new int [T];
	fin.close();
}
void BaumWelch::updateAlpha()
{
	//初始化
	//alpha[1](i) = pi(i)*b(i)(O1);
	for (int i = 0; i < M; ++i)
	{
		alpha[0][i] = Pi[i] * CMatrix[i][O[0]];
	}
	//递归求解alpha(t+1)
	//alpha[t+1][j] = sum(a[t][i]aij)*b[j][O[t+1]]
	for (int t = 0; t < T-1; ++t)
	{
		for (int i = 0; i < M; ++i)
		{
			double sum = 0.0;
			for (int j = 0; j < M; ++j)
			{
				//为什么是TMatrix[j][i]，
				//因为是计算从其他状态转移到i的结果求和
				//		t 		t+1
				//		S1
				//		S2		S2
				//		S3
				sum += alpha[t][j]*TMatrix[j][i];
			}
			alpha[t+1][i] = sum * CMatrix[i][O[t+1]];
		}
	}
	// cout<<"it will output the alpha data"<<endl;
	// for (int i = 0; i < T; i++)
	// {
	// 	for (int j = 0; j < M ; j++)
	// 	{
	// 		cout<< alpha[i][j]<<"  " ;
	// 	}
	// 	cout<<endl;
	// }
}
void BaumWelch::updateBeta() 
{
	//初始化
	//beta[T](i) = 1;
	for (int i = 0; i < M; ++i)
	{
		beta[T-1][i] = 1;
	}
	//递归计算
	//beta[t][i] = sum(a[i][j]*b[j][O[t+1]]*beta[t+1][j])
 	for (int t = T-2; t >=0; --t)
	{
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				beta[t][i] += TMatrix[i][j] * CMatrix[j][O[t+1]] * beta[t+1][j];
			}
		}
	}
	// cout<<"it will output the beta data"<<endl;
	// for (int i=0; i<T; i++)
	// {
	// 	for (int j = 0; j <M ; j++)
	// 	{
	// 		cout<< beta[i][j]<<"  " ;
	// 	}
	// 	cout<<endl;
	// }
}
void BaumWelch::updateXi()
{
	for (int t = 0; t < T - 1 ; ++t)
	{
		double frac = 0.0;
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				frac += alpha[t][i] * TMatrix[i][j] * CMatrix[j][O[t+1]] * beta[t+1][j];
			}
		}
		double dem = 0.0;
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				dem = alpha[t][i] * TMatrix[i][j] * CMatrix[j][O[t+1]] * beta[t+1][j];
				xi[t][i][j] = dem / frac;
			}
		}
	}
}
void BaumWelch::updateGamma()
{
	for (int t = 0; t < T ; ++t)
	{
		double frac = 0.0;
		for (int i = 0; i < M; ++i)
		{
			frac += alpha[t][i] * beta[t][i];
		}
		for (int i = 0; i < M; ++i)
		{
			gamma[t][i] = alpha[t][i] * beta[t][i] / frac;
		}
	}
	// cout<<"it will output gama"<<endl;
	// for (int i = 0; i < T; ++i)
	// {
	// 	for (int j = 0; j < M; ++j)
	// 	{
	// 		cout<<gamma[i][j] <<"  ";
	// 	}
	// 	cout<<endl;
	// }
	
}
// 更新初始状态概率矩阵
void BaumWelch::updatePi()
{
	for (int i = 0; i < M; ++i)
	{
		Pi[i] = gamma[0][i];
	}
	// cout<<"it will output pi"<<endl;
	// for (int i = 0; i < M; ++i)
	// {
	// 	cout<<Pi[i]<<"  "; 
	// }
	// cout<<endl;
}
//更新转移概率矩阵
//等于从状态i转移到状态j的期望 除以 从所有状态i转移出去的期望
//Tmatrix[i][j] = sum(xi[t-1][i][j])/sum(gamma[t-1][i])
void BaumWelch::updateTMatrix() 
{
	for (int i = 0; i < M; ++i)
	{	
		double frac = 0.0;
		for (int t = 0; t < T-1; ++t)
		{
			frac += gamma[t][i];
		}
		for (int j = 0; j < M; ++j)
		{
			double dem = 0.0;
			for (int t = 0; t < T-1; ++t)
			{
				dem += xi[t][i][j];
			}
			TMatrix[i][j] = dem / frac;			
		}
	}
	// cout << " it will output TMatrix" <<endl;
	// for (int i = 0; i < M; ++i)
	// {
	// 	for (int j = 0; j < M; ++j)
	// 	{
	// 		cout<<TMatrix[i][j]<<"  ";
	// 	}
	// 	cout<<endl;
	// }
}
//更新混淆矩阵
//等于在状态j下观察到活动为K的次数的期望值 除以 从其他所有状态转移到状态j的次数的期望值
//CMatrix[i][j] = sum(gamma[t][j](条件：O[t] = V[n]))/sum(gamma[t][j])
//V[N] 为活动数组
void BaumWelch::updateCMatrix() 
{
	for (int i = 0; i < M; ++i)
	{
		double frac = 0.0;
		for (int t = 0; t < T; ++t)
		{
			frac += gamma[t][i];
		}
		for (int j = 0; j < N; ++j)
		{
			double dem = 0.0;
			for (int t = 0; t < T; ++t)
			{
				if (O[t] == AC[j])
				{
					dem += gamma[t][i];	
				}
				CMatrix[i][j] = dem / frac;
			}
		}
	}
	// cout << " it will output CMatrix" <<endl;
	// for (int i = 0; i < M; ++i)
	// {
	// 	for (int j = 0; j < N; ++j)
	// 	{
	// 		cout<<CMatrix[i][j]<<"  ";
	// 	}
	// 	cout<<endl;
	// }
}
void BaumWelch::run(const char* inputfile)
{
		initParameters(inputfile);
        int iter = 8; // 迭代次数
        while (iter-- > 0)
		{
            // E-Step
            updateAlpha();
            // updatePO();
            updateBeta();
            updateGamma();
            updatePi();
            updateXi();
            // M-Step
            updateTMatrix();
            updateCMatrix();
        }
}
