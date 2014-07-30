// ChineseSplitHMM.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "iostream"
#include <fstream>
#include <vector>
#include "HMMmodel.h"
#include "ChineseSplit.h"
#include "map"
#ifndef Separator
#define Separator "/ "
#endif

using namespace std;
//������Ĳ����ı����д����õ����ձ����ŷָ�֮��ľ��Ӽ���
//ÿһ�仰������д��һ��
void getsegdatafile(const char* inputfile, const char* outputfile)
{
	//const char* infilename = "inputfile.txt";
	ifstream infile;
	infile.open(inputfile);
	ofstream outfile;
	outfile.open(outputfile);
	string strtmp;
	string str1 = "";
	string str2 = "";
	while(getline(infile, strtmp, '\n'))
	{
		//getline(infile, strtmp, '\n');
		while(!strtmp.empty())
		{

			int len = strtmp.length();
			//cout<<"len:"<<len<<endl;
			//cout<<"strtmp:"<<strtmp<<endl;
			int index =0;
			for (int i = 0; i < len; i+=2)
			{
				unsigned char ch = strtmp[i];
				if (ch > 176)
				{
					index+=2;
				}
				else
				{
					break;
				}
			}
			// cout<<"strtmp:"<<strtmp<<endl;
			// cout<<"index:"<<index<<endl;
			if (index == len)
			{
				str2 = strtmp.substr(0, index);
				strtmp = "";
			}
			else
			{
				str1 = str2 + strtmp.substr(0, index+2);
				str2 = "";
				strtmp =strtmp.substr(index+2, len-index-2);
			}
			
			//cout<<"strtmp:"<<strtmp<<endl;
			if (str2.empty())
			{
				int len = str1.length();
				if (len >= 2)
				{
					string tmp = str1.substr(len-2,len);
					unsigned char c =tmp[0];
					if(c < 176)
					{
						outfile<<str1.substr(0, len-2)<<endl;
						outfile<<str1.substr(len-2, 2)<<endl;
					}
					else
					{
						outfile << str1 <<endl;
					}
				}
				//cout<<"str1:"<<str1<<endl;
				
			}
			//cout<<"str2:"<<str2<<endl;
		}
	}
	//str1 = str2��
	//cout<<"str1:"<<str2<<endl;
	outfile << str2 <<endl;
	return ;
}

//���������ѵ��ʱ�Ĺ۲켯���ɵ�map��
void loadmapdata(const char* inputfile, map<string, int>& mm)
{
	 ifstream infile1;
	 infile1.open(inputfile);
	 string linetmp;
	 int linenum=0;
	 while(getline(infile1, linetmp, '\n'))
	 {
		if (linetmp.empty())
		{
			continue ;
		}
	 	linenum++;
	 }
	 infile1.close();

	 ifstream infile2;
	 infile2.open(inputfile);
	 string line, strtmp;
	 for (int i = 0; i < linenum; ++i)
	 {
	 	infile2 >> line;
	 	infile2 >>strtmp;
		int num = atoi(strtmp.c_str());
	 	mm.insert(pair<string,int>(line,num));
	 }
	 infile2.close();

}
//inputfile1 Ϊmap���ݱ�
//inputfile2 Ϊ���Ե������ļ�
//  inputfile2��ʵ��������������ļ�����������������ȡΪ����֮����ļ�
//	��ˣ��ڵ���֮ǰ�������getsegdatafile()����
//outputfile Ϊ���������ļ���ÿһ�����ɵĹ۲�����
//  ���У���һ��Ϊ���г��ȣ�����һ��Ϊ������map���еı��
void getOSequence(const char* inputfile1,const char* inputfile2, const char* outputfile)
{
	//ChineseSplit cs;
	map<string, int> mdata;
	//mdata = CNS.getMapData();

	loadmapdata(inputfile1, mdata);

	int T = 0;
	int* OS;
	int num = 0;
	string strtmp;
	string str;
	ifstream infile;
	infile.open(inputfile2);
	ofstream outfile;
	outfile.open(outputfile);
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		int len = strtmp.length();
		T = len/2;
		OS = new int [T];
		for (int i = 0; i < T; ++i)
		{
			OS[i] = 0;
		}
		int j=0;
		for (int i = 0; i < len-1;i+=2)
		{
			str = strtmp.substr(i,2);
			j = i/2;
			int n;
			n = mdata[str];
			OS[j] = mdata[str];
		}
		outfile << T <<endl;
		for (int i = 0; i < T; ++i)
		{
			outfile << OS[i] <<" ";
		}
		outfile <<endl;
	}
	outfile.close();
	infile.close();
	//*********************************************************************
}
// ����viterbi�㷨�õ������״̬���У���ţ���ת��ɶ�Ӧ��S,B,M,E�������
void outputpath(int T, int* path)
{
	char* state = new char [T];
	map<int, char> mic;
	mic[0] = 'S';
	mic[1] = 'B';
	mic[2] = 'M';
	mic[3] = 'E';
	for(int i =0; i<T ; i++)
	{
		state[i] = mic[path[i]];
	}
	cout <<"the output state is :"<<endl;
	for(int i =0; i<T-1 ; i++)
	{
		cout<<state[i]<<"->";
	}
	cout<<state[T-1]<<endl;
	return ;
}
//begin******************************************************************
//����viterbi�㷨�õ������״̬���У���ţ���������Ĳ����ļ����б��
// getsegresult()
//		path:viterbi�㷨�õ���״̬�������
//		inputfile:����Ĳ����ı�tmp����������������ȡΪ����֮����ļ���
//		outputfile:���֮����ļ�------------------�����޸ģ�
//		str:���֮����ַ���
//   ***getsplitstr()
void getSplitStr(int* path, string str1, string& str2)
{
	int i =-1;
	int j =0;
	int m ;
	int L = str1.length();
	while(!str1.empty())
	{
		int len = str1.length();
		//cout<<"len:"<<len<<endl;
		switch(path[++i])
		{
			case 0:
				str2 += str1.substr(0,2) + Separator;
				str1 = str1.substr(2,len-2);
				break;
			case 1: j = i; break;
			case 2:  break;
			case 3:
				m = (i-j+1)*2;
				str2 += str1.substr(0,m) + Separator;
				str1 = str1.substr(m,len-m);
				break;
			default : cout<<"ERROR"<<endl; break;
		}
		//���ǵ����ִʽ���������ʱ��Ĵ�������ungly
		//��������BM��βʱ
		//�����Ծ������һ���ֵ�״̬��ֱ����Ϊ��β��
		if (i == L/2 -2)
		{
			str2 += str1 + Separator;
			str1 = "";
		}
		//cout<<"str1len:"<<str1.length()<<endl;
	}
	return ;
}
//void getSegResult(int* path, const char* inputfile, const char* outputfile, string& str)
//{
//	str = "";
//	ifstream fstr;
//	fstr.open(inputfile);
//	ofstream resultfile;
//	resultfile.open(outputfile);
//	string strtmp;
//	
//	while(getline(fstr, strtmp,'\n'))
//	{
//		if (strtmp.empty())
//		{
//			continue;
//		}
//		string strout = "";
//		getSplitStr( path, strtmp, strout);
//		resultfile<<strout<<endl;
//		str+= strout;
//	}
//	
//}
//end********************************************************************


//�������ļ���ÿһ��ѹ��vector��
//�����ļ�Ϊ�������ŷָ�֮��ľ���
void file2Vector(const char* inputfile, vector<string>& vstr)
{	
	ifstream fin;
	string strtmp;
	fin.open(inputfile);
	while(getline(fin, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		vstr.push_back(strtmp);
	}
	return ;
}
void getVStrSegResult(int* path, vector<string>& vstr, string& str)
{
	if(!vstr.empty())
	{
		string strtmp = vstr[0];
		vector<string>::iterator v_it;
		v_it = vstr.begin();
		vstr.erase(v_it);
		string strout = "";
		getSplitStr( path, strtmp, strout);
		str= strout;
	}
	return ;
}



//begin******************************************************************
//ͳ���ļ��м�¼������
//inputfile���۲����м��ϵ��ļ�
//num��ͳ�ƶ��ٸ��۲�����
void getlinenum(const char* inputfile, int& num)
{
	string strtmp;
	ifstream fin;
	fin.open(inputfile);
	int numline=0;
	while(getline(fin, strtmp, '\n'))
	{
		numline++;
	}
	fin.close();
	num = (int)numline/2;
	return ;
}
//ͨ����ȡ�۲����м��ϵ��ļ��������е���Ŀ�볤�ȷֱ����������
//inpufile:�۲����м��ϵ��ļ�
// int* T: ����ÿһ�����еĳ���
// int**O,�У����еĸ������У����еĳ���
void getOSdata(const char* inputfile, int num ,int* T, int** O)
{
	ifstream fin;
	fin.open(inputfile);
	for (int i = 0; i < num; ++i)
	{
			
		fin >> T[i];
		O[i] = new int [T[i]];
		for (int j = 0; j < T[i]; ++j)
		{
			fin >> O[i][j];
		}
	}
	cout <<"T[i]:"<<T[0]<<endl;
	fin.close();
	cout <<"T[i]:"<<T[0]<<endl;
	return ;
}
//end***********************************************************************

//��������Ĺ۲����еĳ��ȣ������У��õ������path
//����path���õ����֮����ַ���getsegresult()
// inputfileΪ�����HMMģ�Ͳ���
// num Ϊ����ǵľ�����Ŀ
//void recstr(const char* inputfile,vector<string>& vstr,int num, int* T, int** O)
//{
//	string strtmp = "";
//	string str = "";
//	vector<string> vstr;
//	file2Vector(datasegfile, vstr);
//	HMM hmm;
//	hmm.initialModel(HMMModel);
//	ofstream fout(segresult);
//
//	for (int i = 0; i < num; ++i)
//	{		
//
//		hmm.setT(T[i]);
//		hmm.setO(O[i]); 
//		int* path = new int [T[i]];
//		for(int j =0; j<T[i] ; j++)
//		{
//			path[j] = 0;
//		}
//		hmm.viterbi(path);
//		outputpath(T[i], path);
//		getVStrSegResult(path, vstr, str);		
//		strtmp += str;
//		fout<<strtmp<<endl;
//	}
//}

//����۲����м���
//�õ�����ַ���
//void getresult(const char* inputfile, string str)
//{
//	//const char* inputfile = "osequence.txt";
//	int num;
//	getlinenum(inputfile,num);
//	//cout<<"num:"<<num<<endl;
//	ifstream fin;
//	fin.open(inputfile);
//	int* T = new int [num];
//	int** O = new int*[num];
//	getOSdata(inputfile, num , T, O);
////	recstr(num, T,O);
//	return ;
//}
int _tmain(int argc, _TCHAR* argv[])
{
	const char* databasefile = "../data/databasefile.txt";
	const char* markfile = "../tempdata/markfile.txt";
	const char* statefile = "../tempdata/statefile.txt";
	const char* HMMModel = "../data/hmmmodel.txt";
	const char* MAPData = "../data/mapdata.txt";
	const char* datatestfile = "../iodata/datatestfile.txt";
	const char* datasegfile = "../iodata/datasegfile.txt";
	const char* Osequence = "../iodata/osequence.txt";
	const char* segresult = "../iodata/segresult.txt";
	//�ж�map�ļ���hmmmodel�Ƿ���ڣ������ڣ��򴴽������ڣ���ֱ�Ӽ���
	ifstream mapfile;
	ifstream modelfile;
	mapfile.open(MAPData);
	modelfile.open(HMMModel);
	if(!mapfile || !modelfile)
	{
		ChineseSplit CNS;
		CNS.getMarkSentenceFile(databasefile, markfile, statefile);
		CNS.initialModel();
		CNS.getInitMatrix(statefile);
		CNS.getTranMatrix(statefile);
		CNS.getConMatrix(markfile);
		CNS.saveMapData(MAPData);
		CNS.saveHMMModel(HMMModel);
	}

	getsegdatafile(datatestfile, datasegfile);
	getOSequence(MAPData,  datasegfile, Osequence);
	//************************
	//׼������ ����۲����еĳ��������
	int num;

	getlinenum(Osequence,num);
	//cout<<"num:"<<num<<endl;
	int* T = new int [num];
	int** O = new int* [num];
	getOSdata(Osequence, num , T, O);
	cout <<"T[i]:"<<T[0]<<endl;
	string strtmp = "";
	string str = "";
	vector<string> vstr;
	file2Vector(datasegfile, vstr);
	HMM hmm;
	hmm.initialModel(HMMModel);
	ofstream fout(segresult);

	for (int i = 0; i < num; ++i)
	{		
		cout <<"T[i]:"<<T[0]<<endl;
		hmm.setT(T[i]);
		hmm.setO(O[i]);
		for (int k=0; k<T[i] ; k++)
		{
			cout<<O[i][k]<<endl;
		}
		int* path = new int [T[i]];
		for(int j =0; j<T[i] ; j++)
		{
			path[j] = 0;
		}
		hmm.viterbi(path);
		outputpath(T[i], path);
		getVStrSegResult(path, vstr, str);		
		strtmp += str;
	}
	fout<<strtmp<<endl;

	//**************************
	//HMM hm;
	//hm.initialData(HMMModel, Osequence);
	//int T = hm.getT(); 
	//int* path = new int [T];
	//for(int i =0; i<T ; i++)
	//{
	//	path[i] = 0;
	//}
	//hm.viterbi(path);
	//outputpath(T, path);
//	getsegresult(path, datatestfile, segresult);
	return 0;
}
