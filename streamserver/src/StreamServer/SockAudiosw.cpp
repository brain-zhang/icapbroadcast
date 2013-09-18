#include "stdafx.h"
#include <winsock2.h>
#include "SockAudiosw.h"
#include "SSExcept.h"
#include "INISet.h"

CSockAudiosw::CSockAudiosw(void)
{
	LPWSADATA lpmyWSAData;
	WORD   VersionReqd;
	VersionReqd = WSA_MAKEWORD(MAJOR_VERSION,MINOR_VERSION);
	lpmyWSAData = (LPWSADATA)malloc(sizeof(WSADATA));
	int Status = WSAStartup(VersionReqd, lpmyWSAData);
	if (Status != 0)
	{
		throw CAudioswExcept(INIT_NET_ERR);
	}
}

CSockAudiosw::~CSockAudiosw(void)
{
	WSACleanup();
}

int CSockAudiosw::SendMessage(string ip, string content) const
{
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(SERVER_PORT);
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

	//建立socket接口
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(s == INVALID_SOCKET)
	{
		throw CAudioswExcept(CREATE_SOCKET_ERR);
		return 1;
	}

	/*
	int nTimeout = 5000;  
	//设置发送超时为1000ms  
	::setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, (char *)&nTimeout, sizeof(int));

	//设置接收超时为1000ms  
	::setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeout, sizeof(int));

	//设置非阻塞方式连接
	unsigned   long   ul   =   1;
	::ioctlsocket(s,   FIONBIO,   (unsigned   long*)&ul);
	*/
	if(::connect(s, (sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
	{
		string strErr = string(CONNECT_NET_DEVICE_ERR);
		strErr += ip;
		throw CAudioswExcept(strErr);
		return 1;
	}

	::send(s, content.c_str(), content.length(), 0);
	::closesocket(s);
	return 0;
}