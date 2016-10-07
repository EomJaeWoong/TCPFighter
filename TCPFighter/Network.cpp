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

/*----------------------------------------------------------------------------*/
// Receive Packet

//------------------------------------------------------------------------------
// Ŭ���̾�Ʈ �ڽ��� ĳ���� �Ҵ�
//------------------------------------------------------------------------------
BOOL recvProc_CreateMyCharacter(CNPacket *pPacket)
{
	pair<ObjectsIter, BOOL> mResult;

	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;
	BYTE			HP;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;
	*pPacket >> HP;

	g_pPlayerObject = new CPlayerObject(TRUE, ID, eTYPE_PLAYER, HP, Direction);
	g_pPlayerObject->SetPosition(X, Y);

	mResult = g_Object.insert(pair<DWORD, CBaseObject *>(ID, g_pPlayerObject));

	return TRUE;
}

//------------------------------------------------------------------------------
// �ٸ� Ŭ���̾�Ʈ�� ĳ���� ����
//------------------------------------------------------------------------------
BOOL recvProc_CreateOtherCharacter(CNPacket *pPacket)
{
	pair<ObjectsIter, BOOL> mResult;
	CPlayerObject *pPlayerObject;

	unsigned int	ID;
	BYTE			Direction;
	short		X;
	short		Y;
	BYTE			HP;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;
	*pPacket >> HP;

	pPlayerObject = new CPlayerObject(FALSE, ID, eTYPE_PLAYER, HP, Direction);
	pPlayerObject->SetPosition(X, Y);

	mResult = g_Object.insert(pair<DWORD, CBaseObject *>(ID, pPlayerObject));

	return mResult.second;
}

//------------------------------------------------------------------------------
// ĳ���� ����
//------------------------------------------------------------------------------
BOOL recvProc_DeleteCharacter(CNPacket *pPacket)
{
	ObjectsIter oIter;
	unsigned int	ID;

	*pPacket >> ID;

	oIter = g_Object.find(ID);
	
	if (oIter != g_Object.end())
	{
		delete oIter->second;
		g_Object.erase(ID);

		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ĳ���� �̵�����
//------------------------------------------------------------------------------
BOOL recvProc_MoveStart(CNPacket *pPacket)
{
	ObjectsIter		oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	oIter = g_Object.find(ID);

	if (oIter != g_Object.end())
	{
		oIter->second->ActionInput(Direction);
		oIter->second->SetPosition(X, Y);

		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ĳ���� �̵�����
//------------------------------------------------------------------------------
BOOL recvProc_MoveStop(CNPacket *pPacket)
{
	ObjectsIter		oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	oIter = g_Object.find(ID);

	if (oIter != g_Object.end())
	{
		oIter->second->ActionInput(dfACTION_STAND);
		((CPlayerObject *)oIter->second)->SetDirection(Direction);
		oIter->second->SetPosition(X, Y);

		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ĳ���� ����1
//------------------------------------------------------------------------------
BOOL recvProc_Attack1(CNPacket *pPacket)
{
	ObjectsIter		oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	oIter = g_Object.find(ID);

	if (oIter != g_Object.end())
	{
		oIter->second->ActionInput(dfACTION_ATTACK1);
		oIter->second->SetPosition(X, Y);

		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ĳ���� ����2
//------------------------------------------------------------------------------
BOOL recvProc_Attack2(CNPacket *pPacket)
{
	ObjectsIter		oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	oIter = g_Object.find(ID);

	if (oIter != g_Object.end())
	{
		oIter->second->ActionInput(dfACTION_ATTACK2);
		oIter->second->SetPosition(X, Y);

		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ĳ���� ����3
//------------------------------------------------------------------------------
BOOL recvProc_Attack3(CNPacket *pPacket)
{
	ObjectsIter		oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	oIter = g_Object.find(ID);

	if (oIter != g_Object.end())
	{
		oIter->second->ActionInput(dfACTION_ATTACK3);
		oIter->second->SetPosition(X, Y);

		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ĳ���� ������
//------------------------------------------------------------------------------
BOOL recvProc_Damage(CNPacket *pPacket)
{
	ObjectsIter		DamageIter;
	unsigned int	AttackID;
	unsigned int	DamageID;
	BYTE			DamageHP;

	*pPacket >> AttackID;
	*pPacket >> DamageID;
	*pPacket >> DamageHP;

	//����Ʈ �ֱ�

	DamageIter = g_Object.find(DamageID);

	if (DamageIter != g_Object.end())
	{
		((CPlayerObject *)DamageIter->second)->SetHP(DamageHP);
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// ����ȭ(ĳ���� ��ǥ ����)
//------------------------------------------------------------------------------
BOOL recvProc_Sync(CNPacket *pPacket)
{
	ObjectsIter		oIter;
	unsigned int		ID;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> X;
	*pPacket >> Y;

	oIter = g_Object.find(ID);

	if (oIter != g_Object.end())
	{
		oIter->second->SetPosition(X, Y);
		return TRUE;
	}

	return FALSE;
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
// Send Packet

//------------------------------------------------------------------------------
// ĳ���� �̵� ����
//------------------------------------------------------------------------------
BOOL sendProc_MoveStart(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_MoveStart(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}

//------------------------------------------------------------------------------
// ĳ���� �̵� ����
//------------------------------------------------------------------------------
BOOL sendProc_MoveStop(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_MoveStop(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}

//------------------------------------------------------------------------------
// ĳ���� ����1
//------------------------------------------------------------------------------
BOOL sendProc_Attack1(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_Attack1(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}

//------------------------------------------------------------------------------
// ĳ���� ����2
//------------------------------------------------------------------------------
BOOL sendProc_Attack2(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_Attack2(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}

//------------------------------------------------------------------------------
// ĳ���� ����3
//------------------------------------------------------------------------------
BOOL sendProc_Attack3(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_Attack3(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
// Make Packet

//------------------------------------------------------------------------------
// ĳ���� �̵� ���� ��Ŷ
//------------------------------------------------------------------------------
int makePacket_MoveStart(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_MOVE_START;

	return pPacket->GetDataSize();
}

//------------------------------------------------------------------------------
// ĳ���� �̵� ���� ��Ŷ
//------------------------------------------------------------------------------
int makePacket_MoveStop(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_MOVE_STOP;

	return pPacket->GetDataSize();
}

//------------------------------------------------------------------------------
// ĳ���� ����1 ��Ŷ
//------------------------------------------------------------------------------
int makePacket_Attack1(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_ATTACK1;

	return pPacket->GetDataSize();
}

//------------------------------------------------------------------------------
// ĳ���� ����2 ��Ŷ
//------------------------------------------------------------------------------
int makePacket_Attack2(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_ATTACK2;

	return pPacket->GetDataSize();
}

//------------------------------------------------------------------------------
// ĳ���� ����3 ��Ŷ
//------------------------------------------------------------------------------
int makePacket_Attack3(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_ATTACK3;

	return pPacket->GetDataSize();
}
/*----------------------------------------------------------------------------*/