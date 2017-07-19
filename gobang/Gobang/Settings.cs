using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gobang
{
    class Settings
    {
        public static int mode;            //选择AI难度

        public static void InitSetting()
        {
            mode = (int)Global.Mode.NORMAL;
            Game.MaxDepth = 2;
        }
    }
}
