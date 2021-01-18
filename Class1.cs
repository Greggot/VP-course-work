using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Media;
using System.Threading;

namespace kyrs
{
    class Class1
    {
        private string[] mass;
        public int subStringCounter = 0;

        public Class1(string a)
        {
            string BuffPtr = File.ReadLines(a).First();

            mass = BuffPtr.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            subStringCounter = mass.Length;
        
        }
        public string get_key(int a)
        {
            return mass[a];
        }
    }
}
