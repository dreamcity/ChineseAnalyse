#include "dictionary.h"
#include "vector"

using namespace std;
# define Separator " " // 词界标记
# define MaxWordLength 30 // 最大词长为个字节（即个汉字）
const char* dict = "../dic/dictionarytest.dict";
CDictionary WordDic(dict); //初始化一个词典
vector<string> split(string str,string pattern)
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
//对句子进行最大匹配法处理，包含对特殊字符的处理
//对字符串用最大匹配法（正向或逆向）处理
string pSegmentSentence(string str1, int& onum)
{
	onum = 0;
	string str2 = "";
	while (!str1.empty())
	{
		int len = str1.length();
		if ( len > MaxWordLength)
		{
			len = MaxWordLength;
		}

		string strtmp = str1.substr(0, len);
		int index = WordDic.findWord(strtmp);
		while(len > 2 && index == 0)
		{
			len -=2;
			strtmp = str1.substr(0, len);
			index = WordDic.findWord(strtmp);
		}
		if (len ==2 && index==0)
		{
			onum++;
		}
		str2 += strtmp + Separator; 
		str1 = str1.substr(strtmp.length(), str1.length() - strtmp.length());
	}
	return str2;
}

string nSegmentSentence(string str1, int& onum)
{
	onum = 0;
	string str2 = "";
	while (!str1.empty())
	{
		int len = str1.length();
		if ( len > MaxWordLength)
		{
			len = MaxWordLength;
		}
		string strtmp  = str1.substr((str1.length() - len), len); //逆向用
		int index = WordDic.findWord(strtmp);
		while(len > 2 && index == 0)
		{
			len -= 2;
			strtmp = str1.substr((str1.length() - len), len);
			index = WordDic.findWord(strtmp);
		}
		if (len ==2 && index==0)
		{
			onum++;
		}
		strtmp = strtmp + Separator; // (逆向用)
		str2 = strtmp + str2 ; // (逆向用)
		str1 = str1.substr(0, str1.length() - len); // (逆向用)
	}
	return str2;
}
//对字符串用最大匹配法（正向或逆向）处理
void splitMM(string str1, string& str2,int& onum, bool flag)
{
	// str2
	//for (int i = 0; i < len; i+=2)
	while(!str1.empty())
	{
		int len = str1.length();
		unsigned char ch = (unsigned char)str1[0];
		// 处理西文字符
		if (ch < 128)
		{
			int index =1;
			//while (index < len && ((unsigned char)str1[index] < 128)) 
			//{
			//	index++;
			//}
			//str2 += str1.substr(0,index) + Separator;
			//str1 = str1.substr(index, len - index);
			//continue ;
			//***************************************************************
			//特殊的情况考虑在内
			while (index < len && ((unsigned char)str1[index] < 128) && (str1[index] != 10) && (str1[index] != 13)) // str1[i]不能是换行符或回车符
			{
				index++;
			}
			
			if ((ch != 32) && (ch != 10) && (ch != 13)) // 如果不是西文空格或换行或回车符
			 {
			 	str2 += str1.substr(0,index) + Separator;
			 }
			 else
			 {
			 	//if (ch == 10 || ch == 13) // 如果是换行或回车符，将它拷贝给s2输出
			 	if (ch == 10 || ch == 13 || ch == 32) 
			 	{
			 		str2 += str1.substr(0, index);
			 	}
			 }
			 str1 = str1.substr(index, len - index);
			 continue ; 
			//***************************************************************
		}
		else
		{
			if (ch < 176)
			{
				int index =0;
				int len = (int)str1.length();
				//while(index < len && ((unsigned char)str1[index] < 176))
				//{
				//	index = index + 2; // 假定没有半个汉字
				//}
				//str2 += str1.substr(0, index) + Separator;
				//str1 = str1.substr(index, len - index);
				//continue;
				//***************************************************************
				//特殊的情况考虑在内
				 while(index < len && ((unsigned char)str1[index] < 176) && ((unsigned char)str1[index] >= 161)
				 && (!((unsigned char)str1[index] == 161 && ((unsigned char)str1[index+1] >= 162 && (unsigned char)str1[index+1] <= 168)))
				 && (!((unsigned char)str1[index] == 161 && ((unsigned char)str1[index+1] >= 171 && (unsigned char)str1[index+1] <= 191)))
				 && (!((unsigned char)str1[index] == 163 && ((unsigned char)str1[index+1] == 172 || (unsigned char)str1[index+1] == 161)
				 || (unsigned char)str1[index+1] == 168 || (unsigned char)str1[index+1] == 169 || (unsigned char)str1[index+1] == 186
				 || (unsigned char)str1[index+1] == 187 || (unsigned char)str1[index+1] == 191)))
				 {
				 	index = index + 2; // 假定没有半个汉字
				 }
				 if (index == 0)
				 {
				 	index = index + 2;
				 }
				 if (!(ch == 161 && (unsigned char)str1[1] == 161)) // 不处理中文空格
				 {
				 	str2+=str1.substr(0, index) + Separator; // 其他的非汉字双字节字符可能连续输出
				 }
				 str1 = str1.substr(index, len - index);
				 continue;
				//***************************************************************
			}
			int index = 2;
			while ( index < len && (unsigned char)str1[index] >= 176)
			{
				index += 2;
			}
			if (flag)
			{
				str2 += pSegmentSentence(str1.substr(0, index), onum);
			}
			else 
			{
				str2 += nSegmentSentence(str1.substr(0, index), onum);
			}
			str1 = str1.substr(index, len - index);
		}
	}
}
void splitDoubleMM(string str1, string& str2)
{
	int cnum1 = 0,  cnum2 = 0;	 // 分词结果中 总共单词数目
	int snum1 = 0,	snum2 = 0;   // 分词结果中 单字单词数目
	int onum1 = 0,	onum2 = 0;   // 分词结果中 词典外单词数目
	float equlen1 = 0, equlen2 = 0; //分词结果中 平均词长
	float sumVariance1 = 0, sumVariance2 = 0;
	string strtmp1 = "";
	string strtmp2 = "";
	splitMM(str1, strtmp1,onum1, 1); //postive MM,  正向最大匹配算法
	splitMM(str1, strtmp2,onum2, 0); //negative MM, 逆向最大匹配算法

	string pattern = " ";
	vector<std::string> result1=split(strtmp1, pattern);
	cnum1 = result1.size();
	equlen1 = (float)str1.length()/cnum1;
	for (int i = 0; i < cnum1; ++i)
	{
		int len = result1[i].length();
		if ( len ==2 )
		{
			snum1++;
		}
		sumVariance1 += (len-equlen1)*(len-equlen1);
	}
	//sumVariance1 = (float)sumVariance1/cnum1;

	vector<std::string> result2=split(strtmp2, pattern);
	cnum2 = result2.size();
	equlen2 = (float)str1.length()/cnum2;
	for (int i = 0; i < cnum2; ++i)
	{
		int len = result2[i].length();
		if ( len ==2 )
		{
			snum2++;
		}
		sumVariance2 += (len-equlen2)*(len-equlen2);
	}
	//sumVariance2 = (float)sumVariance2/cnum2;
	float countnum1 = snum1*0.1 + cnum1*0.2 +  onum1*0.3 + equlen1*0.4 + sumVariance1 * 0.5;
	float countnum2 = snum2*0.1 + cnum2*0.2 +  onum2*0.3 + equlen2*0.4 + sumVariance2 * 0.5;
	//float countnum1 = snum1*0.1 + cnum1*0.2 +  onum1*0.3 + equlen1*0.4 ;
	//float countnum2 = snum2*0.1 + cnum2*0.2 +  onum2*0.3 + equlen2*0.4 ;
	//float countnum1 = sumVariance1 ;
	//float countnum2 = sumVariance1 ;
	str2 = (countnum1 < countnum2)?strtmp1 : strtmp2 ;
}