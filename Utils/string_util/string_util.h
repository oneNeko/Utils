#pragma once
#include<cstring>
#include<string>

namespace Utils {

	//�ַ���ת��
	//string ת const char*
	const char* String2ConstChar(std::string source) {
		return source.c_str();
	}

	//const char* ת string
	std::string ConstChar2String(const char* source) {
		std::string dest = std::string(source);
		return dest;
	}

	//const char* ת char*
	char* ConstChar2Char(const char* source) {
		int nlength = strlen(source);
		char* dest = new char(nlength + 1);
		strcpy_s(dest, nlength, source);
	}

	//char* תconst char*
	//ֱ�Ӹ�ֵ
}