using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gobang
{
    class Game
    {
        public ChessBoard chessBoard;

        bool bIsStart;
        int curPlayer;                  //轮到哪一方落子 1 为黑棋  2 为白棋

        public void startGame()
        {
            chessBoard = new ChessBoard();
            bIsStart = true;
            curPlayer = Global.PLAYER1;
        }

        public void changePlayer()
        {
            curPlayer = (curPlayer == Global.PLAYER1 ? Global.PLAYER2 : Global.PLAYER1);
        }

        public bool placePiece(int x, int y)
        {
            if (chessBoard.bPlacePiece(x, y, curPlayer))
            {
                if (isWin(x, y))
                {
                    bIsStart = false;
                    return true;
                }
                changePlayer();
                return true;
            }
            return false;
        }

        public bool isWin(int x, int y)
        {
            if (evaluatePiece(chessBoard, x, y, curPlayer) >= Global.EVA_FIVE)
                return true;
            return false;
        }

        private void gameOver()
        {
            bIsStart = false;
        }
        public int getCurPlayer()
        {
            return curPlayer;
        }

        public bool bGameStart()
        {
            return bIsStart;
        }
        public bool bGameOver()
        {
            return !bIsStart;
        }
        private int nextPlayer(int p)
        {
            return p == Global.PLAYER1 ? Global.PLAYER2 : Global.PLAYER1;
        }


        private int pieceType(int t, int type)
        {
            return t == type ? Global.EVA_MY : (t == Global.EVA_EMPTY ? Global.EVA_EMPTY : Global.EVA_OT);
        }

        private int pieceType(ChessBoard.Grid[,] board, int x, int y, int p)
        {
            if (x < 0 || y < 0 || x >= Global.GRID_NUM || y >= Global.GRID_NUM)
                return Global.EVA_OT;
            else
                return pieceType(board[x, y].player, p);
        }

        private int getValue(int cnt, int blk)
        {
            if (blk == 0)       //活棋
            {
                switch (cnt)
                {
                    case 1: return Global.EVA_ONE;
                    case 2: return Global.EVA_TWO;
                    case 3: return Global.EVA_THREE;
                    case 4: return Global.EVA_FOUR;
                    default: return Global.EVA_FIVE;
                }
            }
            else if (blk == 1)  //一边拦截
            {
                switch (cnt)
                {
                    case 1: return Global.EVA_ONE_S;
                    case 2: return Global.EVA_TWO_S;
                    case 3: return Global.EVA_THREE_S;
                    case 4: return Global.EVA_FOUR_S;
                    default: return Global.EVA_FIVE;
                }
            }
            else              //两边拦截
            {
                if (cnt >= 5)
                    return Global.EVA_FIVE;
                else
                    return Global.EVA_ZERO;
            }       
        }

        private int evaluateLine(int[] line)
        {
            int cnt = 1; // 连子数
            int blk = 0; // 封闭数
            // 向两个方向考察
            // 向左
            for (int i = 3; i >= 0; --i)
            {
                if (line[i] == Global.EVA_MY) ++cnt;
                else if (line[i] == Global.EVA_OT)
                {
                    ++blk;
                    break;
                }
                else break;
            }
            // 向右
            for (int i = 5; i < 9; ++i)
            {
                if (line[i] == Global.EVA_MY) ++cnt;
                else if (line[i] == Global.EVA_OT)
                {
                    ++blk;
                    break;
                }
                else break;
            }

            return getValue(cnt, blk);
        }





        private int evaluatePiece(ChessBoard state, int x, int y, int p)
        {
            int value = 0;

            int[] line = new int[17];

            bool[] flagX = new bool[8];     //横向是否碰到边界
            for (int i = 0; i < 8; ++i)
            {
                if (i < 4)
                    flagX[i] = x - 4 + i < 0;
                else
                    flagX[i] = x - 3 + i > 14;
            }

            bool[] flagY = new bool[8];     //纵向是否碰到边界
            for (int i = 0; i < 8; ++i)
            {
                if (i < 4)
                    flagY[i] = y - 4 + i < 0;
                else
                    flagY[i] = y - 3 + i > 14;
            }

            ///初始化line,获取棋盘上的状态，超过边界按照玩家棋子算，不超过边界获取棋子类型。
            line[4] = Global.EVA_MY;

            //横
            for (int i = 0; i < 4; ++i)
                line[i] = flagX[i] ? Global.EVA_OT : pieceType(state.chessBoard[x - 4 + i, y].player, p);
            for (int i = 5; i < 9; ++i)
                line[i] = flagX[i - 1] ? Global.EVA_OT : pieceType(state.chessBoard[x - 4 + i, y].player, p);
            value += evaluateLine(line);

            //纵
            for (int i = 0; i < 4; ++i)
                line[i] = flagY[i] ? Global.EVA_OT : pieceType(state.chessBoard[x, y - 4 + i].player, p);
            for (int i = 5; i < 9; ++i)
                line[i] = flagY[i - 1] ? Global.EVA_OT : pieceType(state.chessBoard[x, y - 4 + i].player, p);
            value += evaluateLine(line);

            // \
            for (int i = 0; i < 4; ++i)
                line[i] = flagX[i] || flagY[i] ? Global.EVA_OT : pieceType(state.chessBoard[x - 4 + i, y - 4 + i].player, p);
            for (int i = 5; i < 9; ++i)
                line[i] = flagX[i - 1] || flagY[i - 1] ? Global.EVA_OT : pieceType(state.chessBoard[x - 4 + i, y - 4 + i].player, p);

            value += evaluateLine(line);

            // /
            for (int i = 0; i < 4; ++i)
                line[i] = flagX[7 - i] || flagY[i] ? Global.EVA_OT : pieceType(state.chessBoard[x + 4 - i, y - 4 + i].player, p);
            for (int i = 5; i < 9; ++i)
                line[i] = flagX[8 - i] || flagY[i - 1] ? Global.EVA_OT : pieceType(state.chessBoard[x + 4 - i, y - 4 + i].player, p);
            value += evaluateLine(line);

            return value;

        }


        private int evaluateLine(int[] line, bool ALL)
        {
            int value = 0; // 评估值
            int cnt = 0; // 连子数
            int blk = 0; // 封闭数

            // 从左向右扫描
            for (int i = 0; i < Global.GRID_NUM; ++i)
            {
                if (line[i] == Global.EVA_MY) // 找到第一个己方的棋子
                {
                    cnt = 1;
                    blk = 0;
                    // 看左侧是否封闭
                    if (line[i - 1] == Global.EVA_OT) ++blk;
                    // 计算连子数
                    for (i = i + 1; i < Global.GRID_NUM && line[i] == Global.EVA_MY; ++i, ++cnt) ;
                    // 看右侧是否封闭
                    if (line[i] == Global.EVA_OT) ++blk;
                    // 计算评估值
                    value += getValue(cnt, blk);
                }
            }
            return value;
        }

        public int evaluateState(ChessBoard state, int p)
        {
            int value = 0;
            int[][] line = new int[6][];
            for (int i = 0;i < 6; ++i)
            {
                line[i] = new int[17];      //方便检查边界
            }

            for (int i = 0; i < 6; ++i)
                line[i][0] = line[i][16] = Global.EVA_OT;

            for (int i  = 0; i < Global.GRID_NUM; ++i)
            {
                int lineP = 1;
                for (int j = 0; j < Global.GRID_NUM; ++j)
                {
                    line[0][lineP] = pieceType(state.chessBoard, i, j, p);              // —
                    line[1][lineP] = pieceType(state.chessBoard, j, i, p);              // 丨
                    line[2][lineP] = pieceType(state.chessBoard, i + j, j, p);          // 右上\
                    line[3][lineP] = pieceType(state.chessBoard, i - j, j, p);          // 左上/
                    line[4][lineP] = pieceType(state.chessBoard, j, i + j, p);          // 左下\
                    line[5][lineP] = pieceType(state.chessBoard, 15 - j - 1, i + j, p); // 右下/
                    ++lineP;
                }
                int special = i == 0 ? 4 : 6;
                for (int k = 0; k < special; ++k)
                {
                    value += evaluateLine(line[k], true);
                }
            }

            return value;
        }

        ///****************AI算法****************///
        ///****************AI算法****************///
        ///****************AI算法****************///

        private bool bSearch(ChessBoard state, int x, int y)
        {

            int tmpX = x - 1;
            int tmpY = y - 1;
            for (int i = 0; tmpX < Global.GRID_NUM && i < 3; ++tmpX, ++i)
            {
                int ty = tmpY;
                for (int j = 0; ty < Global.GRID_NUM && j < 3; ++ty, ++j)
                {
                    if (tmpX >= 0 && ty >= 0 && state.chessBoard[tmpX, ty].player != Global.EMPTY)
                        return true;
                    else
                        continue;
                }
            }
            return false;
        }


        public static int MaxDepth;
        //AI为MAX， 玩家为MIN
        private int minMax(ChessBoard state, int x, int y, int p, int depth, int alpha, int beta)
        {
            ChessBoard newState = new ChessBoard(state);
            newState.bPlacePiece(x, y, nextPlayer(p));

            int weight = 0;
            int max = -Global.INF;
            int min = Global.INF;

            if (depth < MaxDepth)
            {
                if (evaluatePiece(newState, x, y, nextPlayer(p)) >= Global.EVA_FIVE)
                {
                    if (p == Global.PLAYER1)
                        return Global.EVA_FIVE;     //AI获胜
                    else
                        return -Global.EVA_FIVE;    //玩家获胜
                }

                for (int i = 0; i < Global.GRID_NUM; ++i)
                {
                    for (int j = 0; j < Global.GRID_NUM; ++j)
                    {
                        if (newState.chessBoard[i, j].player == Global.EMPTY && bSearch(newState, i, j))
                        {
                            weight = minMax(newState, i, j, nextPlayer(p), depth + 1, min, max);
                            if (weight > max)   //更新上界
                                max = weight;
                            if (weight < min)   //更新下界
                                min = weight;

                            //alpha-beta 剪枝
                            if (p == Global.PLAYER2)
                            {
                                if (max >= alpha)
                                    return max;
                            }
                            else
                            {
                                if (min <= beta)
                                    return min;
                            }
                        }
                        else
                            continue;
                    }
                }
                //扩展完毕
                if (p == Global.PLAYER2)
                    return max;
                else
                    return min;

            }
            else
            {
                weight = evaluateState(newState, Global.PLAYER2);        //评估AI局面
                weight -= p == Global.PLAYER1 ? evaluateState(newState, Global.PLAYER1) * 10 : evaluateState(newState, Global.PLAYER1);
                return weight;
            }


        }

        public Point AIPlacePiece()
        {
            int weight;
            int max = -Global.INF; // 本层的权值上界
            int x = 0, y = 0;
            for (int i = 0; i < Global.GRID_NUM; ++i)
            {
                for (int j = 0; j < Global.GRID_NUM; ++j)
                {
                    if (chessBoard.chessBoard[i, j].player == Global.EMPTY && bSearch(chessBoard, i, j)) // 先判断有无搜索必要
                    {
                        weight = minMax(chessBoard, i, j, nextPlayer(Global.PLAYER2), 1, -Global.INF, max); // 扩展此节点

                        if (weight > max)
                        {
                            max = weight; // 更新下层上界
                            x = i;
                            y = j;
                        }
                    }
                    else
                        continue;
                }
            }
            placePiece(x, y);
            return new Point(x, y);
        }
       
    }
}
