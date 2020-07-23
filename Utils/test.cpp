#include "uuid/uuid.h"
#include <iostream>
int main() {
	char m_uuid[1024] = { 0 };
	bool flg = uuid::getBoisIDByCmd(m_uuid);
	if(flg)
		std::cout << m_uuid;
	else {
		std::cout << "»ñÈ¡Ê§°Ü";
	}
	return 0;
}