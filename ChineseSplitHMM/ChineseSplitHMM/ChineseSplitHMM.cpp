// ChineseSplitHMM.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ChineseSplitHMM.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	const char* databasefile = "../data/databasefile.txt";
	const char* splitsegfile = "../tempdata/splitsegfile.txt";
	const char* markfile = "../tempdata/markfile.txt";
	const char* statefile = "../tempdata/statefile.txt";
	const char* HMMModel = "../data/hmmmodel.txt";
	const char* MAPData = "../data/mapdata.txt";
	const char* datatestfile = "../iodata/datatestfile.txt";
	const char* datasegfile = "../tempdata/datasegfile.txt";
	const char* Osequence = "../tempdata/osequence.txt";
	const char* segresult = "../iodata/segresult.txt";
	//判断map文件与hmmmodel是否存在，不存在，则创建；存在，则直接加载
	ifstream mapfile;
	ifstream modelfile;
	mapfile.open(MAPData);
	modelfile.open(HMMModel);
	if(!mapfile || !modelfile)
	{
		ChineseSplit CNS;
		//将观测数据集，转换为按标点符号提取的字符串集合
		CNS.getSplitSegFile(databasefile, splitsegfile);
		//得到标记状态之后的markfile，已经纯粹的状态序列集
		CNS.getMarkSentenceFile(splitsegfile, markfile, statefile);
		CNS.initialModel();
		CNS.getInitMatrix(statefile);	//计算初始概率矩阵
		CNS.getTranMatrix(statefile);	//计算转移概率矩阵
		CNS.getConMatrix(markfile);		//计算混淆概率矩阵
		CNS.saveMapData(MAPData);		//提取观测序列集合中，字符对应的map数据表
		CNS.saveHMMModel(HMMModel);
	}
	//将输入的待分词文本，首先按标点符号提取字符串
	getSplitSegFile(datatestfile, datasegfile);
	//根据MAP的信息，将输入的中文字符串，转换为对应的观测序列编码
	getOSequence(MAPData,  datasegfile, Osequence);
	//准备载入 输入观察序列的长度与编码
	int num;
	getlinenum(Osequence,num);
	int* T = new int [num];
	int** O = new int* [num];
	//提取文本中的观测序列，存储在数组中
	getOSdata(Osequence, num , T, O);
	string strtmp = "";
	string str = "";
	vector<string> vstr;
	//将输入的中文字符串，存入vector<string>容器内
	file2Vector(datasegfile, vstr);
	//初始化HMM模型，并载入由观测序列生成的HMM参数
	HMM hmm;
	hmm.initialModel(HMMModel);
	//分词结果存入segresult中
	ofstream fout(segresult);
	for (int i = 0; i < num; ++i)
	{			
		hmm.setT(T[i]);
		hmm.setO(O[i]);
		int* path = new int [T[i]];
		for(int j =0; j<T[i] ; j++)
		{
			path[j] = 0;
		}
		//调用viterbi算法，得到path序列（int 数组）
		hmm.viterbi(path);	
		outputpath(T[i], path); //根据int数组，得到标记数组SBME
		//通过path数组，将分隔符标记在字符串的vector容器中的每一个元素
		getVStrSegResult(path, vstr, str);		
		strtmp += str;
		delete path;
	}
	fout<<strtmp<<endl;
	return 0;
}
