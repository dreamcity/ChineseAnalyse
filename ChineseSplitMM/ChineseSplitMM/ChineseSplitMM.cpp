// ChineseSplitMM.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "doublemmseg.h"
#include "iostream"
#include "fstream"
#include "string"
int _tmain(int argc, _TCHAR* argv[])
{
	const char* inputfile = "../data/inputfile.txt";
	const char* outputfile = "../data/segmentresult.txt";
	string strtmp;	//用于保存从语料库中读入的每一行
	string line;	//用于输出每一行的结果
	ifstream infile;
	infile.open(inputfile);
	if (!infile.is_open())	// 打开输入文件失败则退出程序
	{
		cerr << "Unable to open input file: " << inputfile
		<< " -- bailing out!" << endl;
		exit(-1);
	}
	ofstream outfile;
	outfile.open(outputfile);	//确定输出文件
	if (!outfile.is_open())
	{
		cerr << "Unable to open input file: " << outputfile
		<< " -- bailing out!" << endl;
		exit(-1);
	}

	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		splitDoubleMM(strtmp, line);
		outfile << line <<endl;
	}
	outfile.close();
	infile.close();
	cout<<"wordsegment done!!!"<<endl;
	return 0;
}