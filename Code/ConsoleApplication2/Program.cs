using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication2
{
    class Program
    {

        static readonly Random Rand = new Random(31);

        static void Main(string[] args)
        {
            const string file = @"C:\Users\raphaelgamer\Desktop\Mappe1.csv";
            const string target = @"C:\Users\raphaelgamer\Desktop\Mappe3.csv";
            try
            {
                StringBuilder stringBuilder = new StringBuilder();
                string content = File.ReadAllText(file);
                string[] lines = content.Split(new string[] { Environment.NewLine }, StringSplitOptions.None);
                foreach (string line in lines)
                {
                    string currentColumn = string.Empty;
                    string[] columns = line.Split(new[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                    foreach (string column in columns)
                    {
                        currentColumn += string.Format("{0};", column);
                        try
                        {
                            double val = Convert.ToDouble(column.Replace(",", "."), CultureInfo.InvariantCulture);
                            val = GetRandom(val*1.001, val*1.002);

                           currentColumn =  currentColumn.Replace(column, val.ToString(CultureInfo.InvariantCulture).Replace(".", ","));

                        }
                        catch (Exception) { }
                    }

                    stringBuilder.AppendLine(currentColumn);

                }

                File.WriteAllText(target, stringBuilder.ToString());
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }


        public static double GetRandom(double min, double max)
        {
            return Rand.NextDouble() * (max - min) + min;
        }
    }
}
