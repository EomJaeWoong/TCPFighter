#include "stdafx.h"
#include "Network.h"

BOOL InitialNetwork(SOCKET *sock, HWND *hWnd)
{
	int retval;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return FALSE;

	*sock = socket(AF_INET, SOCK_STREAM, 0);
	if (*sock == INVALID_SOCKET)
		return FALSE;

	retval = WSAAsyncSelect(*sock, *hWnd, WM_NETWORK, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE);
	if (retval == SOCKET_ERROR)
		return FALSE;

	SOCKADDR_IN sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(5000);
	InetPton(AF_INET, L"127.0.0.1", &sockaddr.sin_addr.s_addr);

	retval = connect(*sock, (SOCKADDR *)&sockaddr, sizeof(sockaddr));
	if (retval == SOCKET_ERROR)
	{
		retval = WSAGetLastError();
		if (retval != WSAEWOULDBLOCK)
			return FALSE;
	}

	return TRUE;
}
