// demo1.cpp : 定义控制台应用程序的入口点。
//

#include "BaumWelch.h"
#include "iostream"

using namespace std;
int main(int argc, char const *argv[])
{
	int M,N;
	BaumWelch bw ;
	const char* inputfile = "1nput.txt";
	bw.run(inputfile);
	//bw.run();
	double* Pi = bw.getPi();
	double** TMatrix = bw.getTMatrix();
	double** CMatrix = bw.getCMatrix();

	M = bw.getM();
	N = bw.getN();

	cout<< "The PMatrix Data:" <<endl;
	for (int i = 0; i < M; ++i)
	{
		cout<<Pi[i]<<" ";
	}
	cout<<endl;
	cout << "The TMatrix Data: " <<endl;
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			cout<<TMatrix[i][j]<<"  ";
		}
		cout<<endl;
	}
	cout << "The CMatrix Data:" <<endl;
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cout<<CMatrix[i][j]<<"  ";
		}
		cout<<endl;
	}
	return 0;
}