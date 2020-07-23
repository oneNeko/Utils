#pragma once
#include<afx.h>
#include<string>

namespace Utils {

	//cstring ת string
	std::string Cstring2String(CString source) {

#ifdef _UNICODE

		USES_CONVERSION;
		std::string dest(W2A(source));
		return dest;

#else

		std::string dest(source.GetBuffer());
		source.ReleaseBuffer();
		return dest;

#endif
	}

	//string ת cstring
	CString String2CString(std::string source) {

#ifdef _UNICODE

		USES_CONVERSION;
		CString dest(source.c_str());
		return dest;

#else

		CString dest;
		dest.Format("%s", source.c_str());
		return dest;

#endif
	}
}