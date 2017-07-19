using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Gobang
{
    public partial class GameUI : Form
    {
        Game game = new Game();
        Bitmap canvas;
        Bitmap board;
        Bitmap flag;
        Bitmap preset;

        Graphics g;
        Graphics gBoard;
        Graphics gFlag;
        Graphics gPre;

        public GameUI()
        {
            InitializeComponent();
            MaximizeBox = false;

            canvas = new Bitmap(chessBoard.ClientSize.Width, chessBoard.ClientSize.Height);
            board = new Bitmap(chessBoard.ClientSize.Width, chessBoard.ClientSize.Height);
            flag = new Bitmap(chessBoard.ClientSize.Width, chessBoard.ClientSize.Height);
            preset = new Bitmap(chessBoard.ClientSize.Width, chessBoard.ClientSize.Height);

            g = Graphics.FromImage(canvas);
            gBoard = Graphics.FromImage(board);
            gFlag = Graphics.FromImage(flag);
            gPre = Graphics.FromImage(preset);

            chessBoard.MouseMove += new System.Windows.Forms.MouseEventHandler(chessBoard_MouseMove);
            chessBoard.MouseDown += new System.Windows.Forms.MouseEventHandler(chessBoard_MouseDown);
            restart();
        }

        private void DrawSideStatus()
        {
            if (game.getCurPlayer() == 1)
            {
                pictureBox2.Image = Global.Black;
                label1.Text = "轮到黑棋落子";
            }
            else if (game.getCurPlayer() == 2)
            {
                pictureBox2.Image = Global.White;
                label1.Text = "轮到白棋落子";
            }
        }

        private void restart()
        {
            game.startGame();
            Global.InitGobang();
            DrawSideStatus();

            gBoard.Clear(Global.Transparent);
            gPre.Clear(Global.Transparent);
            Draw();

            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            restart();
        }

        private void GameUI_Load(object sender, EventArgs e)
        {
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void chessBoard_MouseMove(object sender, MouseEventArgs e)
        {
            if (game.bGameStart())
            {
                int x = Global.coorToIndex(e.X);
                int y = Global.coorToIndex(e.Y);

                gFlag.Clear(Global.Transparent);
                gFlag.DrawImage(Global.Select, Global.indexToCoor(x) + 10, Global.indexToCoor(y) + 10);
                Draw();
            }
        }

        private void playerPlacePiece(int x, int y)
        {
            
            int curPlayer = game.getCurPlayer();
            if (game.placePiece(x, y))
            {
                DrawPiece(curPlayer, x, y);
                gPre.Clear(Global.Transparent);
                gPre.DrawImage(Global.Pre, Global.indexToCoor(x) + 10, Global.indexToCoor(y) + 10);
                Draw();

                game.chessBoard.chessBoard[x, y].player = curPlayer;
                if (game.bGameOver())
                {
                    string WinInfo = "黑棋获胜";
                    if (MessageBox.Show(WinInfo, "Game Over", MessageBoxButtons.OK) == DialogResult.OK)
                    {
                        restart();
                    }
                    return;
                }
            }
            DrawSideStatus();
        }


        private int nextPlayer(int p)
        {
            return p == Global.PLAYER1 ? Global.PLAYER2 : Global.PLAYER1;
        }
        private void chessBoard_MouseDown(object sender, MouseEventArgs e)
        {
            if (game.bGameStart())
            {
                int x = Global.coorToIndex(e.X);
                int y = Global.coorToIndex(e.Y);

                int curPlayer = game.getCurPlayer();

                if (Settings.mode == (int)Global.Mode.PVP)
                {
                    playerPlacePiece(x, y);
                }
                else
                {

                    if (game.placePiece(x, y))
                    {
                        DrawPiece(curPlayer, x, y);
                        gPre.Clear(Global.Transparent);
                        gPre.DrawImage(Global.Pre, Global.indexToCoor(x) + 10, Global.indexToCoor(y) + 10);
                        Draw();

                        int value = 0;
                        value = game.evaluateState(game.chessBoard, Global.PLAYER1);
                        if (game.bGameOver())
                        {
                            string WinInfo = "黑棋获胜";
                            if (MessageBox.Show(WinInfo, "Game Over", MessageBoxButtons.OK) == DialogResult.OK)
                            {
                                restart();
                            }
                            return;
                        }
                        DrawSideStatus();

                        //AI
                        Point pointAI = game.AIPlacePiece();
                        DrawPiece(nextPlayer(curPlayer), pointAI.X, pointAI.Y);
                        gPre.Clear(Global.Transparent);
                        gPre.DrawImage(Global.Pre, Global.indexToCoor(pointAI.X) + 10, Global.indexToCoor(pointAI.Y) + 10);
                        Draw();
                        if (game.bGameOver())
                        {
                            string WinInfo = "白棋获胜";
                            if (MessageBox.Show(WinInfo, "Game Over", MessageBoxButtons.OK) == DialogResult.OK)
                            {
                                restart();
                            }
                            return;
                        }

                        DrawSideStatus();
                    }
                }
            }
            
            
        }
        private void DrawPiece(int curPlayer, int x, int y)
        {
            if (curPlayer == Global.PLAYER1)
            {
                gBoard.DrawImage(Global.Black, Global.indexToCoor(x) + 10, Global.indexToCoor(y) + 10, 31, 31);
            }
            else
            {
                gBoard.DrawImage(Global.White, Global.indexToCoor(x) + 10, Global.indexToCoor(y) + 10, 31, 31);
            }
        }

        private void Draw()
        {
            g.DrawImage(Global.BackGround, 0, 0);
            g.DrawImage(board, 0, 0);
            g.DrawImage(flag, 0, 0);
            g.DrawImage(preset, 0, 0);

            chessBoard.BackgroundImage = canvas;

            chessBoard.CreateGraphics().DrawImage(canvas, 0, 0);
        }
    }
}
