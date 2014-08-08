#include "TFIDF.h"
using namespace std;

TFIDF::TFIDF()
{
	docbase = "../database/SougouReduced/";
	doclist = "../data/doclist.txt";
	idfmapfile = "../data/idfmapdata.txt";
}
TFIDF::~TFIDF()
{
}
//将字符串按 指定的字符串分割
vector<string>tfsplit(string str,string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size(); 
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            if (s.length()>0)
            {
            	result.push_back(s);
 	        	i=pos+pattern.size()-1;
            }

        }
    }
    return result;
}
void TFIDF::getDocList(string dirstr, const char* outputfile)
{
	// const char* inputfile = "E:\\Document\\TempFile\\TEST\\";
	ofstream outfile;
	outfile.open(outputfile,ios::app);
	string str = dirstr + "*.*";
	long handle ;
	struct _finddata_t fileinfo; 
	handle = _findfirst(str.c_str(), &fileinfo);
	if (handle == -1)
	{
		return ;
	}
	while( _findnext( handle,  &fileinfo ) != -1 )
	{
		if (fileinfo.attrib == _A_SUBDIR)
		{
		   if(strcmp(fileinfo.name,".")==0 || strcmp(fileinfo.name,"..")==0)
                continue ;
            string childpath = dirstr  + fileinfo.name + "/";
			//如果找到的路径为目录，则递归
           	getDocList(childpath, outputfile);
		}
       else
       {
		    //将找到的文件写入文档中
       		//cout<<dirstr<<fileinfo.name<<endl;
       		outfile<<dirstr<<fileinfo.name<<endl;
       }
	}
	outfile.close();
	return ;
}
//得到输入文件（已经分词之后的），的map表
void TFIDF::getWordMap(const char* inputfile)
{
	int val = 0;
	WordNum = 0;
	string strtmp;
	ifstream infile(inputfile);
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		string pattern = separate;
		vector<string> result = tfsplit(strtmp,pattern);
		WordNum +=result.size();
		for (int i = 0; i < result.size(); ++i)
		{
			string str = result[i].c_str();
			map<string, int>::iterator m_it_w;
			m_it_w = wordmap.find(str);
			if (m_it_w == wordmap.end())
			{
				wordmap.insert(pair<string, int>(str, val));
				val++;
			}
		}
	}
	MaxVal = val;
	return ;
}

void TFIDF::getTFData(const char* inputfile)
{
	int val = wordmap.size();
	float* value = new float [val];
	for(int i= 0;i<val; i++)
	{
		value[i] = 0.0;
	}
	string strtmp;
	ifstream infile(inputfile);
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		string pattern = separate;
		vector<string> result = tfsplit(strtmp,pattern);
		for (int i = 0; i < result.size(); ++i)
		{
			string str = result[i].c_str();
			value[wordmap[str]]++;
		}
	}
	for (int i = 0; i < val; ++i)
	{
		value[i] =(float) value[i] / WordNum;
		//cout << value[i]<<"  ";
	}
	//cout<<endl;
	map<string,int>::iterator it=wordmap.begin();
	for(;it!=wordmap.end();++it)
	{
		tfmap.insert(pair<string, float>(it->first, value[wordmap[it->first]]));
	}
	return ;
}

void TFIDF::getIDFData(const char* inputfile)
{
	string strtmp;
	std::vector<string> vstr;
	ifstream infile(inputfile);
	while(getline(infile, strtmp, '\n'))
	{
		if (strtmp.empty())
		{
			continue ;
		}
		
		vstr.push_back(strtmp);
	}
	DocNum = vstr.size();
	map<string, int>::iterator it_w = wordmap.begin();
	map<string, float>::iterator it_idf = idfmap.begin();
	int val = wordmap.size();
	float* value = new float [val];
	for(int i= 0;i<val; i++)
	{
		value[i] = 0.0;
	}
	//第一轮循环，对每一个wordmap遍历
	//在查找之前，先查找idfmap表，如果纯在，则直接载入
	//第二轮循环，对每一个文件进行查找
	//第三轮循环，对每个文件的每一行进行查找
	for(;it_w!=wordmap.end();++it_w)
	{
		it_idf = idfmap.find(it_w->first);
		if (it_idf != idfmap.end())
		{
			value[it_w->second] = idfmap[it_w->first];
			continue ;
		}
		for(int n=0 ; n<vstr.size();n++)
		{
			string str = vstr[n];
			string strtmp;
			ifstream infile(str);
			while(getline(infile, strtmp, '\n'))
			{
				if (strtmp.empty())
				{
					continue ;
				}
				if (strtmp.find(it_w->first) != string::npos)
				{
					value[it_w->second]++;
					break;
				}
			}			
		}	
	}
	for(int i= 0;i<val; i++)
	{
		if(value[i] != 0)
		{
			value[i] =(float)log((float)DocNum/value[i]);
		}
		//cout<<value[i]<<" ";
	}
	//cout<<endl;

	map<string,int>::iterator it=wordmap.begin();
	for(;it!=wordmap.end();++it)
	{
		idfmap.insert(pair<string, float>(it->first, value[wordmap[it->first]]));
	}
	return ;
}	

void TFIDF::saveIDFMap(const char* outputfile)
{
	ofstream outfile;
	outfile.open(outputfile);
	std::map<string,float>::iterator m_it;
	for ( m_it = idfmap.begin(); m_it != idfmap.end(); m_it++ )
	{
		outfile << m_it->first <<" "<< m_it->second << endl;
	}
	return ;
}

void TFIDF::loadIDFMap(const char* inputfile)
{
	 ifstream infile1;
	 infile1.open(inputfile);
	 string linetmp;
	 int linenum=0;
	 // 统计输入文件的行数
	 while(getline(infile1, linetmp, '\n'))
	 {
		if (linetmp.empty())
		{
			continue ;
		}
	 	linenum++;
	 }
	 infile1.close();
	 //载入新的map表
	 ifstream infile2;
	 infile2.open(inputfile);
	 string line, strtmp;
	 for (int i = 0; i < linenum; ++i)
	 {
	 	infile2 >> line;
	 	infile2 >>strtmp;
		float num = atof(strtmp.c_str());
	 	idfmap.insert(pair<string,float>(line,num));
	 }
	 infile2.close();
}
//输出关键词排序的结果，
void TFIDF::getKeyWords(const char* inputfile, int n)
{
	getWordMap(inputfile);
	getTFData(inputfile);
	if(!doclist)
	{
		getDocList(docbase,doclist);
	}
	if(idfmapfile)
	{
		loadIDFMap(idfmapfile);
	}
	getIDFData(doclist);
	saveIDFMap(idfmapfile);
	map<double, string> keywordsmap;

	map<string,int>::iterator it=wordmap.begin();
	for(;it!=wordmap.end();++it)
	{
		double val = tfmap[it->first] * idfmap[it->first];
		keywordsmap.insert(pair<double,string>(val, it->first));
	}

	cout<<"the keywords is ......"<<endl;
	map<double,string>::reverse_iterator it_t = keywordsmap.rbegin();
	for(int i =0;(it_t != keywordsmap.rend()&& i <n);++it_t,++i)
	{
		//cout<< it_t->first<<"  "<<it_t->second<<endl;
		cout<<it_t->second<<endl;
	}
	return ;
}