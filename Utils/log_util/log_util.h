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
namespace CLog {
	using namespace std;

	//去除末尾反斜杠
	int path_rm_slash(string _source,string &_dest) {
		_dest = _source.substr(0, _source.find_last_of("\\"));
		return 0;
	}

	//检查创建目录
	int ckdir_util(string _path) {
		if (0 != _access(_path.c_str(), 0))
		{
			string m_path = "";
			path_rm_slash(_path, m_path);

			//检查无斜杠路径
			if (m_path == _path) {
				_mkdir(_path.c_str());
				return 0;
			}

			//检查上一级是否存在，若不存在，则递归创建上一级目录
			if (0 != _access(m_path.c_str(), 0)) {
				ckdir_util(m_path);
			}
			//创建本级目录
			_mkdir(_path.c_str());
		}
		return 0;
	}

	int get_time(string& detail_time,string& day_time) {
		struct tm* newTime;
		time_t      szClock;
		time(&szClock);
		newTime = localtime(&szClock);

		char m_time[64] = { 0 };
		
		sprintf(m_time, "%4d%02d%02d", \
			newTime->tm_year + 1900, newTime->tm_mon+1, newTime->tm_mday);
		day_time = m_time;

		sprintf(m_time, "%4d-%02d-%02d %02d:%02d:%02d", \
			newTime->tm_year+1900,newTime->tm_mon+1,newTime->tm_mday, \
			newTime->tm_hour,newTime->tm_min,newTime->tm_sec);	
		detail_time = m_time;

		return 0;
	}

	// 需包含locale、string头文件、使用setlocale函数。
	std::wstring StringToWstring(const std::string str)
	{// string转wstring
		unsigned len = str.size() * 2;// 预留字节数
		setlocale(LC_CTYPE, "");     //必须调用此函数
		wchar_t* p = new wchar_t[len];// 申请一段内存存放转换后的字符串
		mbstowcs(p, str.c_str(), len);// 转换
		std::wstring str1(p);
		delete[] p;// 释放申请的内存
		return str1;
	}

	std::string WstringToString(const std::wstring str)
	{// wstring转string
		unsigned len = str.size() * 4;
		setlocale(LC_CTYPE, "");
		char* p = new char[len];
		wcstombs(p, str.c_str(), len);
		std::string str1(p);
		delete[] p;
		return str1;
	}

	int logout(string _path, string _level, string _text){
		ckdir_util(_path.c_str());

		string m_detailTime = "";
		string m_dayTime = "";
		get_time(m_detailTime,m_dayTime);

		ofstream out(_path + "\\" + m_dayTime + ".log", ofstream::app);
		
		//转为UTF-8流
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::string output = conv.to_bytes(StringToWstring(m_detailTime + " " + _text));
		
		out << output << endl;
		out.close();

		return 0;
	}

	int sslogout(const char* _Format,...) {
		const int BufSize = 2048;
		char szMsg[BufSize];

		va_list args; 	//格式化消息

		va_start(args, _Format);
		vsprintf(szMsg, _Format, args);	 //vsprintf_s BufSize - strlen(szMsg),
		va_end(args);

		return 0;
	}
}