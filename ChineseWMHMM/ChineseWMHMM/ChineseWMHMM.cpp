// ChineseWMHMM.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "WordModel.h"
#include "HMMmodel.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	const char* inputfile =  "../inputdata/inputfile.txt";
	const char* markfile = "../data/markfile.txt"; //�����ŷ־�
	const char* statefile = "../data/statefile.txt"; //�����ŷ־�
	const char* statemapdatafile =  "../data/statemapdata.txt";
	const char* wordmapdatafile =  "../data/wordmapdata.txt";
	const char* modelfile =  "../data/model.txt";
	//const char* markfile = "markfile.txt"; //�����ŷ־�
	//const char* statefile = "statefile.txt"; //�����ŷ־�
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

