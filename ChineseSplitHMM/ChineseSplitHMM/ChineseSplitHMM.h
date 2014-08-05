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
//对输入的测试文本进行处理，得到按照标点符号分隔之后的句子集合
//每一句话，单独写入一行
void getSplitSegFile(const char* inputfile, const char* outputfile)
{
	ifstream infile;
	infile.open(inputfile);
	ofstream outfile;
	outfile.open(outputfile);
	string strtmp;		//每一个待分割的字符串
	string str1 = "";	//存储分割之后的字符串
	string str2 = "";	//存储未完成的句子的后半部分
	//今天是个好天气，我们
	//去游泳，好不好
	//strtmp = 今天是个好天气，我们
	//str1 = 今天是个好天气，
	//str2 = 我们
	//strtmp = 去游泳，好不好
	//str1 = 我们 + 去游泳，
	//str2 = 好不好
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
				//为英文字符，索引加1，继续索引
				if (ch0 < 128)
				{
					i++;
				}
				else
				{
					unsigned char ch1 = strtmp[i+1];
					//当索引到的字符为，。；：！-?""时，结束索引
					if ((ch0 ==163 && ch1==161)||(ch0 ==161 && ch1==163)||(ch0 ==163 && ch1==187)
						||(ch0 ==163 && ch1==172)||(ch0 ==163 && ch1==191)
						||(ch0 ==163 && ch1==186)||(ch0 ==161 && ch1==176)||(ch0 ==161 && ch1==177))
					{
						index = i;
						break;
					}
					//为其他的中文字符，索引加2，继续索引
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
			//如果结束索引时，字符串中，没用相应的标点，直接整个字符串赋值给str2
			if (index == -1)
			{
				str2 = strtmp.substr(0, len);
				strtmp = "";
			}
			// 将字符串strtmp按索引号分隔（将标点号包含），同时加上str2，即上一个字符串的末尾
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

//加载最初由训练时的观察集生成的map表
// map表的每一行，由一个中文字符和对应的map表标号
void loadmapdata(const char* inputfile, map<string, int>& mm)
{
	 ifstream infile1;
	 infile1.open(inputfile);
	 string linetmp;
	 int linenum=0;
	 // 统计输入文件的行数
	 while(getline(infile1, linetmp, '\n'))
	 {
		if (linetmp.empty())
		{
			continue ;
		}
	 	linenum++;
	 }
	 infile1.close();
	 //载入新的map表
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
//inputfile1 为map数据表
//inputfile2 为测试的数据文件
//  inputfile2其实是由最初的输入文件，按标点符号重新提取为句子之后的文件
//	因此，在调用之前，需调用getsegdatafile()函数
//outputfile 为测试数据文件，每一行生成的观察序列
//  其中，上一行为序列长度，下面一行为序列在map表中的编号
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
// 根据viterbi算法得到的输出状态序列（编号），转变成对应的S,B,M,E标记序列
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
//将输入文件的每一行压入vector中
//输入文件为按标点符号分隔之后的句子
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
//根据viterbi算法得到的输出状态序列（编号），对输入的测试文件进行标记
// getVStrSegResult
//		path:viterbi算法得到的状态标记序列
//		vector<string>& vstr ，按标点符号重新提取为句子之后的文件的字符串集合
//		str:标记之后的字符串
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
		//考虑到，分词结果不理想的时候的处理方法，ungly
		//当句子以BM结尾时
		//即忽略句子最后一个字的状态，直接作为结尾。
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
		string strtmp = vstr[0];//得到vector<string>中的第一个字符串
		vector<string>::iterator v_it;
		v_it = vstr.begin();
		vstr.erase(v_it);//删除vector<string>中的第一个字符串
		string strout = "";
		getSplitStr( path, strtmp, strout);
		str= strout;
	}
	return ;
}
//end********************************************************************

//begin******************************************************************
//统计文件中记录的行数
//inputfile：观测序列集合的文件
//num：统计多少个观测序列
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
//通过读取观测序列集合的文件，将序列的数目与长度分别存入数组中
//inpufile:观测序列集合的文件
// int* T: 存入每一个序列的长度
// int**O,行，序列的个数；列，序列的长度
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
