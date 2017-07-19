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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            MaximizeBox = false;
            MinimizeBox = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Visible = false;
            Setting settingForm = new Setting();
            settingForm.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Visible = false;
            GameUI gameUIForm = new GameUI();
            gameUIForm.Show();
            Settings.mode = (int)Global.Mode.PVP;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
