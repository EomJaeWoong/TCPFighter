// TCPFighter.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "TCPFighter.h"
#include "ScreenDib.h"
#include "SpriteDib.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"
#include "FrameSkip.h"

#define MAX_OBJECT 100

// 전역 변수:
HINSTANCE hInst;	// 현재 인스턴스입니다.
HWND g_hWnd;
CBaseObject *g_pPlayerObject;
CBaseObject *gObject[MAX_OBJECT];
CScreenDib g_cScreenDib(640, 480, 32);
CSpriteDib *g_pSpriteDib;
CFrameSkip g_FrameSkip(50);
SOCKET client_sock;
BOOL g_bActiveApp;
CAyaStreamSQ SendQ;
CAyaStreamSQ RecvQ;
int retval; 

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void InitialGame();
void Update_Game(void);
void KeyProcess();
void Action();
void Draw();
void ConnectProc();
void ReadProc();
void WriteProc();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TCPFIGHTER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TCPFIGHTER);
	wcex.lpszClassName = L"TCPFighter";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindow(L"TCPFighter", L"TCPFighter", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 게임, 네트워크 초기화
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	InitialGame();
	InitialNetwork(&client_sock, &g_hWnd);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 윈도우 사이즈 맞추기
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetFocus(g_hWnd);

	RECT WindowRect;
	WindowRect.top = 0;
	WindowRect.left = 0;
	WindowRect.right = 640;
	WindowRect.bottom = 480;

	AdjustWindowRectEx(&WindowRect, GetWindowStyle(g_hWnd), GetMenu(g_hWnd) != NULL, GetWindowExStyle(g_hWnd));

	int iX = (GetSystemMetrics(SM_CXSCREEN) - 640) / 2;
	int iY = (GetSystemMetrics(SM_CYSCREEN) - 480) / 2;

	MoveWindow(g_hWnd, iX, iY, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, TRUE);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 기본 메시지 루프입니다.
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			Update_Game();
		}
	}

	return (int) msg.wParam;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	// 프로그램 비활성화 체크
	case WM_ACTIVATEAPP :
		g_bActiveApp = (BOOL)wParam;
		break;
		
	//네트워크 메시지
	case WM_NETWORK :
		if (WSAGETASYNCERROR(lParam))
			MessageBox(g_hWnd, L"WM_NETWORK", NULL, NULL);

		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT :
			ConnectProc();
			break;

		case FD_CLOSE :
			MessageBox(g_hWnd, L"close", NULL, NULL);
			break;

		case FD_READ :
			ReadProc();
			break;

		case FD_WRITE :
			WriteProc();
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 게임 초기화
// 스프라이트 생성과 로드 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitialGame()
{
	g_pSpriteDib = new CSpriteDib(eSPRITE_MAX, 0x00ffffff);

	g_pSpriteDib->LoadDibSprite(eMAP, L"Data\\_Map.bmp", 0, 0);

	// 맵 추가
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L01, L"Data\\Stand_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L02, L"Data\\Stand_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L03, L"Data\\Stand_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L04, L"Data\\Stand_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L05, L"Data\\Stand_L_01.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R01, L"Data\\Stand_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R02, L"Data\\Stand_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R03, L"Data\\Stand_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R04, L"Data\\Stand_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R05, L"Data\\Stand_R_01.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L01, L"Data\\MOVE_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L02, L"Data\\MOVE_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L03, L"Data\\MOVE_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L04, L"Data\\MOVE_L_04.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L05, L"Data\\MOVE_L_05.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L06, L"Data\\MOVE_L_06.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L07, L"Data\\MOVE_L_07.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L08, L"Data\\MOVE_L_08.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L09, L"Data\\MOVE_L_09.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L10, L"Data\\MOVE_L_10.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L11, L"Data\\MOVE_L_11.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L12, L"Data\\MOVE_L_12.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R01, L"Data\\MOVE_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R02, L"Data\\MOVE_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R03, L"Data\\MOVE_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R04, L"Data\\MOVE_R_04.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R05, L"Data\\MOVE_R_05.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R06, L"Data\\MOVE_R_06.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R07, L"Data\\MOVE_R_07.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R08, L"Data\\MOVE_R_08.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R09, L"Data\\MOVE_R_09.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R10, L"Data\\MOVE_R_10.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R11, L"Data\\MOVE_R_11.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R12, L"Data\\MOVE_R_12.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_L01, L"Data\\Attack1_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_L02, L"Data\\Attack1_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_L03, L"Data\\Attack1_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_L04, L"Data\\Attack1_L_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_R01, L"Data\\Attack1_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_R02, L"Data\\Attack1_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_R03, L"Data\\Attack1_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_R04, L"Data\\Attack1_R_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_L01, L"Data\\Attack2_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_L02, L"Data\\Attack2_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_L03, L"Data\\Attack2_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_L04, L"Data\\Attack2_L_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_R01, L"Data\\Attack2_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_R02, L"Data\\Attack2_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_R03, L"Data\\Attack2_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_R04, L"Data\\Attack2_R_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_L01, L"Data\\Attack3_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_L02, L"Data\\Attack3_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_L03, L"Data\\Attack3_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_L04, L"Data\\Attack3_L_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_R01, L"Data\\Attack3_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_R02, L"Data\\Attack3_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_R03, L"Data\\Attack3_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_R04, L"Data\\Attack3_R_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(eEFFECT_SPARK_01, L"Data\\xSpark_1.bmp", 70, 70);
	g_pSpriteDib->LoadDibSprite(eEFFECT_SPARK_02, L"Data\\xSpark_2.bmp", 70, 70);
	g_pSpriteDib->LoadDibSprite(eEFFECT_SPARK_03, L"Data\\xSpark_3.bmp", 70, 70);
	g_pSpriteDib->LoadDibSprite(eEFFECT_SPARK_04, L"Data\\xSpark_4.bmp", 70, 70);
	
	g_pSpriteDib->LoadDibSprite(eGUAGE_HP, L"Data\\HPGuage.bmp", 0, 0);

	g_pSpriteDib->LoadDibSprite(eSHADOW, L"Data\\Shadow.bmp", 32, 4);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 게임 루프
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update_Game(void)
{
	if (g_bActiveApp)
		KeyProcess();

	Action();
	WriteProc();

	if (g_FrameSkip.FrameSkip())
	{
		Draw();
		g_cScreenDib.DrawBuffer(g_hWnd);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 키 처리
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void KeyProcess()
{
	DWORD dwAction = dfACTION_STAND;

	if (g_pPlayerObject == NULL)	return;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		dwAction = dfACTION_MOVE_LL;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	dwAction = dfACTION_MOVE_RR;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		dwAction = dfACTION_MOVE_UU;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		dwAction = dfACTION_MOVE_DD;

	if ((GetAsyncKeyState(VK_LEFT) & 0x8001) && (GetAsyncKeyState(VK_UP) & 0x8001))
		dwAction = dfACTION_MOVE_LU;
	if ((GetAsyncKeyState(VK_LEFT) & 0x8001) && (GetAsyncKeyState(VK_DOWN) & 0x8001))
		dwAction = dfACTION_MOVE_LD;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8001) && (GetAsyncKeyState(VK_UP) & 0x8001))
		dwAction = dfACTION_MOVE_RU;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8001) && (GetAsyncKeyState(VK_DOWN) & 0x8001))
		dwAction = dfACTION_MOVE_RD;

	if (GetAsyncKeyState(0x5A) & 0x8001 || GetAsyncKeyState(0x5A) & 1)
		dwAction = dfACTION_ATTACK1;
	if (GetAsyncKeyState(0x58) & 0x8001 || GetAsyncKeyState(0x58) & 1)
		dwAction = dfACTION_ATTACK2;
	if (GetAsyncKeyState(0x43) & 0x8001 || GetAsyncKeyState(0x43) & 1)
		dwAction = dfACTION_ATTACK3;

	g_pPlayerObject->ActionInput(dwAction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Action
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Action()
{
	for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
	{
		if (gObject[iCnt] != NULL)
			gObject[iCnt]->Action(1);
	}

	//y축 정렬 시키기
	//이펙트는 제일 뒤로 밀기
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Draw()
{
	BYTE *bypDest = g_cScreenDib.GetDibBuffer();
	int iDestWidth = g_cScreenDib.GetWidth();
	int iDestHeight = g_cScreenDib.GetHeight();
	int iDestPitch = g_cScreenDib.GetPitch();

	g_pSpriteDib->DrawSprite(eMAP, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);

	for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
	{
		if (gObject[iCnt] != NULL)
			gObject[iCnt]->Draw(g_pSpriteDib, bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 서버 접속 완료후 실행
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConnectProc()
{
	stPACKET_SC_CREATE_MY_CHARACTER packet;
	while (1){
		retval = recv(client_sock, (char *)&packet, sizeof(stPACKET_SC_CREATE_MY_CHARACTER), 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return;
			}
		}

		if (packet.Header.byCode == (BYTE)0x89 && packet.Header.byType == dfPACKET_SC_CREATE_MY_CHARACTER
			)
		{
			g_pPlayerObject = new CPlayerObject(TRUE, packet.ID, eTYPE_PLAYER, packet.HP, packet.Direction);
			g_pPlayerObject->SetPosition(packet.X, packet.Y);
			gObject[0] = g_pPlayerObject;
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Packet 읽기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void ReadProc()
{
	st_NETWORK_PACKET_HEADER Header;
	retval = recv(client_sock, RecvQ.GetWriteBufferPtr(), RecvQ.GetNotBrokenPutSize(), 0);

	if (retval == 0)
		return;

	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			return;
		else
			MessageBox(g_hWnd, L"ReadProc", NULL, NULL);
	}
	RecvQ.MoveWritePos(retval);

	while (1)
	{
		if (RecvQ.GetUseSize() < sizeof(Header))
			return;

		RecvQ.Peek((char *)&Header, sizeof(Header));

		if (Header.byCode != (BYTE)0x89)
			return;

		if (RecvQ.GetUseSize() < Header.bySize + sizeof(Header))
			return;

		RecvQ.RemoveData(sizeof(Header));

		switch (Header.byType)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 다른 유저 생성
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
		case dfPACKET_SC_CREATE_OTHER_CHARACTER :
			stPACKET_SC_CREATE_OTHER_CHARACTER stPacketCreateCharacter;
			RecvQ.Get((char *)&stPacketCreateCharacter, sizeof(stPacketCreateCharacter));

			for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
			{
				if (gObject[iCnt] == NULL)
				{
					gObject[iCnt] = new CPlayerObject(FALSE, stPacketCreateCharacter.ID,
						eTYPE_PLAYER, stPacketCreateCharacter.HP, stPacketCreateCharacter.Direction);
					gObject[iCnt]->SetPosition(stPacketCreateCharacter.X, stPacketCreateCharacter.Y);
					break;
				}
			}
			break;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 캐릭터 제거
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
		case dfPACKET_SC_DELETE_CHARACTER :
			stPACKET_SC_DELETE_CHARACTER stPacketDelCharacter;
			RecvQ.Get((char *)&stPacketDelCharacter, sizeof(stPacketDelCharacter));

			for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
			{
				if (gObject[iCnt] != NULL && stPacketDelCharacter.ID == gObject[iCnt]->GetObjectID())
				{
					delete gObject[iCnt];
					gObject[iCnt] = NULL;
					break;
				}
			}
			break;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 움직임 시작
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
		case dfPACKET_SC_MOVE_START :
			stPACKET_SC_MOVE_START stPacketMoveStart;
			RecvQ.Get((char *)&stPacketMoveStart, sizeof(stPacketMoveStart));

			for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
			{
				if (gObject[iCnt] != NULL && stPacketMoveStart.ID == gObject[iCnt]->GetObjectID())
				{
					gObject[iCnt]->ActionInput(stPacketMoveStart.Direction);
					break;
				}
			}
			break;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 움직임 멈춤
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
		case dfPACKET_SC_MOVE_STOP :
			stPACKET_SC_MOVE_STOP stPacketMoveStop;
			RecvQ.Get((char *)&stPacketMoveStop, sizeof(stPacketMoveStop));

			for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
			{
				if (gObject[iCnt] != NULL && stPacketMoveStop.ID == gObject[iCnt]->GetObjectID() &&
					g_pPlayerObject->GetObjectID() != stPacketMoveStop.ID)
				{
					gObject[iCnt]->ActionInput(dfACTION_STAND);
					gObject[iCnt]->SetPosition(stPacketMoveStop.X, stPacketMoveStop.Y);
					break;
				}
			}
			break;

		case dfPACKET_SC_ATTACK1 :
			stPACKET_SC_ATTACK1 stPacketAttack1;
			RecvQ.Get((char *)&stPacketAttack1, sizeof(stPacketAttack1));

			for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
			{
				if (gObject[iCnt] != NULL && stPacketAttack1.ID == gObject[iCnt]->GetObjectID() &&
					g_pPlayerObject->GetObjectID() != stPacketMoveStop.ID)
				{
					gObject[iCnt]->SetPosition(stPacketAttack1.X, stPacketAttack1.Y);
					gObject[iCnt]->ActionInput(dfACTION_ATTACK1);
					break;
				}
			}
			break;

		case dfPACKET_SC_ATTACK2 :
			stPACKET_SC_ATTACK2 stPacketAttack2;
			RecvQ.Get((char *)&stPacketAttack2, sizeof(stPacketAttack2));

			for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
			{
				if (gObject[iCnt] != NULL && stPacketAttack2.ID == gObject[iCnt]->GetObjectID() &&
					g_pPlayerObject->GetObjectID() != stPacketMoveStop.ID)
				{
					gObject[iCnt]->ActionInput(dfACTION_ATTACK2);
					gObject[iCnt]->SetPosition(stPacketAttack2.X, stPacketAttack2.Y);
					break;
				}
			}
			break;

		case dfPACKET_SC_ATTACK3 :
			stPACKET_SC_ATTACK1 stPacketAttack3;
			RecvQ.Get((char *)&stPacketAttack3, sizeof(stPacketAttack3));

			for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
			{
				if (gObject[iCnt] != NULL && stPacketAttack3.ID == gObject[iCnt]->GetObjectID() &&
					g_pPlayerObject->GetObjectID() != stPacketAttack3.ID)
				{
					gObject[iCnt]->ActionInput(dfACTION_ATTACK3);
					gObject[iCnt]->SetPosition(stPacketAttack3.X, stPacketAttack3.Y);
					break;
				}
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Packet 보내기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteProc()
{
	int retval;
	while (1){
		if (SendQ.GetUseSize() <= 0)
			return;

		retval = send(client_sock, SendQ.GetReadBufferPtr(), SendQ.GetNotBrokenGetSize(), 0);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return;
			}//에러처리
		}

		if (retval > 0)
			SendQ.RemoveData(retval);
	}
}