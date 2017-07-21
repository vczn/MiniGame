// 2048.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "2048.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
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

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2048, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CLIENT_WIDTH, CLIENT_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	INT nScreenWidth	= GetSystemMetrics(SM_CXSCREEN);
	INT nScreenHeight	= GetSystemMetrics(SM_CYSCREEN);

	// 移到屏幕中央
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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
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
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

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

