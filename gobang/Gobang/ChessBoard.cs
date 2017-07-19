using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gobang
{
    class ChessBoard
    {
        public class Grid
        {
            public int player;
            public Grid()
            {
                player = Global.EMPTY;
            }
            public Grid(int p)
            {
                player = p;
            }
            public bool isEmpty()
            {
                return player == Global.EMPTY;
            }
        }


        public Grid[,] chessBoard = new Grid[Global.GRID_NUM,Global.GRID_NUM];

        public ChessBoard()
        {
            for (int i = 0; i < Global.GRID_NUM; ++i)
                for (int j = 0; j < Global.GRID_NUM; ++j)
                    chessBoard[i, j] = new Grid();
        }
        public ChessBoard(ChessBoard other)
        {
            for (int i = 0; i < Global.GRID_NUM; ++i)
                for (int j = 0; j < Global.GRID_NUM; ++j)
                    chessBoard[i, j] = new Grid(other.chessBoard[i, j].player);
        }

        public bool bPlacePiece(int x, int y, int p)
        {
            if (x < 0 || y < 0 || x >= 15 || y >= 15)
                return false;
            if (chessBoard[x, y].isEmpty())
            {
                chessBoard[x, y].player = p;
                return true;
            }
            return false;
        }
    }
}
