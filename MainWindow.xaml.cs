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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Threading;

namespace kyrs
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
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
        public MainWindow(string n,Main b)
        {
            InitializeComponent();
            myThr = new Thread(new ThreadStart(collor));
            myThr.Start();
            starter = b;
            Class1 ReedFile = new Class1(n);
            for (int i = 0; i < ReedFile.subStringCounter; i++)
            {
                one.Items.Add(ReedFile.get_key(i));
                two.Items.Add(ReedFile.get_key(i));
                tree.Items.Add(ReedFile.get_key(i));
                fora.Items.Add(ReedFile.get_key(i));
            }
            /*//////////////////////////*/
      
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

            string add = "0 ";

            add += fora.SelectedIndex; add += " ";

            add += tree.SelectedIndex; add += " ";

            add += two.SelectedIndex; add += " "; 
            
            add += one.SelectedIndex; add += " ";


            starter.adress.Content = add;
            starter.Show();
            this.Close();
        }
    }
}
