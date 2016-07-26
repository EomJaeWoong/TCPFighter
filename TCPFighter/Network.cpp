#include "stdafx.h"

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

int MakePacket_MoveStart(st_NETWORK_PACKET_HEADER *pHeader, char *pPacket,
	int iDir, int iX, int iY)
{
	stPACKET_CS_MOVE_START *stPacket = (stPACKET_CS_MOVE_START*)pPacket;

	stPacket->Direction = (BYTE)iDir;
	stPacket->X = (WORD)iX;
	stPacket->Y = (WORD)iY;
	stPacket->Endcode = dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = sizeof(*stPacket);
	pHeader->byType = dfPACKET_CS_MOVE_START;

	return sizeof(*pHeader) + sizeof(*stPacket);
}