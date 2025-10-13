
using Microsoft.VisualStudio.LanguageServer.Client;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Internal.FileEnumerationService;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.Threading;
using Microsoft.VisualStudio.Utilities;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Reflection;
using System.Runtime.Remoting.Contexts;
using System.Text;
using System.Text.Json;
using System.Text.Json.Nodes;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace gpulang_vsix
{
    public static class JsonCompat
    {
        public static object Convert(JsonElement elem)
        {
            switch (elem.ValueKind)
            {
                case JsonValueKind.Object:
                    var dict = new Dictionary<string, object>();
                    foreach (var prop in elem.EnumerateObject())
                    {
                        dict[prop.Name] = Convert(prop.Value);
                    }
                    return dict;

                case JsonValueKind.Array:
                    return elem.EnumerateArray().Select(Convert).ToList();

                case JsonValueKind.String:
                    return elem.GetString();

                case JsonValueKind.Number:
                    if (elem.TryGetInt64(out var l)) return l;
                    if (elem.TryGetDouble(out var d)) return d;
                    return elem.GetRawText();

                case JsonValueKind.True: return true;
                case JsonValueKind.False: return false;
                case JsonValueKind.Null: return null;

                default: return null;
            }
        }
    }

    public class GPULangContentDefinition
    {
        [Export]
        [Name("gpulang")]
        [BaseDefinition(CodeRemoteContentDefinition.CodeRemoteContentTypeName)]
        internal static ContentTypeDefinition BarContentTypeDefinition;

        [Export]
        [FileExtension(".gpul")]
        [ContentType("gpulang")]
        internal static FileExtensionToContentTypeDefinition BarFileExtensionDefinition;

        [Export]
        [FileExtension(".gpuh")]
        [ContentType("gpulang")]
        internal static FileExtensionToContentTypeDefinition FooFileExtensionDefinition;
    }

    [ContentType("gpulang")]
    [Export(typeof(ILanguageClient))]
    public class GPULangLanguageClient : ILanguageClient
    {
        public string Name => "GPULang Extension";

        private System.Diagnostics.Process process;

        public IEnumerable<string> ConfigurationSections => null;

        object Config;
        String Root;
        public object InitializationOptions => new
        {
            config = this.Config
        };

        public IEnumerable<string> FilesToWatch => null;

        public bool ShowNotificationOnInitializeFailed { get { return true; } }

        public event AsyncEventHandler<EventArgs> StartAsync;
        public event AsyncEventHandler<EventArgs> StopAsync;


        public async Task<Connection> ActivateAsync(CancellationToken token)
        {
            await ThreadHelper.JoinableTaskFactory.SwitchToMainThreadAsync(token);
            await Task.Yield();
            bool isDebugging = System.Diagnostics.Debugger.IsAttached;

            var solutionService = (IVsSolution)Package.GetGlobalService(typeof(SVsSolution));

            solutionService.GetSolutionInfo(
                out string solutionDirectory,
                out string solutionFileName,
                out string userOptsFile
            );
            string fullFilePath = Path.Combine(solutionDirectory, "gpulang_config.json");
            if (File.Exists(fullFilePath))
            {
                this.Config = JsonCompat.Convert(JsonDocument.Parse(File.ReadAllText(fullFilePath)).RootElement);
            }
            this.Root = solutionDirectory;

            if (isDebugging)
            {
                NamedPipeClientStream stream = new NamedPipeClientStream(".", "gpulang_socket", PipeDirection.InOut, PipeOptions.Asynchronous);
                await stream.ConnectAsync(token);
                return new Connection(stream, stream);
            }
            else
            {
                ProcessStartInfo info = new ProcessStartInfo();
                info.FileName = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), @"gpulang_server.exe");
                info.CreateNoWindow = true;
                info.RedirectStandardOutput = true;
                info.RedirectStandardError = true;
                info.RedirectStandardInput = true;
                info.UseShellExecute = false;
                var tcs = new TaskCompletionSource<bool>(TaskCreationOptions.RunContinuationsAsynchronously);

                this.process = new System.Diagnostics.Process();
                this.process.StartInfo = info;

                this.process.OutputDataReceived += (sender, e) =>
                {
                    if (e.Data != null)
                    {
                        Debug.WriteLine("[Server] " + e.Data);

                        // Check for ready message
                        if (e.Data.Contains("Waiting for client..."))
                        {
                            tcs.TrySetResult(true);
                        }
                    }
                };

                this.process.ErrorDataReceived += (sender, e) =>
                {
                    if (e.Data != null)
                        Debug.WriteLine("[Server ERROR] " + e.Data);
                };

                if (this.process.Start())
                {
                    this.process.BeginOutputReadLine();
                    this.process.BeginErrorReadLine();
                    using (token.Register(() => tcs.TrySetCanceled()))
                    {
                        await tcs.Task;
                    }

                    NamedPipeClientStream stream = new NamedPipeClientStream(".", "gpulang_socket", PipeDirection.InOut, PipeOptions.Asynchronous);
                    await stream.ConnectAsync(token);
                    return new Connection(stream, stream);
                }

                return null;
            }
        }

        public async Task OnLoadedAsync()
        {
            await StartAsync.InvokeAsync(this, EventArgs.Empty);
        }

        public Task OnServerInitializeFailedAsync(Exception e)
        {
            return Task.CompletedTask;
        }

        public Task OnServerInitializedAsync()
        {
            return Task.CompletedTask;
        }
        public Task<InitializationFailureContext> OnServerInitializeFailedAsync(ILanguageClientInitializationInfo initializationState)
        {
            string value = initializationState.InitializationException.ToString();
            return Task.FromResult(new InitializationFailureContext
            {
                FailureMessage = value
            });
        }
    }
}
