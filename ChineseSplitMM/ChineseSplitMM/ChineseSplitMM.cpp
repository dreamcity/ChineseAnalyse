// ChineseSplitMM.cpp : �������̨Ӧ�ó������ڵ㡣
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
	string strtmp;	//���ڱ�������Ͽ��ж����ÿһ��
	string line;	//�������ÿһ�еĽ��
	ifstream infile;
	infile.open(inputfile);
	if (!infile.is_open())	// �������ļ�ʧ�����˳�����
	{
		cerr << "Unable to open input file: " << inputfile
		<< " -- bailing out!" << endl;
		exit(-1);
	}
	ofstream outfile;
	outfile.open(outputfile);	//ȷ������ļ�
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