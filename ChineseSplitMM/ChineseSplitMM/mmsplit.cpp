#include "doublemmseg.h"
#include "iostream"
#include "fstream"
#include "string"
int main(int argc, char const *argv[])
{
	const char* inputfile = "../data/wordlexion.txt";
	const char* outputfile = "../data/segmentresult.txt";
	string strtmp;	//用于保存从语料库中读入的每一行
	string line;	//用于输出每一行的结果
	ifstream infile;
	infile.open(inputfile);
	if (!infile.is_open())	// 打开输入文件失败则退出程序
	{
		cerr << "Unable to open input file: " << inputfilename
		<< " -- bailing out!" << endl;
		exit(-1);
	}
	ofstream outfile;
	outfile.open(outputfile);	//确定输出文件
	if (!outfile.is_open())
	{
		cerr << "Unable to open input file: " << inputfilename
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
		outfile << strtmp <<endl;
	}
	outfile.close();
	infile.close();
	return 0;
}