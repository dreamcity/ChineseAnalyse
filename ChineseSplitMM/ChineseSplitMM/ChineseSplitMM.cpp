// test.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
//#include "dictionary.h"
#include "mmseg.h"
#include "iostream"
#include "fstream"
#include "cstdlib"
#include "string"
using namespace std;

int main(int argc, char *argv[])
{
	//CDictionary WordDic; //初始化一个词典
	//const char* inputfilename = "../data/wordlexion.utf8";
	const char* inputfilename = "../data/inputtest.txt";
	const char* outputfilename = "../data/SegmentResult.txt";
	string strtmp; //用于保存从语料库中读入的每一行
	string line; //用于输出每一行的结果

	ifstream infile(inputfilename);
	if (!infile.is_open()) // 打开输入文件失败则退出程序
	{
		cout<<"error"<<endl;
		cerr << "Unable to open input file: " << inputfilename
		<< " -- bailing out!" << endl;
		exit(-1);
	}

	ofstream outfile(outputfilename); //确定输出文件
	if (!outfile.is_open())
	{
		cerr << "Unable to open file：SegmentResult.txt"
		<< "--bailing out!" << endl;
		exit(-1);
	}

	while (getline(infile, strtmp, '\n')) //读入语料库中的每一行并用最大匹配法处理
	{
		line = strtmp;
		cout<<"line"<<line<<endl;
		line = SegmentSentenceMM(line); // 调用分词函数进行分词处理
		cout<<"line"<<line<<endl;
		outfile << line << endl; // 将分词结果写入目标文件
	}
	cout<<"wordsegment done!!!"<<endl;
	return 0;
}