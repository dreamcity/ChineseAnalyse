//// helloworld.cpp : 定义控制台应用程序的入口点。
////
//
#include "stdafx.h"
#include <stdio.h> 
#include <wchar.h>  
#include <locale.h>  
#include "iostream"
#include <string>
#include <sstream>
#include <vector>
#include "fstream"
#include<Windows.h>
#include<tchar.h>
#include<stdio.h>
using namespace std;

char* WcharToChar(const wchar_t* wp) 
{ 
    char *m_char;
    int len= WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),NULL,0,NULL,NULL); 
    m_char=new char[len+1]; 
    WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),m_char,len,NULL,NULL); 
    m_char[len]='\0'; 
    return m_char; 
}  
wchar_t* CharToWchar(const char* c) 
{ 
   wchar_t *m_wchar;
    int len = MultiByteToWideChar(CP_ACP,0,c,strlen(c),NULL,0); 
    m_wchar=new wchar_t[len+1]; 
    MultiByteToWideChar(CP_ACP,0,c,strlen(c),m_wchar,len); 
    m_wchar[len]='\0'; 
    return m_wchar; 
}  
//字符串分割函数
 vector<string> split(string str,string pattern)
 {
     std::string::size_type pos;
     std::vector<std::string> result;
     str+=pattern;//扩展字符串以方便操作
     int size=str.size(); 
     //cout<<"size:"<<size<<endl;
     //int j =0;
     for(int i=0; i<size; i++)
     {
         pos=str.find(pattern,i);
         //cout<<"pos"<<i<<":"<<pos<<endl;
         if(pos<size)
         {
             std::string s=str.substr(i,pos-i);
             //cout<<"length:"<<s.length()<<endl;
             // cout<<"sizeof(s)"<<sizeof(s)<<endl;
             if (s.length()>0)
             {
             	result.push_back(s);
 	            // cout<<"result"<<j<<":"<<result[j]<<endl;
 	            //            j++;
 	            i=pos+pattern.size()-1;
 	           // cout<<"i:"<<i<<endl;
             }

         }
     }
     return result;
 }
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "chs"); 
	const char* inputfile = "inputtest.utf8";
 	//cout<<inputfile[0]<<endl;
 	const char* outputfile = "outputtest.txt";
	const char* outputfile1 = "outputtemp.txt";
 	string strtmp; //用于保存从语料库中读入的每一行
 	string markline;
 	ifstream infile(inputfile);
 	ofstream outfile (outputfile);
	ofstream outfile1 (outputfile1);
	for (long i = 0; i < 1000; ++i)
 	{
 		getline(infile, strtmp, '\n');
		//infile>>strtmp;
		//outfile <<i<<":"<<strtmp<<"/S" << endl;
     	string pattern = " ";
     	vector<std::string> result=split(strtmp,pattern);

		for(int i=0; i<result.size(); i++)
 	    {

			string t = result[i].c_str();
			int l =t.length();
			int length = t.length();
			 const wchar_t* strtmp = CharToWchar(result[i].c_str());
			 wstring tmp = strtmp;

			 cout<<"length:"<<t.length()<<endl;
			 cout<<result[i]<<"/E"<<endl;
			 string resulttemp;
			 //outfile<<"tmp[0]:"<<tmp[0]<<endl;
			 //cout<<"t[0]"<<t[0]<<endl;
			 if (tmp[0]>256)  // 区分中英文字符
			 //if(length >= 3)
			 {
				 if (length == 3)
				 {
					 result[i]+="/S";
					 resulttemp = "S";
				 }
				 else if (length == 6)
				 {
					 result[i] = t.substr(0,3)+"/B";
					 result[i]+= t.substr(3,6)+"/E";
					 resulttemp = "BE";
				 }
				 else
				 {
					string m =t.substr(0,3)+"/B";
					resulttemp = "B";
					for( int k = 3; k<length-3;)
					{
						m+= t.substr(k,3) + "/M"; 
						resulttemp += "M";
						k+=3;
					}
					 result[i] =m+t.substr(length-3,3)+"/E"; 
					 resulttemp += "E";
				 }
			 }
			 else
				{
				result[i]+="/E";
				 resulttemp = "E";
			 }
			// std::wcout<<strtmp[0]<<"/M"<<endl;
			outfile << result[i] ; // 将分词结果写入目标文件
			outfile1 << resulttemp;
			//outfile << strtmp[0]<<"/M" << endl; // 将分词结果写入目标文件
		}
		//cout<<endl;
		outfile<<endl;
		outfile1<<endl;
	}
	return 0;

}
