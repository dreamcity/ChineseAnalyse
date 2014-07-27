#include "dictionary.h"
# define MaxWordLength 10 // 最大词长为个字节（即个汉字）
# define Separator "/E " // 词界标记
CDictionary WordDic; //初始化一个词典
//对字符串用最大匹配法（正向或逆向）处理
string SegmentSentence(string s1)
{
	string s2 = ""; //用s2存放分词结果

	while(!s1.empty())
	{
		int len =(int) s1.length(); // 取输入串长度
		if (len > MaxWordLength) // 如果输入串长度大于最大词长
		{
			len = MaxWordLength; // 只在最大词长范围内进行处理
		}
		//string w = s1.substr(0, len); // （正向用）将输入串左边等于最大词长长度串取出作为候选词
		string w = s1.substr((s1.length() - len), len); //逆向用
		//*********************************************
		//while(len > 2)
		//{
		//	len -=2;
		//	w = s1.substr(s1.length() - len, len); //逆向用
		//}
		//w = w + Separator; // (逆向用)
		//s2 = w + s2 ; // (逆向用)
		//s1 = s1.substr(0, s1.length() - len); // (逆向用)
		//**********************************************************
		int n = WordDic.FindWord(w); // 在词典中查找相应的词
		while(len > 2 && n == 0) // 如果不是词
		{
			len -= 2; // 从候选词右边减掉一个汉字，将剩下的部分作为候选词
			//w = w.substr(0, len); //正向用
			w = s1.substr(s1.length() - len, len); //逆向用
			n = WordDic.FindWord(w);
		}
		//s2 += w + Separator; // (正向用）将匹配得到的词连同词界标记加到输出串末尾
		w = w + Separator; // (逆向用)
		s2 = w + s2 ; // (逆向用)
		//s1 = s1.substr(w.length(), s1.length()); //(正向用)从s1-w处开始
		s1 = s1.substr(0, s1.length() - len); // (逆向用)
		//**********************************************************
	}
	return s2;
}

//对句子进行最大匹配法处理，包含对特殊字符的处理
string SegmentSentenceMM (string s1)
{
	string s2 = ""; //用s2存放分词结果
	int i;
	int dd;
	while(!s1.empty() )
	{
		unsigned char ch = (unsigned char)s1[0];
		if (ch < 128) // 处理西文字符
		{
			i = 1;
			dd = (int)s1.length();
			while (i < dd && ((unsigned char)s1[i] < 128) && (s1[i] != 10) && (s1[i] != 13)) // s1[i]不能是换行符或回车符
			{
				i++;
			}
			if ((ch != 32) && (ch != 10) && (ch != 13)) // 如果不是西文空格或换行或回车符
			{
				s2 += s1.substr(0,i) + Separator;
			}
			else
			{
				//if (ch == 10 || ch == 13) // 如果是换行或回车符，将它拷贝给s2输出
				if (ch == 10 || ch == 13 || ch == 32) //谢谢读者mces89的指正
				{
					s2 += s1.substr(0, i);
				}
			}
			s1 = s1.substr(i,dd);
			continue;
		}
		else
		{
			if (ch < 176) // 中文标点等非汉字字符
			{
				i = 0;
				dd = (int)s1.length();
				while(i < dd && ((unsigned char)s1[i] < 176) && ((unsigned char)s1[i] >= 161)
				&& (!((unsigned char)s1[i] == 161 && ((unsigned char)s1[i+1] >= 162 && (unsigned char)s1[i+1] <= 168)))
				&& (!((unsigned char)s1[i] == 161 && ((unsigned char)s1[i+1] >= 171 && (unsigned char)s1[i+1] <= 191)))
				&& (!((unsigned char)s1[i] == 163 && ((unsigned char)s1[i+1] == 172 || (unsigned char)s1[i+1] == 161)
				|| (unsigned char)s1[i+1] == 168 || (unsigned char)s1[i+1] == 169 || (unsigned char)s1[i+1] == 186
				|| (unsigned char)s1[i+1] == 187 || (unsigned char)s1[i+1] == 191)))
				{
					i = i + 2; // 假定没有半个汉字
				}
				if (i == 0)
				{
					i = i + 2;
				}
				if (!(ch == 161 && (unsigned char)s1[1] == 161)) // 不处理中文空格
				{
					s2+=s1.substr(0, i) + Separator; // 其他的非汉字双字节字符可能连续输出
				}
				s1 = s1.substr(i, dd);
				continue;
			}
					// 以下处理汉字串
			i = 2;
			dd = (int)s1.length();
			while(i < dd && (unsigned char)s1[i] >= 176)
			{
				i += 2;
			}
			s2 += SegmentSentence(s1.substr(0, i));
			s1 = s1.substr(i,dd);
		}	
	}
	return s2;
}