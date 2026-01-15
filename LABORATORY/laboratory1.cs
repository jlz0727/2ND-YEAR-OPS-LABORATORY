using System;
using System.Diagnostics;

class Program
{
    static void Main(string[] args)
    {
        int pid;

        Process p = new Process();
        p.StartInfo.FileName = "whoami";
        p.StartInfo.UseShellExecute = false;

        pid = p.Start() ? 1 : -1;

        if (pid < 0)
        {
            Console.WriteLine("fork failed");
        }
        else if (pid == 0)
        {
        }
        else
        {
            Console.WriteLine("\n Process ID is: {0}\n",
                Process.GetCurrentProcess().Id);

            p.WaitForExit();
        }
    }
}
