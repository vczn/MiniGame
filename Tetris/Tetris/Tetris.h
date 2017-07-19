#pragma once

#include "resource.h"

#include <string>
#include <ctime>
#include <random>

namespace TETRIS_GAME {
const int BORDER_SIZE	= 10;					// 边界宽度
const int TETRIS_SIZE	= 30;					// 一个小方格的边长
const int NUMBER_GRID_X = 10;					// 在水平方向上
const int NUMBER_GRID_Y = 20;
const int INFO_X		= 6;					// 右边提示区宽度,单位 TETRIS_SIZE
const int INFO_Y		= 20;

const int MY_TIMER		= 1;
const int INTERVAL		= 500;					// 下降一格的时间间隔 ms
const int NUMBER_TYPE	= 7;


const BOOL All_TYPE_TETRIS[NUMBER_TYPE][4][4] = {
	{
		{ 1,1,1,1 },	// 口口口口
		{ 0,0,0,0 },
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	},

	{
		{ 1,1,1,0 },    // 口口口
		{ 1,0,0,0 },	// 口
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	},

	{
		{ 1,1,1,0 },	// 口口口
		{ 0,0,1,0 },	//     口
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	},

	{
		{ 1,1,1,0 },	// 口口口
		{ 0,1,0,0 },	//   口
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	},

	{
		{ 1,1,0,0 },	// 口口
		{ 1,1,0,0 },	// 口口
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	},

	{
		{ 0,1,1,0 },	//   口口
		{ 1,1,0,0 },	// 口口
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	},

	{
		{ 1,1,0,0 },	// 口口
		{ 0,1,1,0 },	//   口口
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	}
};

UINT GetRandomNum() {
	static std::default_random_engine e(static_cast<UINT>(time(nullptr)));
	static std::uniform_int_distribution<UINT> u(0, NUMBER_TYPE - 1);

	return u(e);
}

class TetrisGame final {
private:
	BOOL	mCurrTetris[4][4];
	BOOL	mNextTetris[4][4];
	BOOL	mExistTetris[NUMBER_GRID_X][NUMBER_GRID_Y];
	POINT	mCurrTetrisPos;
	UINT	mInterval;
	UINT	mScore;
public:
	TetrisGame () { }

	VOID InitGame(HWND hWnd) {
		memcpy(mCurrTetris, All_TYPE_TETRIS[GetRandomNum()], sizeof(mCurrTetris));
		memcpy(mNextTetris, All_TYPE_TETRIS[GetRandomNum()], sizeof(mNextTetris));
		memset(mExistTetris, 0, sizeof(mExistTetris));

		mCurrTetrisPos.x = (NUMBER_GRID_X - 4) / 2;
		mCurrTetrisPos.y = 0;
		mInterval = INTERVAL;
		mScore = 0;

		SetTimer(hWnd, MY_TIMER, mInterval, nullptr);
	}

	POINT& GetCurrPos() noexcept {
		return mCurrTetrisPos;
	}

	// 画游戏区域（左部分）
	VOID DrawGameArea(HDC hdc) const noexcept {
		HPEN hWhitePen		= reinterpret_cast<HPEN>(GetStockObject(NULL_PEN));
		HBRUSH hGrayBrush	= reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
		HBRUSH hBlackBrush	= reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));

		// 画游戏区域的外边框
		Rectangle(hdc, BORDER_SIZE - 1, BORDER_SIZE - 1, BORDER_SIZE + NUMBER_GRID_X * TETRIS_SIZE,
			BORDER_SIZE + NUMBER_GRID_Y * TETRIS_SIZE);

		// 不存在格子,刷成灰色；存在格子，刷成黑色
		// 小格子边框用白色笔画
		SelectObject(hdc, hWhitePen);
		for (int i = 0; i < NUMBER_GRID_X; ++i) {
			for (int j = 0; j < NUMBER_GRID_Y; ++j) {
				if (mExistTetris[i][j] == 0)
					SelectObject(hdc, hGrayBrush);
				else
					SelectObject(hdc, hBlackBrush);

				Rectangle(hdc, BORDER_SIZE + i * TETRIS_SIZE, BORDER_SIZE + j * TETRIS_SIZE,
					BORDER_SIZE + (i + 1) * TETRIS_SIZE, BORDER_SIZE + (j + 1) * TETRIS_SIZE);
			}
		}
	}

	// 画信息区（右部分）
	VOID DrawInfo(HDC hdc) const noexcept {
		INT nStartX = BORDER_SIZE * 2 + NUMBER_GRID_X * TETRIS_SIZE;
		INT nStartY = BORDER_SIZE;
		RECT rect;
		std::wstring szScore = L"得分: " + std::to_wstring(mScore);

		HPEN hBlackPen		= reinterpret_cast<HPEN>(GetStockObject(BLACK_PEN));
		HBRUSH hNullBrush	= reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
		HBRUSH hGrayBrush	= reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
		SelectObject(hdc, hBlackPen);
		SelectObject(hdc, hNullBrush);

		// 画信息区的外边框
		Rectangle(hdc, 2 * BORDER_SIZE + NUMBER_GRID_X * TETRIS_SIZE,
			BORDER_SIZE,
			2 * BORDER_SIZE + (NUMBER_GRID_X + INFO_X) * TETRIS_SIZE,
			BORDER_SIZE + NUMBER_GRID_Y * TETRIS_SIZE);

		// 画 next tetris
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (mNextTetris[i][j])
					SelectObject(hdc, hNullBrush);
				else
					SelectObject(hdc, hGrayBrush);

				// Next Tetris 区域的 上、下、左、右 都留有一个 TETRIS_SIZE 的大小，所以坐标 + 1
				Rectangle(hdc, nStartX + (j + 1) * TETRIS_SIZE,
					nStartY + (i + 1) * TETRIS_SIZE,
					nStartX + (j + 2) * TETRIS_SIZE,
					nStartY + (i + 2) * TETRIS_SIZE);
			}
		}

		// 画得分区域
		rect.left	= nStartX + TETRIS_SIZE;		// 1(左空白)
		rect.top	= nStartY + TETRIS_SIZE * 6;	// 6 = 1(上空白) + 4(Next Tetris) + 1(下空白)
		rect.right	= nStartX + TETRIS_SIZE * 5;	// 5 = 1(左空白) + 4(Next Tetris)
		rect.bottom = nStartY + TETRIS_SIZE * 7;

		DrawText(hdc, szScore.c_str(), szScore.size(), &rect, DT_CENTER | DT_VCENTER);
	}

	// 画 4x4 的方块
	// 以左上角为原点，水平方向为 x， 竖直方向为 y，单位为 TETRIS_SIZE
	VOID DrawCurrTetris(HDC hdc) const noexcept {
		HPEN hBlackPen = reinterpret_cast<HPEN>(GetStockObject(BLACK_PEN));
		HBRUSH hWhiteBrush = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));

		SelectObject(hdc, hBlackPen);
		SelectObject(hdc, hWhiteBrush);

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (mCurrTetris[i][j]) {
					Rectangle(hdc,
						BORDER_SIZE + (mCurrTetrisPos.x + j) * TETRIS_SIZE,
						BORDER_SIZE + (mCurrTetrisPos.y + i) * TETRIS_SIZE,
						BORDER_SIZE + (mCurrTetrisPos.x + j + 1) * TETRIS_SIZE,
						BORDER_SIZE + (mCurrTetrisPos.y + i + 1) * TETRIS_SIZE);
				}
			}
		}
	}

	BOOL CheckCollision(INT x, INT y) const noexcept {
		// 碰左墙
		if (x < 0)
			return TRUE;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (mCurrTetris[i][j]) {
					// 碰右墙
					if (x + j >= NUMBER_GRID_X)
						return TRUE;

					// 碰下墙
					if (y + i >= NUMBER_GRID_Y)
						return TRUE;

					// 碰到已有方块
					if (mExistTetris[x + j][y + i])
						return TRUE;
				}

			}
		}
		return FALSE;
	}

	VOID RotateTetris() noexcept {
		BOOL bNewTetris[4][4] = { };
		BOOL bTmpTetris[4][4] = { };	// 备份原数据
		BOOL bTheRowHasData = FALSE;	// 保证第一行有数据
		BOOL bTheColHasData = FALSE;	// 保证第一列有数据

		// 备份
		memcpy(bTmpTetris, mCurrTetris, sizeof(bTmpTetris));

		// 在 4x4 正方形中直接旋转到新的 4x4 矩阵中 
		// mCurrTetris Rotation-> bNewTetris  
		for (int i = 0, xPos = 0; i < 4; ++i) {
			bTheRowHasData = FALSE;
			for (int j = 0; j < 4; ++j) {
				if ((bNewTetris[xPos][j] = mCurrTetris[3 - j][i]) == TRUE)
					bTheRowHasData = TRUE;
			}
			if (bTheRowHasData)
				++xPos;
		}
		memset(mCurrTetris, 0, sizeof(mCurrTetris));

		// 把方块平移到 4x4 的左上角
		// bNewTetrs Translate-> mCurrTetris
		for (int i = 0, yPos = 0; i < 4; ++i) {
			bTheColHasData = FALSE;
			for (int j = 0; j < 4; ++j) {
				if ((mCurrTetris[j][yPos] = bNewTetris[j][i]) == TRUE)
					bTheColHasData = TRUE;
			}
			if (bTheColHasData)
				++yPos;
		}

		// 检测碰撞
		if (CheckCollision(mCurrTetrisPos.x, mCurrTetrisPos.y))
			memcpy(mCurrTetris, bTmpTetris, sizeof(mCurrTetris));
	}

	VOID Collision() {
		// 合并 mCurrTetris 到 mExistTetris
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (mCurrTetris[i][j])
					mExistTetris[mCurrTetrisPos.x + j][mCurrTetrisPos.y + i] = TRUE;
			}
		}

		// 消行
		BOOL bFullLine = TRUE;
		UINT iFullLine = 0;
		for (int y = NUMBER_GRID_Y, newY = NUMBER_GRID_Y; y >= 0; --y) {
			bFullLine = TRUE;
			for (int x = 0; x < NUMBER_GRID_X; ++x) {
				mExistTetris[x][newY] = mExistTetris[x][y];
				if (!mExistTetris[x][y]) {
					bFullLine = FALSE;		// 该行未满
				}
			}

			if (bFullLine)	// 满行
				++iFullLine;
			else			// 满行
				--newY;
		}

		// 根据消掉的行数计算分数
		mScore += (iFullLine * iFullLine) * 10;
		
		// 改变 CurrTetris 和 NextTetris
		memcpy(mCurrTetris, mNextTetris, sizeof(mCurrTetris));
		memcpy(mNextTetris, All_TYPE_TETRIS[GetRandomNum()], sizeof(mNextTetris));

		mCurrTetrisPos.x = (NUMBER_GRID_X - 4) / 2;
		mCurrTetrisPos.y = 0;
	}

	VOID GameOver(HWND hWnd) const noexcept {
		KillTimer(hWnd, MY_TIMER);
		MessageBox(hWnd, L"GAME OVER", L"提示", MB_OK);
	}
};

}