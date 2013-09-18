#pragma once


#include <iostream>
using namespace std;

class CSockAudiosw
{

	#define MAJOR_VERSION 1
	#define MINOR_VERSION 2
	#define WSA_MAKEWORD(x,y)  ((y)*256+(x))
	#define SERVER_PORT 9877

public:
	CSockAudiosw(void);
	~CSockAudiosw(void);

	int SendMessage(string ip, string content) const;
};
