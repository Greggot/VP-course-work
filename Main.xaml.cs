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
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Windows.Threading;

namespace kyrs
{
    /// <summary>
    /// Логика взаимодействия для Main.xaml
    /// </summary>
    public partial class Main : Window
    {
        public int R = 32, G = 120, B = 240;
        private string OpenText;
        public List<string> lst = new List<string>();
        public Main StartName;
        public int SumR=1, SumG=1, SumB=0;
        public DispatcherTimer myThr;

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                this.DragMove();
            }
        }
        private void EvTick(Object sender, EventArgs e) 
        {
            byte nR = 0x00, nG = 0x00, nB = 0x00;

                if (StartName.R > 240)
                {
                    StartName.SumR = -1;
                }
                else if (StartName.R < 150 & StartName.SumR == -1)
                {
                    StartName.SumR = 0;
                    StartName.SumG = 1;
                }
                StartName.R += SumR;
                nR = Convert.ToByte(StartName.R);

                if (StartName.G > 240)
                {
                    StartName.SumG = -1;
                }
                else if (StartName.G < 150 & StartName.SumG == -1)
                {
                    StartName.SumG = 0;
                    StartName.SumB = 1;
                }
                StartName.G += SumG;
                nG = Convert.ToByte(StartName.G);


                if (StartName.B > 240)
                {
                    StartName.SumB = -1;
                }
                else if (StartName.B < 150 & StartName.SumB == -1)
                {
                    StartName.SumB = 0;
                    StartName.SumR = 1;
                }
                StartName.B += SumB;
                nB = Convert.ToByte(StartName.B);

                StartName.Background = new SolidColorBrush(Color.FromArgb(0xFF, nR, nG, nB));
                
        }
       
        public Main()
        {
            StartName = this;
            InitializeComponent();
            myThr = new DispatcherTimer();
            myThr.Tick += new EventHandler(EvTick);
            myThr.Interval = new TimeSpan(0, 0, 0, 0, 7);
            myThr.Start();
            /*myThr = new Timer(new ThreadStart(() =>
                {
                    
                }
                ));
            myThr.Start();
            */
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();


            Microsoft.Win32.OpenFileDialog dialog = new Microsoft.Win32.OpenFileDialog();

                    if (dialog.ShowDialog() == true)
                    {
                        OpenText = dialog.FileName;
                        MainWindow Setings = new MainWindow(OpenText, this);
                        Setings.Show();
                    }
                    else
                        this.Show();
               
            }
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            this.Hide();

            CreateComand Create = new CreateComand(this);
            Create.Show();
        }

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
        }

        private void minButton_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.Close();
        }
        private void ExitButton_MouseDown(object sender, MouseButtonEventArgs e)
        {
            this.WindowState = WindowState.Minimized;
        }
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            while(lst.Count>0)
            lst.RemoveAt(0);
            comands.Text = "";
            IDs.Items.Clear();
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            string saveTXT = "";
            this.Hide();
            Microsoft.Win32.OpenFileDialog OpenDialog = new Microsoft.Win32.OpenFileDialog();
            if (OpenDialog.ShowDialog() == true)
            { 
                OpenText = OpenDialog.FileName;
                Microsoft.Win32.SaveFileDialog saveDialog = new Microsoft.Win32.SaveFileDialog();
                if (saveDialog.ShowDialog() == true)
                {
                    saveTXT = saveDialog.FileName;
                    string                                      massegeConsol = "";
                    massegeConsol += OpenText;                  massegeConsol += " ";
                    massegeConsol += lst.Count;                 massegeConsol += " ";
                    for (int i = 0; i < lst.Count; i++)
                    { massegeConsol += lst[i];                  massegeConsol += " "; }
                    massegeConsol += adress.Content;            massegeConsol += " ";
                    massegeConsol += saveTXT;                   massegeConsol += " ";
                    massegeConsol += IDs.SelectedItem;          massegeConsol += " ";
                    var proc = new ProcessStartInfo()
                    {
                        FileName = @"SPNParser.exe",
                        Arguments = massegeConsol,
                    };
                    Process.Start(proc);
                    this.Show();
                }
                else
                    this.Show();   
            }
            else
                this.Show();
            
        }
    }
}
