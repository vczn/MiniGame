#pragma once

#include "resource.h"
#include <array>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include <unordered_map>

CONST INT CLIENT_WIDTH	= 600;
CONST INT CLIENT_HEIGHT	= 650;
CONST INT GRID_SIZE		= 90;
CONST INT EDGE_SIZE		= 20;
RECT RECT_SCORE			= { 70, 30, 70 + 140, 30 + 40 };
RECT RECT_BEST			= { 530 - 140, 30, 530, 30 + 40 };
RECT RECT_GAME			= { 70, 80, 530, 540 };
RECT RECT_NEW			= { 230, 570, 370, 600 };

UINT g_score, g_bestScore;
BOOL g_bWin;
std::array<std::array<UINT, 4>, 4> g_grid;

std::unordered_map<INT, COLORREF> COLORS = {
	{   -1, RGB(0xbb, 0xad, 0xa0) },	// background
	{    0, RGB(0xcd, 0xc1, 0xb4) },	// 0
	{    2, RGB(0xee, 0xe4, 0xda) },	// 2^1
	{    4, RGB(0xed, 0xe0, 0xc8) },	// 2^2
	{    8, RGB(0xf5, 0x95, 0x63) },	// 2^3
	{   16, RGB(0xf5, 0x95, 0x63) },	// 2^4
	{   32, RGB(0xf6, 0x7b, 0x5e) },	// 2^5
	{   64, RGB(0xf6, 0x7b, 0x60) },	// 2^6
	{  128, RGB(0xed, 0xcf, 0x72) },	// 2^7
	{  256, RGB(0xed, 0xcc, 0x61) },	// 2^8
	{  512, RGB(0xed, 0xc8, 0x50) },	// 2^9
	{ 1024, RGB(0xec, 0xc4, 0x41) },	// 2^10
	{ 2048, RGB(0xee, 0xc2, 0x2e) },	// 2^11
	{ 4096, RGB(0xff, 0x3c, 0x3e) },	// 2^12
	{ 8192, RGB(0xff, 0x1e, 0x20) },	// 2^13
};

int GetRandom(int val) {
	static std::default_random_engine e{ static_cast<UINT>(time(nullptr)) };
	std::uniform_int_distribution<int> u(0, val);

	return u(e);
}

VOID GenerateOneGrid() {
	while (true) {
		int iGrid	= GetRandom(9) == 9 ? 4 : 2;
		int iPos	= GetRandom(15);
		if (g_grid[iPos / 4][iPos % 4] == 0U) {
			g_grid[iPos / 4][iPos % 4] = static_cast<UINT>(iGrid);
			break;
		}
	}
}

inline UINT EncryptionScore(UINT score) noexcept {
	return (score >> 1) + 2333;
}

inline UINT DecryptedScore(UINT score) noexcept {
	return ((score - 2333) << 1);
}

VOID InitGame(HWND hWnd) {
	g_score = 0;
	g_bestScore = 0;
	g_bWin = FALSE;

	for (auto& arr : g_grid) {
		for (auto& x : arr) {
			x = 0U;
		}
	}

	// get best score from file
	std::ifstream fin{"log.txt"};

	if (fin.is_open()) {
		UINT tmp = 0U;
		fin >> tmp;
		g_bestScore = DecryptedScore(tmp);
	}
	fin.close();
	
	GenerateOneGrid();
	GenerateOneGrid();
}



VOID DrawGame(HDC hdc) {
	std::wstring wstrTmp;
	// draw score rectangle
	Rectangle(hdc, RECT_SCORE.left, RECT_SCORE.top, RECT_SCORE.right, RECT_SCORE.bottom);
	wstrTmp = L"score: " + std::to_wstring(g_score);
	DrawText(hdc, wstrTmp.c_str(), wstrTmp.size(), 
		&RECT_SCORE, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// draw best score rectangle
	Rectangle(hdc, RECT_BEST.left, RECT_BEST.top, RECT_BEST.right, RECT_BEST.bottom);
	wstrTmp = L"best score: " + std::to_wstring(g_bestScore);
	DrawText(hdc, wstrTmp.c_str(), wstrTmp.size(), 
		&RECT_BEST, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// draw new game
	Rectangle(hdc, RECT_NEW.left, RECT_NEW.top, RECT_NEW.right, RECT_NEW.bottom);

	// draw game area
	Rectangle(hdc, RECT_GAME.left, RECT_GAME.top, RECT_GAME.right, RECT_GAME.bottom);

	HBRUSH hBgBrush = CreateSolidBrush(COLORS[-1]);
	FillRect(hdc, &RECT_GAME, hBgBrush);
	DeleteObject(hBgBrush);

	// draw grid
	INT nStartX = RECT_GAME.left;
	INT nStartY = RECT_GAME.top;

	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, L"Consolas");
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			int thisGrid = static_cast<int>(g_grid[i][j]);

			HBRUSH hBrush;
			if (thisGrid <= 8192)
				hBrush = CreateSolidBrush(COLORS[thisGrid]);
			else
				hBrush = CreateSolidBrush(COLORS[8192]);

			RECT rect = { 
				nStartX + EDGE_SIZE * (j + 1) + GRID_SIZE * j,
				nStartY + EDGE_SIZE * (i + 1) + GRID_SIZE * i,
				nStartX + EDGE_SIZE * (j + 1) + GRID_SIZE * (j + 1), 
				nStartY + EDGE_SIZE * (i + 1) + GRID_SIZE * (i + 1)
			};

			FillRect(hdc, &rect, hBrush);
			if (thisGrid != 0) {
				wstrTmp = std::to_wstring(thisGrid);
				DrawText(hdc, wstrTmp.c_str(), wstrTmp.size(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			}
			DeleteObject(hBrush);
		}
	}
	DeleteObject(hFont);
}

namespace {

void __Operator(std::array<UINT, 4>& arr, BOOL& bMove) {
	for (int i = 0; i < 4; ++i) {
		int nextIndex = -1;
		// find next index with value is not zero
		for (int j = i + 1; j < 4; ++j) {
			if (arr[j] != 0) {
				nextIndex = j;
				break;
			}
		}

		// move 
		if (nextIndex != -1) {	// next index exist 
			if (arr[i] == 0U) {
				arr[i] = arr[nextIndex];
				arr[nextIndex] = 0U;
				--i;
				bMove = TRUE;
			}
			else if (arr[i] == arr[nextIndex]) {
				g_score += (arr[i] <<= 1);
				if (g_bestScore < g_score)
					g_bestScore = g_score;
				arr[nextIndex] = 0U;
				bMove = TRUE;
			}
		}
	}
}

#define ADD_GRID(b) \
	do {\
		if (b)\
			GenerateOneGrid();\
	} while (0)

}

VOID Key_Left() {
	BOOL bMove = FALSE;
	for (auto& arr : g_grid) {	
		__Operator(arr, bMove);
	}
	ADD_GRID(bMove);
}

VOID Key_Up() {
	BOOL bMove = FALSE;
	for (int i = 0; i < 4; ++i) {
		std::array<UINT, 4> arr
			= { g_grid[0][i], g_grid[1][i], g_grid[2][i], g_grid[3][i] };
		__Operator(arr, bMove);

		g_grid[0][i] = arr[0];
		g_grid[1][i] = arr[1];
		g_grid[2][i] = arr[2];
		g_grid[3][i] = arr[3];
	}
	ADD_GRID(bMove);
}

VOID Key_Right() {
	BOOL bMove = FALSE;
	for (auto& arr : g_grid) {
		std::reverse(arr.begin(), arr.end());
		__Operator(arr, bMove);
		std::reverse(arr.begin(), arr.end());
	}
	ADD_GRID(bMove);
}

VOID Key_Down() {
	BOOL bMove = FALSE;
	for (int i = 0; i < 4; ++i) {
		std::array<UINT, 4> arr
			= { g_grid[3][i], g_grid[2][i], g_grid[1][i], g_grid[0][i] };
		__Operator(arr, bMove);

		g_grid[3][i] = arr[0];
		g_grid[2][i] = arr[1];
		g_grid[1][i] = arr[2];
		g_grid[0][i] = arr[3];
	}
	ADD_GRID(bMove);
}

BOOL CheckFail() {
	for (const auto& arr : g_grid) {
		for (const auto& x : arr) {
			if (x == 0U)
				return FALSE;
		}
	}

	// horizontal judgment
	for (const auto& arr : g_grid) {
		for (int j = 0; j < 3; ++j) {
			if (arr[j] == arr[j + 1])
				return FALSE;
		}
	}

	// vertical judgment
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 3; ++i) {
			if (g_grid[i][j] == g_grid[i + 1][j])
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CheckWin() {
	if (g_score <= 20480U)
		return FALSE;
	for (const auto& arr : g_grid) {
		for (const auto& x : arr) {
			if (x >= 2048U)
				return TRUE;
		}
	}

	return FALSE;
}

VOID Destroy() {
	std::ofstream fout("log.txt");
	fout << EncryptionScore(g_bestScore);
	fout.close();
}


VOID CheckState(HWND hWnd) {
	if (CheckFail()) {
		MessageBox(hWnd, L"try again", L"fail", MB_OK);
		Destroy();
		InitGame(hWnd);	
		InvalidateRect(hWnd, nullptr, TRUE);
	}
	if (g_bWin == FALSE && CheckWin()) {
		int dr = MessageBox(hWnd, L"are you continue?", L"win", MB_OKCANCEL);
		g_bWin = TRUE;
		if (dr == 2)
			std::quick_exit(EXIT_SUCCESS);
	}
}
