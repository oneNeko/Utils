#include "uuid/uuid.h"
#include <iostream>
#include "log_util/log_util.h"
int main() {
	/*char m_uuid[1024] = { 0 };
	bool flg = Utils::getBoisIDByCmd(m_uuid);
	if(flg)
		std::cout << m_uuid;
	else {
		std::cout << "��ȡʧ��";
	}*/
	std::string str = "����������";
	std::string path = "File\\Log";
	std::string ip = "192.168.16.23";
	for (int i = 0; i < 100; i++) {
		CLog::LogOut(path.c_str(), 1, "����������ip=%s,data=%d", ip.c_str(), 12);
	}
	return 0;
}