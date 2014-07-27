#include "dictionary.h"
# define MaxWordLength 10 // ���ʳ�Ϊ���ֽڣ��������֣�
# define Separator "/E " // �ʽ���
CDictionary WordDic; //��ʼ��һ���ʵ�
//���ַ��������ƥ�䷨����������򣩴���
string SegmentSentence(string s1)
{
	string s2 = ""; //��s2��ŷִʽ��

	while(!s1.empty())
	{
		int len =(int) s1.length(); // ȡ���봮����
		if (len > MaxWordLength) // ������봮���ȴ������ʳ�
		{
			len = MaxWordLength; // ֻ�����ʳ���Χ�ڽ��д���
		}
		//string w = s1.substr(0, len); // �������ã������봮��ߵ������ʳ����ȴ�ȡ����Ϊ��ѡ��
		string w = s1.substr((s1.length() - len), len); //������
		//*********************************************
		//while(len > 2)
		//{
		//	len -=2;
		//	w = s1.substr(s1.length() - len, len); //������
		//}
		//w = w + Separator; // (������)
		//s2 = w + s2 ; // (������)
		//s1 = s1.substr(0, s1.length() - len); // (������)
		//**********************************************************
		int n = WordDic.FindWord(w); // �ڴʵ��в�����Ӧ�Ĵ�
		while(len > 2 && n == 0) // ������Ǵ�
		{
			len -= 2; // �Ӻ�ѡ���ұ߼���һ�����֣���ʣ�µĲ�����Ϊ��ѡ��
			//w = w.substr(0, len); //������
			w = s1.substr(s1.length() - len, len); //������
			n = WordDic.FindWord(w);
		}
		//s2 += w + Separator; // (�����ã���ƥ��õ��Ĵ���ͬ�ʽ��Ǽӵ������ĩβ
		w = w + Separator; // (������)
		s2 = w + s2 ; // (������)
		//s1 = s1.substr(w.length(), s1.length()); //(������)��s1-w����ʼ
		s1 = s1.substr(0, s1.length() - len); // (������)
		//**********************************************************
	}
	return s2;
}

//�Ծ��ӽ������ƥ�䷨���������������ַ��Ĵ���
string SegmentSentenceMM (string s1)
{
	string s2 = ""; //��s2��ŷִʽ��
	int i;
	int dd;
	while(!s1.empty() )
	{
		unsigned char ch = (unsigned char)s1[0];
		if (ch < 128) // ���������ַ�
		{
			i = 1;
			dd = (int)s1.length();
			while (i < dd && ((unsigned char)s1[i] < 128) && (s1[i] != 10) && (s1[i] != 13)) // s1[i]�����ǻ��з���س���
			{
				i++;
			}
			if ((ch != 32) && (ch != 10) && (ch != 13)) // ����������Ŀո���л�س���
			{
				s2 += s1.substr(0,i) + Separator;
			}
			else
			{
				//if (ch == 10 || ch == 13) // ����ǻ��л�س���������������s2���
				if (ch == 10 || ch == 13 || ch == 32) //лл����mces89��ָ��
				{
					s2 += s1.substr(0, i);
				}
			}
			s1 = s1.substr(i,dd);
			continue;
		}
		else
		{
			if (ch < 176) // ���ı��ȷǺ����ַ�
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
					i = i + 2; // �ٶ�û�а������
				}
				if (i == 0)
				{
					i = i + 2;
				}
				if (!(ch == 161 && (unsigned char)s1[1] == 161)) // ���������Ŀո�
				{
					s2+=s1.substr(0, i) + Separator; // �����ķǺ���˫�ֽ��ַ������������
				}
				s1 = s1.substr(i, dd);
				continue;
			}
					// ���´����ִ�
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