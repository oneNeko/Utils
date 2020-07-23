#pragma once
#include <string>
#include <windows.h>
namespace uuid {
	BOOL getBoisIDByCmd(char* lpszBaseBoard) {
		const long MAX_COMMAND_SIZE = 1000; // 命令行输出缓冲大小	

		WCHAR szFetCmd[] = L"wmic csproduct get UUID"; // 获取BOIS命令行	
		const std::string strEnSearch = "UUID"; // 主板序列号的前导信息

		BOOL   bret = FALSE;
		HANDLE hReadPipe = NULL; //读取管道
		HANDLE hWritePipe = NULL; //写入管道	
		PROCESS_INFORMATION pi; //进程信息	
		memset(&pi, 0, sizeof(pi));
		STARTUPINFO	si;	//控制命令行窗口信息
		memset(&si, 0, sizeof(si));
		SECURITY_ATTRIBUTES sa; //安全属性
		memset(&sa, 0, sizeof(sa));

		char szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // 放置命令行结果的输出缓冲区
		std::string	strBuffer;
		unsigned long count = 0;
		long ipos = 0;

		pi.hProcess = NULL;
		pi.hThread = NULL;
		si.cb = sizeof(STARTUPINFO);
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		//1.创建管道
		bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
		if (!bret) {
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);

			return bret;
		}

		//2.设置命令行窗口的信息为指定的读写管道
		GetStartupInfo(&si);
		si.hStdError = hWritePipe;
		si.hStdOutput = hWritePipe;
		si.wShowWindow = SW_HIDE; //隐藏命令行窗口
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		//3.创建获取命令行的进程
		bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		if (!bret) {
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			return bret;
		}

		//4.读取返回的数据
		int fileSize = GetFileSize(hReadPipe, NULL);
		WaitForSingleObject(pi.hProcess, 200);
		int i = 0;
		for (i = 0; i < 5 ;i++) {
			bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);

			if (!bret) {
				CloseHandle(hWritePipe);
				CloseHandle(hReadPipe);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				return bret;
			}
			
			//处理cmd两行堆积
			if (strstr(szBuffer, "UUID") != NULL) {
				szBuffer[3] = '#';
			}

			//处理多余字符
			int m_cnt = 0;
			char m_str[1024] = { 0 };
			for (int i = 0; i < sizeof(szBuffer); i++) {
				if ((szBuffer[i] <= 'F' && szBuffer[i] >= 'A') || (szBuffer[i] <= '9' && szBuffer[i] >= '0') || szBuffer[i] == '-') {
					m_str[m_cnt] = szBuffer[i];
					m_cnt++;
				}
			}

			//检查UUID格式
			if (strlen(m_str) == 36 && m_str[8]=='-' && m_str[13]=='-' && m_str[18]=='-'&& m_str[23]=='-') {
				strcpy(lpszBaseBoard, m_str);
				break;
			}
		}

		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (i >= 5) {
			return FALSE;
		}
		return TRUE;
	}
}