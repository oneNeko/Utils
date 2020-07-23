#include "uuid/uuid.h"
#include <iostream>
#include "log_util/log_util.h"
int main() {
	/*char m_uuid[1024] = { 0 };
	bool flg = Utils::getBoisIDByCmd(m_uuid);
	if(flg)
		std::cout << m_uuid;
	else {
		std::cout << "获取失败";
	}*/
	std::string str = "程序启动！";
	std::cout<<CLog::logout("File\\Log","1",str);
	return 0;
}