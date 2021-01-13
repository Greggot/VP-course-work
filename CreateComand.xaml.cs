using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Threading;

namespace kyrs
{
    /// <summary>
    /// Логика взаимодействия для CreateComand.xaml
    /// </summary>
    /// 
    
    public partial class CreateComand : Window
    {
        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                this.DragMove();
            }
        }
        private Main starter;
        private Thread myThr;

        private void collor()
        {
            while (true)
            {
                byte nR = Convert.ToByte(starter.R);
                byte nG = Convert.ToByte(starter.G);
                byte nB = Convert.ToByte(starter.B);
                Action action = () => this.Background = new SolidColorBrush(Color.FromArgb(0xFF, nR, nG, nB));
                if (!Dispatcher.CheckAccess())
                    Dispatcher.Invoke(action);
                else
                    break;
                Thread.Sleep(1);
            }
        }
        public CreateComand(Main a)
        {
            starter = a;
            InitializeComponent();
            myThr = new Thread(new ThreadStart(collor));
            myThr.Start();
        }

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            myThr.Abort();
            starter.Show();
            this.Close();
        }

        private void minButton_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.Close();
        }
        private void ExitButton_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.WindowState = WindowState.Minimized;
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            int time;
            bool provint1 = int.TryParse(text3.Text,out time);
            bool provint2 = int.TryParse(text4.Text, out time);

            if (provint1 && provint2)
            {
                string massej = "";
                starter.IDs.Items.Add(text2.Text);
                massej += text1.Text; massej += ' ';
                massej += text2.Text; massej += ' ';
                massej += text3.Text; massej += ' ';
                massej += text4.Text; massej += ' ';
                massej += text5.Text; massej += ' ';
                starter.lst.Add(massej);
                starter.comands.Text += massej;
                starter.comands.Text += '\n';
                this.Close();
                starter.Show();
            }
        }
    }
}
