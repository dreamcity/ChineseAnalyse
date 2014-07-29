// ChineseSplitHMM.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "iostream"
#include <fstream>
#include "HMMmodel.h"
#include "ChineseSplit.h"
#include "map"
#ifndef Separator
#define Separator "/ "
#endif
using namespace std;
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

void getsplitstr(int* path, string str1, string& str2)
{

	int i =0;
	int j;
	int m ;
	while(!str1.empty())
	{
		int len = str1.length();
		//cout<<"len:"<<len<<endl;
		switch(path[i++])
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
		//cout<<"str1len:"<<str1.length()<<endl;
	}
	return ;
}
void getsegresult(int* path, const char* inputfile, const char* outputfile)
{
	ifstream fstr;
	fstr.open(inputfile);
	ofstream resultfile;
	resultfile.open(outputfile);
	string strtmp;
	
	while(getline(fstr, strtmp,'\n'))
	{
		if (strtmp.empty())
		{
			continue;
		}
		string strout = "";
		getsplitstr( path, strtmp, strout);
		resultfile<<strout<<endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	const char* databasefile = "../data/databasefile.txt";
	const char* markfile = "../tempdata/markfile.txt";
	const char* statefile = "../tempdata/statefile.txt";
	const char* HMMModel = "../data/hmmmodel.txt";
	const char* MAPData = "../data/mapdata.txt";
	const char* datatestfile = "../iodata/datatestfile.txt";
	const char* Osequence = "../iodata/osequence.txt";
	const char* segresult = "../iodata/segresult.txt";
	//�ж�map�ļ���hmmmodel�Ƿ���ڣ������ڣ��򴴽������ڣ���ֱ�Ӽ���
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

	getOSequence(MAPData, datatestfile, Osequence);
	HMM hm;
	hm.initialData(HMMModel, Osequence);
	int T = hm.getT(); 
	int* path = new int [T];
	for(int i =0; i<T ; i++)
	{
		path[i] = 0;
	}
	hm.viterbi(path);
	outputpath(T, path);
	getsegresult(path, datatestfile, segresult);
	return 0;
}
