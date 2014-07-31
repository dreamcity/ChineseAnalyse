// ChineseSplitHMM.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//�ж�map�ļ���hmmmodel�Ƿ���ڣ������ڣ��򴴽������ڣ���ֱ�Ӽ���
	ifstream mapfile;
	ifstream modelfile;
	mapfile.open(MAPData);
	modelfile.open(HMMModel);
	if(!mapfile || !modelfile)
	{
		ChineseSplit CNS;
		//���۲����ݼ���ת��Ϊ����������ȡ���ַ�������
		CNS.getSplitSegFile(databasefile, splitsegfile);
		//�õ����״̬֮���markfile���Ѿ������״̬���м�
		CNS.getMarkSentenceFile(splitsegfile, markfile, statefile);
		CNS.initialModel();
		CNS.getInitMatrix(statefile);	//�����ʼ���ʾ���
		CNS.getTranMatrix(statefile);	//����ת�Ƹ��ʾ���
		CNS.getConMatrix(markfile);		//����������ʾ���
		CNS.saveMapData(MAPData);		//��ȡ�۲����м����У��ַ���Ӧ��map���ݱ�
		CNS.saveHMMModel(HMMModel);
	}
	//������Ĵ��ִ��ı������Ȱ���������ȡ�ַ���
	getSplitSegFile(datatestfile, datasegfile);
	//����MAP����Ϣ��������������ַ�����ת��Ϊ��Ӧ�Ĺ۲����б���
	getOSequence(MAPData,  datasegfile, Osequence);
	//׼������ ����۲����еĳ��������
	int num;
	getlinenum(Osequence,num);
	int* T = new int [num];
	int** O = new int* [num];
	//��ȡ�ı��еĹ۲����У��洢��������
	getOSdata(Osequence, num , T, O);
	string strtmp = "";
	string str = "";
	vector<string> vstr;
	//������������ַ���������vector<string>������
	file2Vector(datasegfile, vstr);
	//��ʼ��HMMģ�ͣ��������ɹ۲��������ɵ�HMM����
	HMM hmm;
	hmm.initialModel(HMMModel);
	//�ִʽ������segresult��
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
		//����viterbi�㷨���õ�path���У�int ���飩
		hmm.viterbi(path);	
		outputpath(T[i], path); //����int���飬�õ��������SBME
		//ͨ��path���飬���ָ���������ַ�����vector�����е�ÿһ��Ԫ��
		getVStrSegResult(path, vstr, str);		
		strtmp += str;
		delete path;
	}
	fout<<strtmp<<endl;
	return 0;
}
