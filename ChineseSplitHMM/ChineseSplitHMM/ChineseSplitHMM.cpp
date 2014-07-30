// ChineseSplitHMM.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
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
void getsegdatafile(const char* inputfile, const char* outputfile)
{
	//const char* infilename = "inputfile.txt";
	ifstream infile;
	infile.open(inputfile);
	ofstream outfile;
	outfile.open(outputfile);
	string strtmp;
	string str1 = "";
	string str2 = "";
	while(getline(infile, strtmp, '\n'))
	{
		//getline(infile, strtmp, '\n');
		while(!strtmp.empty())
		{

			int len = strtmp.length();
			//cout<<"len:"<<len<<endl;
			//cout<<"strtmp:"<<strtmp<<endl;
			int index =0;
			for (int i = 0; i < len; i+=2)
			{
				unsigned char ch = strtmp[i];
				if (ch > 176)
				{
					index+=2;
				}
				else
				{
					break;
				}
			}
			// cout<<"strtmp:"<<strtmp<<endl;
			// cout<<"index:"<<index<<endl;
			if (index == len)
			{
				str2 = strtmp.substr(0, index);
				strtmp = "";
			}
			else
			{
				str1 = str2 + strtmp.substr(0, index+2);
				str2 = "";
				strtmp =strtmp.substr(index+2, len-index-2);
			}
			
			//cout<<"strtmp:"<<strtmp<<endl;
			if (str2.empty())
			{
				int len = str1.length();
				if (len >= 2)
				{
					string tmp = str1.substr(len-2,len);
					unsigned char c =tmp[0];
					if(c < 176)
					{
						outfile<<str1.substr(0, len-2)<<endl;
						outfile<<str1.substr(len-2, 2)<<endl;
					}
					else
					{
						outfile << str1 <<endl;
					}
				}
				//cout<<"str1:"<<str1<<endl;
				
			}
			//cout<<"str2:"<<str2<<endl;
		}
	}
	//str1 = str2；
	//cout<<"str1:"<<str2<<endl;
	outfile << str2 <<endl;
	return ;
}

//加载最初有训练时的观察集生成的map表
void loadmapdata(const char* inputfile, map<string, int>& mm)
{
	 ifstream infile1;
	 infile1.open(inputfile);
	 string linetmp;
	 int linenum=0;
	 while(getline(infile1, linetmp, '\n'))
	 {
		if (linetmp.empty())
		{
			continue ;
		}
	 	linenum++;
	 }
	 infile1.close();

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
	//ChineseSplit cs;
	map<string, int> mdata;
	//mdata = CNS.getMapData();

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
//根据viterbi算法得到的输出状态序列（编号），对输入的测试文件进行标记
// getsegresult()
//		path:viterbi算法得到的状态标记序列
//		inputfile:输入的测试文本tmp（按标点符号重新提取为句子之后的文件）
//		outputfile:标记之后的文件------------------（待修改）
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
		//cout<<"len:"<<len<<endl;
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
//void getSegResult(int* path, const char* inputfile, const char* outputfile, string& str)
//{
//	str = "";
//	ifstream fstr;
//	fstr.open(inputfile);
//	ofstream resultfile;
//	resultfile.open(outputfile);
//	string strtmp;
//	
//	while(getline(fstr, strtmp,'\n'))
//	{
//		if (strtmp.empty())
//		{
//			continue;
//		}
//		string strout = "";
//		getSplitStr( path, strtmp, strout);
//		resultfile<<strout<<endl;
//		str+= strout;
//	}
//	
//}
//end********************************************************************


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
void getVStrSegResult(int* path, vector<string>& vstr, string& str)
{
	if(!vstr.empty())
	{
		string strtmp = vstr[0];
		vector<string>::iterator v_it;
		v_it = vstr.begin();
		vstr.erase(v_it);
		string strout = "";
		getSplitStr( path, strtmp, strout);
		str= strout;
	}
	return ;
}



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
	cout <<"T[i]:"<<T[0]<<endl;
	fin.close();
	cout <<"T[i]:"<<T[0]<<endl;
	return ;
}
//end***********************************************************************

//载入输入的观察序列的长度，与序列，得到输出的path
//根据path，得到标记之后的字符串getsegresult()
// inputfile为输入的HMM模型参数
// num 为待标记的句子数目
//void recstr(const char* inputfile,vector<string>& vstr,int num, int* T, int** O)
//{
//	string strtmp = "";
//	string str = "";
//	vector<string> vstr;
//	file2Vector(datasegfile, vstr);
//	HMM hmm;
//	hmm.initialModel(HMMModel);
//	ofstream fout(segresult);
//
//	for (int i = 0; i < num; ++i)
//	{		
//
//		hmm.setT(T[i]);
//		hmm.setO(O[i]); 
//		int* path = new int [T[i]];
//		for(int j =0; j<T[i] ; j++)
//		{
//			path[j] = 0;
//		}
//		hmm.viterbi(path);
//		outputpath(T[i], path);
//		getVStrSegResult(path, vstr, str);		
//		strtmp += str;
//		fout<<strtmp<<endl;
//	}
//}

//输入观察序列集合
//得到标记字符串
//void getresult(const char* inputfile, string str)
//{
//	//const char* inputfile = "osequence.txt";
//	int num;
//	getlinenum(inputfile,num);
//	//cout<<"num:"<<num<<endl;
//	ifstream fin;
//	fin.open(inputfile);
//	int* T = new int [num];
//	int** O = new int*[num];
//	getOSdata(inputfile, num , T, O);
////	recstr(num, T,O);
//	return ;
//}
int _tmain(int argc, _TCHAR* argv[])
{
	const char* databasefile = "../data/databasefile.txt";
	const char* markfile = "../tempdata/markfile.txt";
	const char* statefile = "../tempdata/statefile.txt";
	const char* HMMModel = "../data/hmmmodel.txt";
	const char* MAPData = "../data/mapdata.txt";
	const char* datatestfile = "../iodata/datatestfile.txt";
	const char* datasegfile = "../iodata/datasegfile.txt";
	const char* Osequence = "../iodata/osequence.txt";
	const char* segresult = "../iodata/segresult.txt";
	//判断map文件与hmmmodel是否存在，不存在，则创建；存在，则直接加载
	ifstream mapfile;
	ifstream modelfile;
	mapfile.open(MAPData);
	modelfile.open(HMMModel);
	if(!mapfile || !modelfile)
	{
		ChineseSplit CNS;
		CNS.getMarkSentenceFile(databasefile, markfile, statefile);
		CNS.initialModel();
		CNS.getInitMatrix(statefile);
		CNS.getTranMatrix(statefile);
		CNS.getConMatrix(markfile);
		CNS.saveMapData(MAPData);
		CNS.saveHMMModel(HMMModel);
	}

	getsegdatafile(datatestfile, datasegfile);
	getOSequence(MAPData,  datasegfile, Osequence);
	//************************
	//准备载入 输入观察序列的长度与编码
	int num;

	getlinenum(Osequence,num);
	//cout<<"num:"<<num<<endl;
	int* T = new int [num];
	int** O = new int* [num];
	getOSdata(Osequence, num , T, O);
	cout <<"T[i]:"<<T[0]<<endl;
	string strtmp = "";
	string str = "";
	vector<string> vstr;
	file2Vector(datasegfile, vstr);
	HMM hmm;
	hmm.initialModel(HMMModel);
	ofstream fout(segresult);

	for (int i = 0; i < num; ++i)
	{		
		cout <<"T[i]:"<<T[0]<<endl;
		hmm.setT(T[i]);
		hmm.setO(O[i]);
		for (int k=0; k<T[i] ; k++)
		{
			cout<<O[i][k]<<endl;
		}
		int* path = new int [T[i]];
		for(int j =0; j<T[i] ; j++)
		{
			path[j] = 0;
		}
		hmm.viterbi(path);
		outputpath(T[i], path);
		getVStrSegResult(path, vstr, str);		
		strtmp += str;
	}
	fout<<strtmp<<endl;

	//**************************
	//HMM hm;
	//hm.initialData(HMMModel, Osequence);
	//int T = hm.getT(); 
	//int* path = new int [T];
	//for(int i =0; i<T ; i++)
	//{
	//	path[i] = 0;
	//}
	//hm.viterbi(path);
	//outputpath(T, path);
//	getsegresult(path, datatestfile, segresult);
	return 0;
}
