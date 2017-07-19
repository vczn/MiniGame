using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gobang
{
    class Global
    {
        public const int GRID_NUM = 15;
        public const int GRID_SIZE = 35;

        public const int EMPTY = 0;
        public const int PLAYER1 = 1;                                   //黑棋
        public const int PLAYER2 = 2;                                   //白棋
        public const int INF = 2147483647;

        public const int EVA_EMPTY = 0;
        public const int EVA_MY = 1;
        public const int EVA_OT = 2;

        //估值表
        public const int EVA_ZERO = 0;
        public const int EVA_ONE = 10;
        public const int EVA_ONE_S = 1;
        public const int EVA_TWO = 100;
        public const int EVA_TWO_S = 10;
        public const int EVA_THREE = 1000;
        public const int EVA_THREE_S = 100;
        public const int EVA_FOUR = 10000;
        public const int EVA_FOUR_S = 1000;
        public const int EVA_FIVE = 100000;


        public enum Mode{
            PVP, NORMAL, HARD,
        };

        public static Color Transparent = Color.FromArgb(0, 0, 0, 0);       //透明
        public static Image BackGround = Gobang.Properties.Resources.棋盘;
        public static Image Black = Gobang.Properties.Resources.black;
        public static Image White = Gobang.Properties.Resources.white;
        public static Image Select = Gobang.Properties.Resources.selected;
        public static Image Pre = Gobang.Properties.Resources.pre;

        public static bool bPlayer;
        public static void InitGobang()     // true 黑棋      false 白棋
        {
            bPlayer = true;
        }

        public const int ERROR_INDEX = -1;

        public static int coorToIndex(int coor)
        {
            int index = coor / GRID_SIZE;
            return (index >= 0 && index < GRID_NUM) ? index : ERROR_INDEX;
        }

        public static int indexToCoor(int index)
        {
            return index * GRID_SIZE;
        }
    }
}
