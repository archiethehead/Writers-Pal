using System;
using Avalonia;
using Writers_Pal.Models;

namespace Writers_Pal {

    internal sealed class Program {

        // Initialization code. Don't use any Avalonia, third-party APIs or any
        // SynchronizationContext-reliant code before AppMain is called: things aren't initialized
        // yet and stuff might break.
        [STAThread]
        public static void Main(string[] args) {

            ScriptProject newProj = new ScriptProject("A", "A", "D:\\Personal\\Programs\\CSharp\\Writers-Pal\\Assets", false);
            BuildAvaloniaApp().StartWithClassicDesktopLifetime(args);

        }

        // Avalonia configuration, don't remove; also used by visual designer.
        public static AppBuilder BuildAvaloniaApp()
            => AppBuilder.Configure<App>()
                .UsePlatformDetect()
                .WithInterFont()
                .LogToTrace();

    }

}