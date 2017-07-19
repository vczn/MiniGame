// Tetris.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Tetris.h"


// 全局变量: 
HINSTANCE hInst;                                // 当前实例
std::wstring szTitle;							// 标题栏文本
std::wstring szWindowClass;					    // 主窗口类名

using namespace TETRIS_GAME;
TetrisGame game;

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
	szTitle = L"俄罗斯方块";
	szWindowClass = L"TetrisWndClassName";
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TETRIS);
    wcex.lpszClassName  = szWindowClass.c_str();
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

   HWND hWnd = CreateWindowW(szWindowClass.c_str(), szTitle.c_str(), WS_MINIMIZEBOX | WS_SYSMENU,
      CW_USEDEFAULT, CW_USEDEFAULT, 600, 800, nullptr, nullptr, hInstance, nullptr);

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
//	WM_CREATE   - 创建主窗口
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LONG nWinX, nWinY, nClientX, nClientY, nScreenX, nScreenY, nNewWinX, nNewWinY;
	static RECT rect;

    switch (message)
    {
	case WM_CREATE:
		// 获取窗口大小
		GetWindowRect(hWnd, &rect);
		nWinX = rect.right - rect.left;
		nWinY = rect.bottom - rect.top;
		// 获取客户区的大小
		GetClientRect(hWnd, &rect);
		nClientX = rect.right - rect.left;
		nClientY = rect.bottom - rect.top;
		// 改变窗口大小 && 移动到屏幕中央
		nNewWinX = 3 * BORDER_SIZE + (NUMBER_GRID_X + INFO_X) * TETRIS_SIZE + (nWinX - nClientX);
		nNewWinY = 2 * BORDER_SIZE +  NUMBER_GRID_Y * TETRIS_SIZE + (nWinY - nClientY);
		nScreenX = GetSystemMetrics(SM_CXSCREEN);
		nScreenY = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(hWnd, (nScreenX - nNewWinX) / 2, (nScreenY - nNewWinY) / 2,
			nNewWinX, nNewWinY, true);

		game.InitGame(hWnd);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_TIMER:
		if (game.CheckCollision(game.GetCurrPos().x, game.GetCurrPos().y + 1)) {	// 碰撞
			if (game.GetCurrPos().y == 0) {											// 在顶端碰撞
				game.GameOver(hWnd);

				game.InitGame(hWnd);
			}
			else {																	// 在非顶端碰撞，判断消行
				game.Collision();
			}
		}
		else																		// 未碰撞，下移
			++game.GetCurrPos().y;

		InvalidateRect(hWnd, nullptr, TRUE);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			RECT rc;
			GetClientRect(hWnd, &rc);

			HDC hMemDC			= CreateCompatibleDC(hdc);								// del
			HBITMAP hMemBmp		= CreateCompatibleBitmap(hdc, rc.right, rc.bottom);		// del
			SelectObject(hMemDC, hMemBmp);
			// 白色背景
			HBRUSH hWhiteBrush	= reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
			FillRect(hMemDC, &rc, hWhiteBrush);

			game.DrawGameArea(hMemDC);
			game.DrawInfo(hMemDC);
			game.DrawCurrTetris(hMemDC);

			BitBlt(hdc, 0, 0, rc.right, rc.bottom, hMemDC, 0, 0, SRCCOPY);

			DeleteDC(hMemDC);
			DeleteObject(hMemBmp);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			if (!game.CheckCollision(game.GetCurrPos().x - 1, game.GetCurrPos().y)) {
				--game.GetCurrPos().x;
				InvalidateRect(hWnd, nullptr, FALSE);
			}
			break;
		case VK_RIGHT:
			if (!game.CheckCollision(game.GetCurrPos().x + 1, game.GetCurrPos().y)) {
				++game.GetCurrPos().x;
				InvalidateRect(hWnd, nullptr, FALSE);
			}
			break;
		case VK_UP:
			game.RotateTetris();
			InvalidateRect(hWnd, nullptr, FALSE);
			break;
		case VK_DOWN:
			if (game.CheckCollision(game.GetCurrPos().x, game.GetCurrPos().y + 1)) {
				game.Collision();
			}
			else
				++game.GetCurrPos().y;
			InvalidateRect(hWnd, nullptr, FALSE);
			break;
		default:
			break;
		}
		break;
    case WM_DESTROY:
		KillTimer(hWnd, MY_TIMER);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
