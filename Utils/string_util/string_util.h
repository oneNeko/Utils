#pragma once
#include<cstring>
#include<string>

namespace Utils {

	//字符串转换
	//string 转 const char*
	const char* String2ConstChar(std::string source) {
		return source.c_str();
	}

	//const char* 转 string
	std::string ConstChar2String(const char* source) {
		std::string dest = std::string(source);
		return dest;
	}

	//const char* 转 char*
	char* ConstChar2Char(const char* source) {
		int nlength = strlen(source);
		char* dest = new char(nlength + 1);
		strcpy_s(dest, nlength, source);
	}

	//char* 转const char*
	//直接赋值
}