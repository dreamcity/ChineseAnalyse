#include "iostream"
#include "map"
#include "fstream"
#include "string"
using namespace std;
void PrintIntIntMap(std::map<string,int>& m, string pre) 
{
	std::map<string,int>::iterator it;
	std::cout << pre;
	for ( it = m.begin(); it != m.end(); it++ )
		std::cout << "(" << it->first << "," << it->second << ") ";
	std::cout << std::endl;
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
int main(int argc, char const *argv[])
{
	const char* inputfile = "outputtest.txt";
	string strtmp;
 	ifstream infile(inputfile);
 	std::map<string, int> mm0;
 	std::map<string, int> mm1;
	map<string,int>::iterator m_it;

 	// mm0
	int num=0;
	string t0,t1,tmp;
 	while(getline(infile, strtmp, '\n'))
 	{
 		int length = strtmp.length();
 		for (int i = 4; i < length-1; i+=4)
 		{
 			t0=strtmp.substr(i,3);
 			t1=strtmp.substr(i+3,1);
 			tmp = t0 + t1;
 			m_it = mm0.find(t0);
 			if (m_it == mm0.end())
 			{
 				//cout<<"tmp:"<<t0<<endl;
 				mm0.insert(pair<string,int>(t0,num));
 				num++;
 			}
 			else
 			{
 				continue;
 			}		
 		}
 	}
 	//int val = mm0.size();
 	// int num = 1000;
 	// if (num<val)
 	// {
 	// 	num = 1000;
 	// }
 	int value0[1880]={0};
 	int value1[1880]={0};
 	int value2[1880]={0};
 	int value3[1880]={0};
 	cout<<"val:"<<mm0.size()<<endl;
 	// for (int i = 0; i < ; ++i)
 	//20
 	// 	/* code */
 	// }
 	//PrintIntIntMap(mm0,"插入二元组：");
 	ifstream infile1(inputfile);
 	while(getline(infile1, strtmp, '\n'))
 	{
 		int length = strtmp.length();
 		for (int i = 4; i < length-1;i+=4)
 		{
 			t0=strtmp.substr(i,3);
 			t1=strtmp.substr(i+3,1);
 			tmp= t0+t1;
 			char c = tmp[3];
 			//cout<<"c:"<<c<<endl;
 			int val = mm0[t0];
 			m_it = mm1.find(tmp);
 			if (m_it == mm1.end())
 			{
 				mm1.insert(pair<string, int>(tmp,val));
 				switch(c)
 				{
	 				case 'S':	value0[val]=1; break;
	 				case 'B':	value1[val]=1; break;
	 				case 'M':	value2[val]=1; break;
	 				case 'E':	value3[val]=1; break;
	 				default :   cout<<"Error!!!"<<endl; break;
 				}
 				//cout<<"notfound"<<endl;
 			}
 			else
 			{
 				switch(c)
 				{
	 				case 'S':	value0[val]+=1; break;
	 				case 'B':	value1[val]+=1; break;
	 				case 'M':	value2[val]+=1; break;
	 				case 'E':	value3[val]+=1; break;
	 				default :   cout<<"Error!!!"<<endl; break;
 				}
 			}
 		}
 	}
 	// cout<<"value0=";
 	// for (int i = 0; i < 1880; ++i)
 	// {
 	// 	cout<<value0[i];
 	// }
 	// cout<<endl;
 	//构建二维混淆矩阵
	// 分配空间
	float** CMatrix;
	CMatrix = new float *[4];
	for (int i = 0; i < 4; ++i)
	{
		CMatrix[i] = new float [1880];
	}
	//赋值
	for (int j = 0; j < 1880; ++j)
	{
		CMatrix[0][j]=value0[j];
	}
	for (int j = 0; j < 1880; ++j)
	{
		CMatrix[1][j]=value1[j];
	}
	for (int j = 0; j < 1880; ++j)
	{
		CMatrix[2][j]=value2[j];
	}
	for (int j = 0; j < 1880; ++j)
	{
		CMatrix[3][j]=value3[j];
	}
	//disPlay2D(CMatrix, 4, 1880);

	//计算概率
	int sum[4] = {0.0};
	// float sum=0;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 1880; ++j)
		{
			// sum+=CMatrix[i][j];
			sum[i]+=CMatrix[i][j];
		}
	}
	//cout<<"sum:"<<sum<<endl;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 1880; ++j)
		{
			// CMatrix[i][j] = (float) CMatrix[i][j] / sum ;
			CMatrix[i][j] = (float) CMatrix[i][j] / sum[i] ;
		}
	}
	disPlay2D(CMatrix, 4, 1880);
	return 0;
}