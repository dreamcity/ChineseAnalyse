#include "iostream"
#include "fstream"
#include "string"
#include "map"
using namespace std;
	// 	 S 	B 	B 	E
	// S
	// B
	// M
	// E

map<string , int >  initiation()
{
	map<string , int >mapstring;
	mapstring["SS"] = 0;
	mapstring["SB"] = 1;
	mapstring["BM"] = 2;
	mapstring["BE"] = 3;
	mapstring["MM"] = 4;
	mapstring["ME"] = 5;
	mapstring["ES"] = 6;
	mapstring["EB"] = 7;
	return mapstring;
}

int *countNum(string& mark, int* count)
{
	//int count[8] = {0};
	//string mark;
	map<string , int > mapstring = initiation() ;
	int val = mapstring[mark];
	switch (val)
		{
			case 0: 
				count[0]+=1; break;
			case 1: 
				count[1]+=1; break;
			case 2:
				count[2]+=1; break;
			case 3:
				count[3]+=1; break;
			case 4: 
				count[4]+=1; break;
			case 5:
				count[5]+=1; break;
			case 6:
				count[6]+=1; break;
			case 7:
				count[7]+=1; break;
			default :cout<<"error"<<endl; break;
		}
	return count;
}

template <class T1, class T2>
void disPlay1D(T1* arr, T2 m)
{
	for (int i = 0; i < m; ++i)
	{
		cout<<"value"<<"["<<i<<"]:"<<arr[i]<<endl;
	}
}
template <class T1, class T2>
void disPlay2D(T1** arr, T2 m, T2 n)
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout<<arr[i][j]<<"     ";
		}
		cout<<endl;
	}
}


float *countFre(int* count, float* markfre, int m)
{
	int sum[4] ={0};
	int j = 0;
	for (int i = 0; i < 4; ++i)
	{
		sum[i] = count[j] + count[j+1];
		j+=2;
	}

	for (int i = 0; i < m; ++i)
	{
		markfre[i] = (float)count[i] / sum[i/2];
		//cout<<"markfre:"<<markfre[i]<<endl;
	}
	//diaPlay1(markfre, 8);
	return markfre;
}
float **get2DM(float* markfre, int m, int n)
{
	float** tranFrenM; 
	tranFrenM = new float *[m];
	for (int i = 0; i < m; ++i)
	{
		tranFrenM[i] = new float [n];
	}
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			tranFrenM[i][j] =0;
		}
	}
	tranFrenM[0][0] = markfre[0];
	tranFrenM[0][1] = markfre[1];
	tranFrenM[1][2] = markfre[2];
	tranFrenM[1][3] = markfre[3];
	tranFrenM[2][2] = markfre[4];
	tranFrenM[2][3] = markfre[5];
	tranFrenM[3][0] = markfre[6];
	tranFrenM[3][1] = markfre[7];
	return tranFrenM;
}
float** getTranFrenMatrix(const char* inputfile)
{
	string strtmp;
	string mark;
	int count[8] = {0}; 
	float fre[8] = {0}; 
	int* countmark;
	float* countfre;
	float** tranFreM;
	ifstream infile(inputfile);
	while(getline(infile, strtmp, '\n'))
	//for (int i = 0; i < 10; ++i)
	{
		//getline(infile, strtmp, '\n');
		int length = strtmp.length() ;
		for (int j = 0; j < length-1; ++j)
		{
			mark = strtmp[j];
			mark += strtmp[j+1];
			//cout<<"mark:"<<mark<<endl;
			countmark = countNum(mark, count);
		}
	}
	countfre = countFre(countmark, fre, 8);
	//disPlay1D(countmark, 8);
	//disPlay1D(countfre, 8);	
	tranFreM = get2DM(countfre, 4, 4);
	disPlay2D(tranFreM,4,4);
	return tranFreM;
}
float* getIniMatrix(const char* inputfile)
{
	string strtmp;
	ifstream infile(inputfile);
	int count[4] = {0};
	float* iniMatrix;
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp[1] == 'S')
		{
			count[0]+=1;
		}
		else
		{
			count[3]+=1;
		}
	}
	int sum = count[0] + count[3];
	iniMatrix[0] = (float)count[0] / sum;
	iniMatrix[1] = (float)count[1] / sum;
	iniMatrix[2] = (float)count[2] / sum;
	iniMatrix[3] = (float)count[3] / sum;
	disPlay1D(iniMatrix, 4);
	return iniMatrix;
}
int main(int argc, char const *argv[])
{
	const char* inputfile = "outputtemp.txt";
	float* iniMatrix;
	iniMatrix = getIniMatrix(inputfile);
	float** tranFreMatrix;
	tranFreMatrix = getTranFrenMatrix(inputfile);
	return 0;
}