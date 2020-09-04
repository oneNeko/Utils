#pragma once
/* 该部分为pch.h或者stdafx.h 里的，wmi需要com支持
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 的一些常见且经常可放心忽略的隐藏警告消息
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC 支持功能区和控制条
*/


#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")
#define _AFX_ALL_WARNINGS
#include <locale>
#include <exception>

std::wstring StringToWstring(const std::string str)
{// string转wstring
	unsigned len = str.size() * 2;// 预留字节数
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t* p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	size_t pReturnValue = 0;
	mbstowcs_s(&pReturnValue, p, len, str.c_str(), len);// 转换
	std::wstring str1(p);
	delete[] p;// 释放申请的内存
	return str1;
}

wchar_t * char2wchar(const char* cchar)
{
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

int GetHardwareInfo(std::string query, std::string attribute, char* result, int* erro_code) {
	*erro_code = 0;
	HRESULT hres;

	// Initialize COM.
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		/*cout << "Failed to initialize COM library. "
		<< "Error code = 0x"
		<< hex << hres << endl;*/
		return 1;              // Program has failed.
	}

	// Initialize 
	hres = CoInitializeSecurity(
		NULL,
		-1,      // COM negotiates service                  
		NULL,    // Authentication services
		NULL,    // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,    // authentication
		RPC_C_IMP_LEVEL_IMPERSONATE,  // Impersonation
		NULL,             // Authentication info 
		EOAC_NONE,        // Additional capabilities
		NULL              // Reserved
	);
	if (FAILED(hres))
	{
		/*cout << "Failed to initialize security. "
		<< "Error code = 0x"
		<< hex << hres << endl;*/
		CoUninitialize();
		return 1;          // Program has failed.
	}

	// Obtain the initial locator to Windows Management
	// on a particular host computer.
	IWbemLocator *pLoc = 0;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		/*cout << "Failed to create IWbemLocator object. "
		<< "Error code = 0x"
		<< hex << hres << endl;*/
		CoUninitialize();
		return 1;       // Program has failed.
	}

	IWbemServices *pSvc = 0;

	// Connect to the root\cimv2 namespace with the
	// current user and obtain pointer pSvc
	// to make IWbemServices calls.

	hres = pLoc->ConnectServer(

		_bstr_t(L"ROOT\\CIMV2"), // WMI namespace
		NULL,                    // User name
		NULL,                    // User password
		0,                       // Locale
		NULL,                    // Security flags                 
		0,                       // Authority       
		0,                       // Context object
		&pSvc                    // IWbemServices proxy
	);

	if (FAILED(hres))
	{
		/*cout << "Could not connect. Error code = 0x"
		<< hex << hres << endl;*/
		pLoc->Release();
		CoUninitialize();
		return 1;                // Program has failed.
	}

	//cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;

	// Set the IWbemServices proxy so that impersonation
	// of the user (client) occurs.
	hres = CoSetProxyBlanket(

		pSvc,                         // the proxy to set
		RPC_C_AUTHN_WINNT,            // authentication service
		RPC_C_AUTHZ_NONE,             // authorization service
		NULL,                         // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,       // authentication level
		RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
		NULL,                         // client identity 
		EOAC_NONE                     // proxy capabilities     
	);

	if (FAILED(hres))
	{
		//cout << "Could not set proxy blanket. Error code = 0x"
		//<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;               // Program has failed.
	}

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		//bstr_t("SELECT * FROM Win32_OperatingSystem"), //此处wql语句更改，实现不同的查询
		//bstr_t("select * from Win32_ComputerSystemProduct"),
		bstr_t(query.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		/*cout << "Query for processes failed. "
		<< "Error code = 0x"
		<< hex << hres << endl;*/
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;               // Program has failed.
	}
	else
	{
		IWbemClassObject *pclsObj;
		ULONG uReturn = 0;

		while (pEnumerator)
		{
			hres = pEnumerator->Next(WBEM_INFINITE, 1,
				&pclsObj, &uReturn);

			if (0 == uReturn)
			{
				break;
			}

			VARIANT vtProp;

			// Get the value of the Name property
			hres = pclsObj->Get(char2wchar(attribute.c_str()), 0, &vtProp, 0, 0);//此处把Name改成对应的要获取的属性值
																				 //wcout << "Manufacturer Name : " << vtProp.bstrVal << endl;
			if (vtProp.vt == VT_EMPTY || vtProp.vt == VT_NULL || vtProp.vt != VT_BSTR) {
				*erro_code = 2;
			}
			else {
				USES_CONVERSION;
				std::string aa(W2A(vtProp.bstrVal));
				strcpy_s(result, 128, aa.c_str());
			}
			VariantClear(&vtProp);
		}

	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	CoUninitialize();

	return 0;
}
