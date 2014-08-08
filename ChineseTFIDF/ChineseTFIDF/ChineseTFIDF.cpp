// ChineseTFIDF.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TFIDF.h"

int _tmain(int argc, _TCHAR* argv[])
{
	const char* inputfile = "../data/segresult.txt";
	TFIDF TF;
	TF.getKeyWords(inputfile,7);
	return 0;
}

