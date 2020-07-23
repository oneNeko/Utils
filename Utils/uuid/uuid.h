#pragma once
#include <string>
#include <windows.h>
namespace uuid {
	BOOL getBoisIDByCmd(char* lpszBaseBoard) {
		const long MAX_COMMAND_SIZE = 1000; // ��������������С	

		WCHAR szFetCmd[] = L"wmic csproduct get UUID"; // ��ȡBOIS������	
		const std::string strEnSearch = "UUID"; // �������кŵ�ǰ����Ϣ

		BOOL   bret = FALSE;
		HANDLE hReadPipe = NULL; //��ȡ�ܵ�
		HANDLE hWritePipe = NULL; //д��ܵ�	
		PROCESS_INFORMATION pi; //������Ϣ	
		memset(&pi, 0, sizeof(pi));
		STARTUPINFO	si;	//���������д�����Ϣ
		memset(&si, 0, sizeof(si));
		SECURITY_ATTRIBUTES sa; //��ȫ����
		memset(&sa, 0, sizeof(sa));

		char szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
		std::string	strBuffer;
		unsigned long count = 0;
		long ipos = 0;

		pi.hProcess = NULL;
		pi.hThread = NULL;
		si.cb = sizeof(STARTUPINFO);
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		//1.�����ܵ�
		bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
		if (!bret) {
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);

			return bret;
		}

		//2.���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
		GetStartupInfo(&si);
		si.hStdError = hWritePipe;
		si.hStdOutput = hWritePipe;
		si.wShowWindow = SW_HIDE; //���������д���
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		//3.������ȡ�����еĽ���
		bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		if (!bret) {
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			return bret;
		}

		//4.��ȡ���ص�����
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
			
			//����cmd���жѻ�
			if (strstr(szBuffer, "UUID") != NULL) {
				szBuffer[3] = '#';
			}

			//��������ַ�
			int m_cnt = 0;
			char m_str[1024] = { 0 };
			for (int i = 0; i < sizeof(szBuffer); i++) {
				if ((szBuffer[i] <= 'F' && szBuffer[i] >= 'A') || (szBuffer[i] <= '9' && szBuffer[i] >= '0') || szBuffer[i] == '-') {
					m_str[m_cnt] = szBuffer[i];
					m_cnt++;
				}
			}

			//���UUID��ʽ
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