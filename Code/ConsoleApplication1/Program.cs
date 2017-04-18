using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            const string file = @"file.txt";
            const string target = @"C:\Users\raphaelgamer\Desktop\file2.txt";

            try
            {

                string exeDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                exeDir = Path.Combine(exeDir, "music2");
                string content = File.ReadAllText(file);
                StringBuilder stringBuilder = new StringBuilder();
                string[] lines = content.Split(new string[] { Environment.NewLine }, StringSplitOptions.None);
                foreach (string line in lines)
                {
                    string[] s = line.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                    if (s.Length > 0)
                    {
                        string url = s[0].Replace("\"", "");
                        //url = "https://www.youtube.com/watch?v=Um7pMggPnug&index=18&list=PLw-VjHDlEOgtl4ldJJ8Arb2WeSlAyBkJS";
                        Directory.SetCurrentDirectory(exeDir);
                        var procStIfo = new ProcessStartInfo("youtube-dl.exe", string.Format("--no-playlist -x --audio-format mp3 --audio-quality 192 {0}", url));
                        procStIfo.RedirectStandardOutput = true;
                        procStIfo.UseShellExecute = false;
                        procStIfo.CreateNoWindow = true;
                        procStIfo.RedirectStandardOutput = true;

                        var proc = new Process { StartInfo = procStIfo };
                        proc.OutputDataReceived += p_OutputDataReceived;
                        proc.Start();
                        proc.BeginOutputReadLine();
                        proc.WaitForExit();

                        stringBuilder.AppendLine(url);
                    }
                }
                //string[] music = Directory.GetFiles(".", "*", SearchOption.AllDirectories);
                ////string[] mkvFiles= Directory.GetFiles(".", "*.mp4", SearchOption.AllDirectories);
                //foreach (string file1 in music)
                //{
                //    if (file1.Contains(".exe"))
                //    {
                //        continue;
                //    }

                //    Console.WriteLine("Converting file : ", file1);

                //    Directory.SetCurrentDirectory(exeDir);
                //    string file2 = Path.GetFileName(file1);
                //    var procStIfoConverter = new ProcessStartInfo("ffmpeg.exe", string.Format("-i \"{0}\" -vn -f mp3 -ab 192k \"{0}\".mp3", file2));
                //    procStIfoConverter.RedirectStandardOutput = true;
                //    procStIfoConverter.UseShellExecute = false;
                //    procStIfoConverter.CreateNoWindow = true;
                //    procStIfoConverter.RedirectStandardOutput = true;

                //    var procConveter = new Process { StartInfo = procStIfoConverter };
                //    procConveter.OutputDataReceived += p_OutputDataReceived;
                //    procConveter.Start();
                //    procConveter.BeginOutputReadLine();
                //    procConveter.WaitForExit();
                //}
                File.WriteAllText(target, stringBuilder.ToString());

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            Console.WriteLine("Finish");
            Console.ReadLine();

        }

        private static void p_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            Console.Write(e.Data);
        }
    }
}
