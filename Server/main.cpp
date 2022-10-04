#include "Server.h"

#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#else

#endif    



int main()
{
	Server server(60000);
	return 0;
}