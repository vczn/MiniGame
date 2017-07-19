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
    public partial class Setting : Form
    {
        public Setting()
        {
            InitializeComponent();
            MaximizeBox = false;
            Settings.mode = (int)Global.Mode.NORMAL;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Visible = false;
            GameUI gameUIForm = new GameUI();
            gameUIForm.Show();
        }

      
        private void groupBox2_Enter(object sender, EventArgs e)
        {
            
        }

        

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            Settings.mode = (int)Global.Mode.NORMAL;
            Game.MaxDepth = 2;
        }

        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            Settings.mode = (int)Global.Mode.HARD;
            Game.MaxDepth = 3;
        }
    }
}
