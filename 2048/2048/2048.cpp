// 2048.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "2048.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2048, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2048));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CLIENT_WIDTH, CLIENT_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	INT nScreenWidth	= GetSystemMetrics(SM_CXSCREEN);
	INT nScreenHeight	= GetSystemMetrics(SM_CYSCREEN);

	// �Ƶ���Ļ����
	MoveWindow(hWnd, (nScreenWidth - CLIENT_WIDTH) / 2, 
		(nScreenHeight - CLIENT_HEIGHT) / 2, CLIENT_WIDTH, 
		CLIENT_HEIGHT, TRUE);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//


#define IDB_NEW 1001	// button "new game"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hButton;
	static INT dr;

    switch (message) {
	case WM_CREATE:
		InitGame(hWnd);
		// create new game button
		hButton = CreateWindow(L"Button", L"new game",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT,
			RECT_NEW.left, RECT_NEW.top, RECT_NEW.right - RECT_NEW.left,
			RECT_NEW.bottom - RECT_NEW.top, hWnd, (HMENU)IDB_NEW,
			hInst, nullptr);
		break;
    case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...

		RECT rc;
		GetClientRect(hWnd, &rc);
		HDC hMemDC = CreateCompatibleDC(hdc);									// del
		DrawGame(hdc);
		BitBlt(hdc, 0, 0, rc.right, rc.bottom, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);
	}
		break;
	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_LEFT:
			Key_Left();
			InvalidateRect(hWnd, nullptr, TRUE);
			CheckState(hWnd);
			break;
		case VK_UP:
			Key_Up();
			InvalidateRect(hWnd, nullptr, TRUE);
			CheckState(hWnd);
			break;
		case VK_RIGHT:
			Key_Right();
			InvalidateRect(hWnd, nullptr, TRUE);
			CheckState(hWnd);
			break;
		case VK_DOWN:
			Key_Down();
			InvalidateRect(hWnd, nullptr, TRUE);
			CheckState(hWnd);
			break;
		}
	}	
		break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDB_NEW: {
			dr = MessageBox(hWnd, L"new game?", L"confirm", MB_OK);
			if (dr == 1) {
				Destroy();
				InitGame(hWnd);
				InvalidateRect(hWnd, nullptr, TRUE);
			}
		
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
    case WM_DESTROY:
		Destroy();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

