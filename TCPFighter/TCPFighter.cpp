// TCPFighter.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "TCPFighter.h"
#include "ScreenDib.h"
#include "SpriteDib.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"

#define MAX_OBJECT 100

// ���� ����:
HINSTANCE hInst;	// ���� �ν��Ͻ��Դϴ�.
HWND g_hWnd;
CBaseObject *g_pPlayerObject;
CBaseObject *gObject[MAX_OBJECT];
CScreenDib g_cScreenDib(640, 480, 32);
CSpriteDib *g_pSpriteDib;
BOOL g_bActiveApp;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void InitialGame();
void Update_Game(void);
void KeyProcess();
void Action();
void Draw();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
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

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.
	
	InitialGame();
	g_hWnd = CreateWindow(L"TCPFighter", L"TCPFighter", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ������ ���߱�
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

	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	// ���α׷� ��Ȱ��ȭ üũ
	case WM_ACTIVATEAPP :
		g_bActiveApp = (BOOL)wParam;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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

void InitialGame()
{
	g_pSpriteDib = new CSpriteDib(eSPRITE_MAX, 0x00ffffff);

	g_pSpriteDib->LoadDibSprite(eMAP, L"Data\\_Map.bmp", 0, 0);

	// �� �߰�
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

	g_pPlayerObject = new CPlayerObject(TRUE);
	g_pPlayerObject->SetPosition(100, 100);
	gObject[0] = g_pPlayerObject;
}

void Update_Game(void)
{
	if (g_bActiveApp)
		KeyProcess();

	Action();
	Draw();

	g_cScreenDib.DrawBuffer(g_hWnd);
}

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

void Action()
{
	for (int iCnt = 0; iCnt < MAX_OBJECT; iCnt++)
	{
		if (gObject[iCnt] != NULL)
			gObject[iCnt]->Action(1);
	}

	//y�� ���� ��Ű��
	//����Ʈ�� ���� �ڷ� �б�
}

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