// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//CDictionary WordDic; //��ʼ��һ���ʵ�
	//const char* inputfilename = "../data/wordlexion.utf8";
	const char* inputfilename = "../data/inputtest.txt";
	const char* outputfilename = "../data/SegmentResult.txt";
	string strtmp; //���ڱ�������Ͽ��ж����ÿһ��
	string line; //�������ÿһ�еĽ��

	ifstream infile(inputfilename);
	if (!infile.is_open()) // �������ļ�ʧ�����˳�����
	{
		cout<<"error"<<endl;
		cerr << "Unable to open input file: " << inputfilename
		<< " -- bailing out!" << endl;
		exit(-1);
	}

	ofstream outfile(outputfilename); //ȷ������ļ�
	if (!outfile.is_open())
	{
		cerr << "Unable to open file��SegmentResult.txt"
		<< "--bailing out!" << endl;
		exit(-1);
	}

	while (getline(infile, strtmp, '\n')) //�������Ͽ��е�ÿһ�в������ƥ�䷨����
	{
		line = strtmp;
		cout<<"line"<<line<<endl;
		line = SegmentSentenceMM(line); // ���÷ִʺ������зִʴ���
		cout<<"line"<<line<<endl;
		outfile << line << endl; // ���ִʽ��д��Ŀ���ļ�
	}
	cout<<"wordsegment done!!!"<<endl;
	return 0;
}