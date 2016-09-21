// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
// Windows ��� ����:
#include <windows.h>
#include <windowsx.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <map>

using namespace std;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "eSprite.h"
#include "ObjectType.h"
#include "ScreenDib.h"
#include "SpriteDib.h"
#include "PacketDefine.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"
#include "StreamQueue.h"
#include "NPacket.h"
#include "Network.h"
#include "FrameSkip.h"
#include "CMap.h"

#define Objects			map<DWORD, CBaseObject *>
#define ObjectsIter		Objects::iterator

extern CAyaStreamSQ SendQ;
extern CAyaStreamSQ RecvQ;

extern Objects g_Object;
extern CBaseObject *g_pPlayerObject;

extern CMap g_cTileMap;