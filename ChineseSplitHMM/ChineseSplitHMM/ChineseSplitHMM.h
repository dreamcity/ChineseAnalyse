#include "iostream"
#include <fstream>
#include <vector>
#include "HMMmodel.h"
#include "ChineseSplit.h"
#include "map"
#ifndef Separator
#define Separator "/ "
#endif

using namespace std;
//������Ĳ����ı����д����õ����ձ����ŷָ�֮��ľ��Ӽ���
//ÿһ�仰������д��һ��
void getSplitSegFile(const char* inputfile, const char* outputfile)
{
	ifstream infile;
	infile.open(inputfile);
	ofstream outfile;
	outfile.open(outputfile);
	string strtmp;		//ÿһ�����ָ���ַ���
	string str1 = "";	//�洢�ָ�֮����ַ���
	string str2 = "";	//�洢δ��ɵľ��ӵĺ�벿��
	//�����Ǹ�������������
	//ȥ��Ӿ���ò���
	//strtmp = �����Ǹ�������������
	//str1 = �����Ǹ���������
	//str2 = ����
	//strtmp = ȥ��Ӿ���ò���
	//str1 = ���� + ȥ��Ӿ��
	//str2 = �ò���
	while(getline(infile, strtmp, '\n'))
	{
		if(strtmp.empty())
		{
			continue;
		}
		while(!strtmp.empty())
		{
			int len = strtmp.length();
			int index = -1;
			for (int i = 0; i < len; )
			{
				unsigned char ch0 = strtmp[i];
				//ΪӢ���ַ���������1����������
				if (ch0 < 128)
				{
					i++;
				}
				else
				{
					unsigned char ch1 = strtmp[i+1];
					//�����������ַ�Ϊ����������-?""ʱ����������
					if ((ch0 ==163 && ch1==161)||(ch0 ==161 && ch1==163)||(ch0 ==163 && ch1==187)
						||(ch0 ==163 && ch1==172)||(ch0 ==163 && ch1==191)
						||(ch0 ==163 && ch1==186)||(ch0 ==161 && ch1==176)||(ch0 ==161 && ch1==177))
					{
						index = i;
						break;
					}
					//Ϊ�����������ַ���������2����������
					else
					{
						unsigned char ch3 = strtmp[i+2];
						unsigned char ch4 = strtmp[i+3];
						if ((ch0 ==161 && ch1==170)&&(ch3 ==161 && ch4 ==170))
						{
							int j =i;
							while(ch3 ==161 && ch4 ==170)
							{
								j+=2;
								ch3 = strtmp[j+2];
								ch4 = strtmp[j+3];
							}
							index = j;
							break;
						}
						i+=2;
					}
				}
			}
			//�����������ʱ���ַ����У�û����Ӧ�ı�㣬ֱ�������ַ�����ֵ��str2
			if (index == -1)
			{
				str2 = strtmp.substr(0, len);
				strtmp = "";
			}
			// ���ַ���strtmp�������ŷָ��������Ű�������ͬʱ����str2������һ���ַ�����ĩβ
			else
			{
				str1 = str2 + strtmp.substr(0, index+2);
				str2 = "";
				strtmp =strtmp.substr(index+2, len-index-2);
			}

			if (str2.empty())
			{
				outfile << str1 <<endl;
				
			}
		}
	}
	outfile << str2 <<endl;
	return ;
}

//���������ѵ��ʱ�Ĺ۲켯���ɵ�map��
// map���ÿһ�У���һ�������ַ��Ͷ�Ӧ��map����
void loadmapdata(const char* inputfile, map<string, int>& mm)
{
	 ifstream infile1;
	 infile1.open(inputfile);
	 string linetmp;
	 int linenum=0;
	 // ͳ�������ļ�������
	 while(getline(infile1, linetmp, '\n'))
	 {
		if (linetmp.empty())
		{
			continue ;
		}
	 	linenum++;
	 }
	 infile1.close();
	 //�����µ�map��
	 ifstream infile2;
	 infile2.open(inputfile);
	 string line, strtmp;
	 for (int i = 0; i < linenum; ++i)
	 {
	 	infile2 >> line;
	 	infile2 >>strtmp;
		int num = atoi(strtmp.c_str());
	 	mm.insert(pair<string,int>(line,num));
	 }
	 infile2.close();

}
//inputfile1 Ϊmap���ݱ�
//inputfile2 Ϊ���Ե������ļ�
//  inputfile2��ʵ��������������ļ�����������������ȡΪ����֮����ļ�
//	��ˣ��ڵ���֮ǰ�������getsegdatafile()����
//outputfile Ϊ���������ļ���ÿһ�����ɵĹ۲�����
//  ���У���һ��Ϊ���г��ȣ�����һ��Ϊ������map���еı��
void getOSequence(const char* inputfile1,const char* inputfile2, const char* outputfile)
{
	map<string, int> mdata;
	loadmapdata(inputfile1, mdata);
	int T = 0;
	int* OS;
	int num = 0;
	string strtmp;
	string str;
	ifstream infile;
	infile.open(inputfile2);
	ofstream outfile;
	outfile.open(outputfile);
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		int len = strtmp.length();
		T = len/2;
		OS = new int [T];
		for (int i = 0; i < T; ++i)
		{
			OS[i] = 0;
		}
		int j=0;
		for (int i = 0; i < len-1;i+=2)
		{
			str = strtmp.substr(i,2);
			j = i/2;
			int n;
			n = mdata[str];
			OS[j] = mdata[str];
		}
		outfile << T <<endl;
		for (int i = 0; i < T; ++i)
		{
			outfile << OS[i] <<" ";
		}
		outfile <<endl;
	}
	outfile.close();
	infile.close();
	//*********************************************************************
}
// ����viterbi�㷨�õ������״̬���У���ţ���ת��ɶ�Ӧ��S,B,M,E�������
void outputpath(int T, int* path)
{
	char* state = new char [T];
	map<int, char> mic;
	mic[0] = 'S';
	mic[1] = 'B';
	mic[2] = 'M';
	mic[3] = 'E';
	for(int i =0; i<T ; i++)
	{
		state[i] = mic[path[i]];
	}
	cout <<"the output state is :"<<endl;
	for(int i =0; i<T-1 ; i++)
	{
		cout<<state[i]<<"->";
	}
	cout<<state[T-1]<<endl;
	return ;
}

//begin******************************************************************
//�������ļ���ÿһ��ѹ��vector��
//�����ļ�Ϊ�������ŷָ�֮��ľ���
void file2Vector(const char* inputfile, vector<string>& vstr)
{	
	ifstream fin;
	string strtmp;
	fin.open(inputfile);
	while(getline(fin, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		vstr.push_back(strtmp);
	}
	return ;
}
//����viterbi�㷨�õ������״̬���У���ţ���������Ĳ����ļ����б��
// getVStrSegResult
//		path:viterbi�㷨�õ���״̬�������
//		vector<string>& vstr ����������������ȡΪ����֮����ļ����ַ�������
//		str:���֮����ַ���
//   ***getsplitstr()
void getSplitStr(int* path, string str1, string& str2)
{
	int i =-1;
	int j =0;
	int m ;
	int L = str1.length();
	while(!str1.empty())
	{
		int len = str1.length();
		switch(path[++i])
		{
			case 0:
				str2 += str1.substr(0,2) + Separator;
				str1 = str1.substr(2,len-2);
				break;
			case 1: j = i; break;
			case 2:  break;
			case 3:
				m = (i-j+1)*2;
				str2 += str1.substr(0,m) + Separator;
				str1 = str1.substr(m,len-m);
				break;
			default : cout<<"ERROR"<<endl; break;
		}
		//���ǵ����ִʽ���������ʱ��Ĵ�������ungly
		//��������BM��βʱ
		//�����Ծ������һ���ֵ�״̬��ֱ����Ϊ��β��
		if (i == L/2 -2)
		{
			str2 += str1 + Separator;
			str1 = "";
		}
		//cout<<"str1len:"<<str1.length()<<endl;
	}
	return ;
}
void getVStrSegResult(int* path, vector<string>& vstr, string& str)
{
	if(!vstr.empty())
	{
		string strtmp = vstr[0];//�õ�vector<string>�еĵ�һ���ַ���
		vector<string>::iterator v_it;
		v_it = vstr.begin();
		vstr.erase(v_it);//ɾ��vector<string>�еĵ�һ���ַ���
		string strout = "";
		getSplitStr( path, strtmp, strout);
		str= strout;
	}
	return ;
}
//end********************************************************************

//begin******************************************************************
//ͳ���ļ��м�¼������
//inputfile���۲����м��ϵ��ļ�
//num��ͳ�ƶ��ٸ��۲�����
void getlinenum(const char* inputfile, int& num)
{
	string strtmp;
	ifstream fin;
	fin.open(inputfile);
	int numline=0;
	while(getline(fin, strtmp, '\n'))
	{
		numline++;
	}
	fin.close();
	num = (int)numline/2;
	return ;
}
//ͨ����ȡ�۲����м��ϵ��ļ��������е���Ŀ�볤�ȷֱ����������
//inpufile:�۲����м��ϵ��ļ�
// int* T: ����ÿһ�����еĳ���
// int**O,�У����еĸ������У����еĳ���
void getOSdata(const char* inputfile, int num ,int* T, int** O)
{
	ifstream fin;
	fin.open(inputfile);
	for (int i = 0; i < num; ++i)
	{
			
		fin >> T[i];
		O[i] = new int [T[i]];
		for (int j = 0; j < T[i]; ++j)
		{
			fin >> O[i][j];
		}
	}
	fin.close();
	return ;
}
//end***********************************************************************
