#pragma once
#include <string>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <io.h>
#include <direct.h>
#include <ctime>
#include <locale>
#include <codecvt>
using namespace std;
namespace CLog {
	

	//ȥ��ĩβ��б��
	int path_rm_slash(string _source,string &_dest) {
		_dest = _source.substr(0, _source.find_last_of("\\"));
		return 0;
	}

	//��鴴��Ŀ¼
	int ckdir_util(string _path) {
		if (0 != _access(_path.c_str(), 0))
		{
			string m_path = "";
			path_rm_slash(_path, m_path);

			//�����б��·��
			if (m_path == _path) {
				_mkdir(_path.c_str());
				return 0;
			}

			//�����һ���Ƿ���ڣ��������ڣ���ݹ鴴����һ��Ŀ¼
			if (0 != _access(m_path.c_str(), 0)) {
				ckdir_util(m_path);
			}
			//��������Ŀ¼
			_mkdir(_path.c_str());
		}
		return 0;
	}

	//��ȡʱ��
	int get_time(string& detail_time,string& day_time) {
		struct tm* newTime = new tm();
		time_t      szClock;
		time(&szClock);
		localtime_s(newTime,&szClock);

		char m_time[64] = { 0 };
		
		sprintf_s(m_time, "%4d%02d%02d", \
			newTime->tm_year + 1900, newTime->tm_mon+1, newTime->tm_mday);
		day_time = m_time;

		sprintf_s(m_time, "%4d-%02d-%02d %02d:%02d:%02d", \
			newTime->tm_year+1900,newTime->tm_mon+1,newTime->tm_mday, \
			newTime->tm_hour,newTime->tm_min,newTime->tm_sec);	
		detail_time = m_time;

		return 0;
	}

	// �����locale��stringͷ�ļ���ʹ��setlocale������
	std::wstring StringToWstring(const std::string str)
	{// stringתwstring
		unsigned count = str.size() * 2;// Ԥ���ֽ���
		size_t pReturnValue = 0;
		setlocale(LC_CTYPE, "");     //������ô˺���
		wchar_t* wcstr = new wchar_t[count];// ����һ���ڴ���ת������ַ���
		mbstowcs_s(&pReturnValue, wcstr,strlen(str.c_str()), str.c_str(), count);// ת��
		std::wstring str1(wcstr);
		delete[] wcstr;// �ͷ�������ڴ�
		return str1;
	}

	// wstringתstring
	std::string WstringToString(const std::wstring str)
	{
		size_t count = str.size() * 4;
		size_t pReturnValue = 0;
		setlocale(LC_CTYPE, "");
		char* mbstr = new char[count];
		wcstombs_s(&pReturnValue, mbstr, strlen(mbstr),str.c_str(), count);
		std::string res(mbstr);
		delete[] mbstr;
		return res;
	}

	//
	int log(string _path, int _level, string _text){
		ckdir_util(_path.c_str());

		string m_detailTime = "";
		string m_dayTime = "";
		get_time(m_detailTime,m_dayTime);

		ofstream out(_path + "\\" + m_dayTime + ".log", ofstream::app);
		
		//תΪUTF-8��
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::string output = conv.to_bytes(StringToWstring(m_detailTime + " " + _text));
		
		out << output << endl;
		out.close();

		return 0;
	}

	//�ӿں���
	int LogOut(IN const char* _path, IN int _level, IN const char* _Format, ...) {
		const int BufSize = max_length;
		char szMsg[BufSize] = { 0 };

		va_list args; 	//��ʽ����Ϣ

		va_start(args, _Format);
		vsprintf_s(szMsg, max_length, _Format, args);	 //vsprintf_s BufSize - strlen(szMsg),
		va_end(args);

		log(string(_path), _level, string(szMsg));

		return 0;
	}
}