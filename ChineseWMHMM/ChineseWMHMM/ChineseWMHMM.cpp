// ChineseWMHMM.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "WordModel.h"
#include "HMMmodel.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	const char* inputfile =  "../inputdata/inputfile.txt";
	const char* markfile = "../data/markfile.txt"; //标点符号分句
	const char* statefile = "../data/statefile.txt"; //标点符号分句
	const char* statemapdatafile =  "../data/statemapdata.txt";
	const char* wordmapdatafile =  "../data/wordmapdata.txt";
	const char* modelfile =  "../data/model.txt";
	//const char* markfile = "markfile.txt"; //标点符号分句
	//const char* statefile = "statefile.txt"; //标点符号分句
	//const char* statemapdatafile =  "statemapdata.txt";
	//const char* wordmapdatafile =  "wordmapdata.txt";
	//const char* modelfile =  "model.txt";
	WordModel wm;
	wm.filePreprocess(inputfile, markfile);	
	wm.getMapData(markfile,statefile);
	wm.initialModel();
	wm.getIniPro(statefile);
	wm.getTranPro(statefile);
	wm.getConPro(markfile);
	wm.saveModelData(modelfile);
	map<string , int> statemap = wm.getStateMap();
	map<string , int> wordmap = wm.getWordMap();
	wm.saveMapData(statemapdatafile, statemap);
	wm.saveMapData(wordmapdatafile, wordmap);
	return 0;
}

