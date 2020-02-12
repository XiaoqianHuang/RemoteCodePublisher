///////////////////////////////////////////////////////////////////////
// MainWindow.xmal.cs - GUI for users to navigate the directories and 
// files by patterns and regex, convert the selected files and display 
// them either in browser or new window.
// Language:    C++, Visual Studio 2019   
// Platform:      Macbook Pro , Win 10  
// Application: Project #4 - Remote Code Page Management - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                        xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is responsible for providing GUI for users to navigate 
* the directories and files by patterns and regex, convert the selected 
* files and display them either in browser or new window.
*
* Required Files:
* ---------------
*  ITranslator.h
*  SourceCodeWindow.xmal
*
* Maintenance History:
* --------------------
* * ver 2.0 : 30 Apr 2019
* - first release
* - modify to meet all the requiremnet in project#4
* ver 1.0 : 4 Feb 2019
* - first release
* - demonstrate the application meet all the requiremnet in project#3
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - More exception handle
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
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
using MsgPassingCommunication;

namespace WPF
{
    public partial class MainWindow : Window
    {
        string currentpath = "root";
        string dirroot = "";
        string patterns = "";
        string regex = "";
        bool subdir = false;
        string convertedfolderpath = "";
        string selectedfile = "";
        string selectedconvertedfile = "";
        string savePath = "";
        List<String> selectedFiles = new List<String>();
        List<String> convertedFiles = new List<String>();
        List<String> dirs = new List<String>();
        List<String> files = new List<String>();
        List<String> args = new List<String>();
        Translater translater;
        private CsEndPoint endPoint_;
        CsEndPoint serverEndPoint;
        private Thread rcvThread = null;
        private Thread demoThread = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();

    public MainWindow()
        {
            InitializeComponent();
        }

        //----< Initial the GUI >---------------------------------------------

        private void window_Loaded(object sender, RoutedEventArgs e)
        {
            // start Comm
            string[] args = Environment.GetCommandLineArgs();
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost"; 
            endPoint_.port = int.Parse(args[1]);
            savePath = args[3];
            translater = new Translater();
            translater.listen(endPoint_,savePath);

            // initialize dispatcher
            InitializeMessageDispatcher();

            // start processing message
            rcvThread = new Thread(rcvThreadProc);
            rcvThread.IsBackground = true;
            rcvThread.Start();

            // configure server information
            serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost"; 
            serverEndPoint.port = int.Parse(args[2]);

            // initialize user interface
            patterns = "*.h *.cpp";
            regex = "[A-Z](.*)";
            Patterns.Text = patterns;
            Regex.Text = regex;
            sendGetRootMsg();
            sendGetDirMsg();
            sendGetConvertedFolderMsg();
            sendGetFileMsg();

            if (args[4] == "demo1")
            {
                demoThread = new Thread(demo1);
                demoThread.IsBackground = true;
                demoThread.Start();
            }
            if (args[4] == "demo2")
            {
                demoThread = new Thread(demo2);
                demoThread.IsBackground = true;
                demoThread.Start();
            }
        }

        //----< demo for client 1 >---------------------------------------------
        void demo1()
        {
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                Console.Write("\n  Demonstrating Navigation...");
                MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                Options.IsChecked = true;
                Navigate_Click(this, e);
            }
            ));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                Console.Write("\n  Demonstrating Navigation...");
                MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                Options.IsChecked = false;
                Navigate_Click(this, e);
            }
            ));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
                {
                    if (SelectDir.Items.Count != 0)
                    {
                        Console.Write("\n  Demonstrating selecting dir...");
                        MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                        SelectDir.SelectedIndex = 0;
                        SelectDir_MouseDoubleClick(this, e);
                    }
                }
            ));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                    Console.Write("\n  Demonstrating Navigation...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    Navigate_Click(this, e);
            }
            ));

            demo1_1_2();
        }

        void demo1_1_2()
        {
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (Files.Items.Count > 1)
                {
                    Console.Write("\n  Demonstrating adding files...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    Files.SelectedIndex = 0;
                    AddConvertFile_Click(this, e);
                }
            }
));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (Files.Items.Count > 1)
                {
                    Console.Write("\n  Demonstrating adding files...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    Files.SelectedIndex = 1;
                    AddConvertFile_Click(this, e);
                }
            }
));
            demo1_2();
        }

        void demo1_2()
        {
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (SelectDir.Items.Count != 0)
                {
                    Console.Write("\n  Demonstrating selecting publish...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    Button_Click(this, e);
                }
            }
            ));
            System.Threading.Thread.Sleep(1000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                Console.Write("\n  Demonstrating changing tab...");
                Tab2.IsSelected = true;
            }
            ));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (SelectDir.Items.Count != 0)
                {
                    Console.Write("\n  Demonstrating full converted file list...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    FullConvertedFile.IsChecked = false;
                    DisplayFullList_Click(this, e);
                }
            }
            ));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (SelectDir.Items.Count != 0)
                {
                    Console.Write("\n  Demonstrating canceling full converted file list...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    FullConvertedFile.IsChecked = true;
                    DisplayFullList_Click(this, e);
                }
            }
            ));
            demo1_3();
        }

        void demo1_3()
        {
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (ConvertedList.Items.Count != 0)
                {
                    Console.Write("\n  Demonstrating downloading...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    ConvertedList.SelectedIndex = 0;
                    DownloadFile_Click(this, e);
                }
            }
            ));
        }

        //----< demo for client 2 >---------------------------------------------
        void demo2()
        {
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (SelectDir.Items.Count >1 )
                {
                    Console.Write("\n  Demonstrating selecting dir...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    SelectDir.SelectedIndex = 2;
                    SelectDir_MouseDoubleClick(this, e);
                }
            }
            ));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                Console.Write("\n  Demonstrating Navigation...");
                MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                Navigate_Click(this, e);
            }
            ));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (Files.Items.Count > 1)
                {
                    Console.Write("\n  Demonstrating adding files...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    Files.SelectedIndex = 0;
                    AddConvertFile_Click(this, e);
                }
            }
));
            System.Threading.Thread.Sleep(2000);
            App.Current.Dispatcher.Invoke(new Action(() =>
            {
                if (Files.Items.Count > 1)
                {
                    Console.Write("\n  Demonstrating adding files...");
                    MouseButtonEventArgs e = new MouseButtonEventArgs(Mouse.PrimaryDevice, 0, MouseButton.Left);
                    Files.SelectedIndex = 1;
                    AddConvertFile_Click(this, e);
                }
            }
));
            demo1_2();
        }

        //----< handle selected dir >---------------------------------------------

        private void SelectDir_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string tmp = currentpath;
            try
            {
                if (SelectDir.SelectedIndex == -1)
                    return;
                string selectedDir = null;
                selectedDir = SelectDir.SelectedItem.ToString(); // acquire the text of selected path
                if (selectedDir == "..")
                {
                    currentpath = Directory.GetParent(currentpath).ToString();
                }
                else
                {
                    currentpath = currentpath + "\\" + selectedDir;
                }
                CurrentPath.Text = currentpath;
                sendGetDirMsg();
                sendGetFileMsg();
            }
            catch(Exception exp)
            {
                Console.WriteLine("Some thing went wrong here!\n");
                currentpath = tmp;
                CurrentPath.Text = currentpath;
                sendGetDirMsg();
                sendGetFileMsg();
                Console.WriteLine(exp);
            }
        }

        //----< acquire the requirements seted by user and process navigation >---------------------------------------------

        private void Navigate_Click(object sender, RoutedEventArgs e)
        {
            patterns = Patterns.Text.ToString();
            regex = Regex.Text.ToString();
            if (Options.IsChecked == true)
                subdir = true;
            else
                subdir = false;
            sendGetFileMsg();
        }

        //----< handle selected file >---------------------------------------------

        private void AddConvertFile_Click(object sender, RoutedEventArgs e)
        {
            if (selectedfile != "")
            {
                if (!selectedFiles.Contains(selectedfile))
                {
                    selectedFiles.Add(currentpath+selectedfile);
                    ConvertList.Items.Add(selectedfile); //?? fullpath? just name??
                }
            }
        }

        //----< acquire and display all the html file in ConvertedPage Folder >---------------------------------------------

        private void DisplayFullList_Click(object sender, RoutedEventArgs e)
        {
            if(FullConvertedFile.IsChecked == true)
            {
                ConvertedList.Items.Clear();
                if (convertedFiles.Count != 0)
                {
                    foreach (var file in convertedFiles)
                    {
                        ConvertedList.Items.Add(file);
                    }
                }
            }
            else
            {
                sendGetAllConvertedFilesMsg();
            }
        }

        //----< clean all items in the listbox >---------------------------------------------

        private void CleanConvertFile_Click(object sender, RoutedEventArgs e)
        {
            ConvertList.Items.Clear();
            selectedFiles.Clear();
        }

        //----< process convertion of selected files >---------------------------------------------

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            List<String> cf = new List<string>();
            ConvertList.Items.Clear();
            ConvertList.Items.Add("( Please wait..)");
            if (selectedFiles.Capacity != 0)
            {
                foreach (var file in selectedFiles)
                {
                    cf.Add(file);
                }
                try
                {
                    sendPublishMsg(cf);
                }
                catch
                {
                    ConvertList.Items.Clear();
                    ConvertList.Items.Add("( Oops! Some thing wrong! [Do not publish this item] )");
                }
            }
            else
            {
                ConvertList.Items.Clear();
                ConvertList.Items.Add("( Please select a file to convert! [Do not publish this item] )");
            }
        }

        //----< download selected file >---------------------------------------------
        private void DownloadFile_Click(object sender, RoutedEventArgs e)
        {
            if (selectedconvertedfile != null)
            {
                sendReceiveFileMsg();
                try
                {
                    var path = System.IO.Path.GetFullPath(savePath);
                    System.Diagnostics.Process.Start(path);

                    string content = File.ReadAllText(path + "\\" + System.IO.Path.GetFileName(selectedconvertedfile));
                    SourceCodeWindow NewWindow = new SourceCodeWindow();
                    NewWindow.SourceCode.Text = content;
                    NewWindow.Show();

                    System.Diagnostics.Process.Start(path + "\\" + System.IO.Path.GetFileName(selectedconvertedfile));
                }
                catch
                {
                    Console.Write("\n  The indicated download file folder is not exist!");
                }
            }
            else
            {
                Console.Write("\n  No file is selected!");
            }
        }

        //----< handle selections in files listbox >---------------------

        private void Files_SelectionChanged( object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count == 0)
                selectedfile = "";
            try
            {
                string selStr = e.AddedItems[0].ToString();
                selectedfile = System.IO.Path.Combine(currentpath, selStr);
            }
            catch
            {
                Console.Write("\n Please select an item!");
            }
        }


        //----< handle selections in converted listbox >---------------------

        private void ConvertedFiles_SelectionChanged(object sender, SelectionChangedEventArgs e) {
            if (e.AddedItems.Count == 0)
                selectedconvertedfile = "";
            try
            {
                ConvertList.Items.Clear();
                string selStr = e.AddedItems[0].ToString();
                selectedconvertedfile = convertedfolderpath + "\\"+ selStr;
            }
            catch
            {
                Console.Write("\n Please select an item!");
            }
        }

        //----< tab control >---------------------

        private void MainWindowclick2_Click(object sender, RoutedEventArgs e)
        {

        }

        //----< create argument according to option in GUI >---------------------

        private void setArgs()
        {
            List<String> args_ = new List<string>();
            //args_.Add(currentpath);
            if(subdir == true)
            {
                args_.Add("/s");
            }
            else
            {
                args_.Add("/r");
            }
            if(patterns != "")
            {
                var ps = patterns.Split(' '); // seperate args
                foreach ( var p in ps)
                {
                    args_.Add(p);
                }
            }
            if (regex != "")
            {
                var re = regex.Split(' ');
                foreach (var r in re)
                {
                    args_.Add(r);
                }
            }
            args = args_;
        }

        //----< send message to get root dir>---------------------
        private void sendGetRootMsg()
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getRoot");
            translater.postMessage(msg);
        }

        //----< send message to get children dirs>---------------------
        private void sendGetDirMsg()
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", currentpath);
            translater.postMessage(msg);
        }

        //----< send message to get children files>---------------------
        private void sendGetFileMsg()
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getFiles");
            msg.add("path", currentpath);
            setArgs();
            var count = 0;
            foreach (var arg in args)
            {
                count++;
                msg.add("arg"+count, arg);
            }
            translater.postMessage(msg);
        }


        //----< send message to get children dirs>---------------------
        private void sendPublishMsg(List<string> convertList)
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "publish");
            var count = 0;
            foreach(var file in convertList)
            {
                count++;
                msg.add("filePath"+count, file);
            }
            translater.postMessage(msg);
        }

        //----< send message to get converted page folder dir>---------------------
        private void sendGetConvertedFolderMsg()
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getConvertedFolderDir");
            translater.postMessage(msg);
        }

        //----< send message to get all the converted files >---------------------
        private void sendGetAllConvertedFilesMsg()
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getAllConvertedFiles");
            translater.postMessage(msg);
        }

        //----< send message to recieve file >---------------------
        private void sendReceiveFileMsg()
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "receiveFile");
            msg.add("fileName", System.IO.Path.GetFileName(selectedconvertedfile));
            translater.postMessage(msg);
        }

        //----< send message to quit>---------------------
        private void sendQuitMsg()
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "serverQuit");
            translater.postMessage(msg);
        }

        //----< add client processing for message with key >---------------

        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }

        //----< function dispatched by child thread to main thread >-------

        private void clearDirs()
        {
            SelectDir.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addDir(string dir)
        {
            if(dir != ".." && dir != ".")
            {
                SelectDir.Items.Add(dir);
            }
        }
        //----< function dispatched by child thread to main thread >-------

        private void insertParent()
        {
            if(currentpath != dirroot)
            {
                SelectDir.Items.Insert(0, "..");
            }
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearFiles()
        {
            Files.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addFile(string file)
        {
            string temp = null;
            for (int i = currentpath.Length; i < file.Length; i++)
            {
                temp += file[i];
            }
            Files.Items.Add(temp);
        }

        //----< set path by child thread to main thread >-------

        private void setPath(string path)
        {
            CurrentPath.Text = path;
            currentpath = path;
        }

        //----< set result path by child thread to main thread >-------

        private void setResultPath(string path)
        {
            CurrentPath_.Text = path;
            convertedfolderpath = path;
        }

        //----< function dispatched by child thread to main thread >-------

        private void clearConvertedFiles()
        {
            ConvertedList.Items.Clear();
            ConvertList.Items.Clear();
            ConvertList.Items.Add("( Success!! Please go to \"Converted Files\" and check! [Do not publish this item] )");
        }

        //----< function dispatched by child thread to main thread >-------

        private void addConvertedFile(string file)
        {
            ConvertedList.Items.Add(file);
        }

        void rcvThreadProc()
        {
            Console.Write("\n  starting client's receive thread");
            while (true)
            {
                CsMessage msg = translater.getMessage();
                msg.show();

                // pass the Dispatcher's action value to the main thread for execution
                try
                {
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
                catch
                {
                    Console.Write("no command");
                }
            }
        }

        //----< Initialize the dictionary of dispatcher >---------------------------
        private void InitializeMessageDispatcher()
        {
            InitializeGetRoot();
            InitializeGetDirs();
            InitializeGetFiles();
            InitializeGetConvertedFolderDir();
            InitializeGetAllConvertedList();
            InitializeGetConvertedList();
            InitializeSendFile();
        }

        //----< Initialize action to get root dir  into dispatcher dictionary >---------------------------
        private void InitializeGetRoot()
        {
            Action<CsMessage> getRoot= (CsMessage rcvMsg) => // input CsMessage, no return
            {
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("root"))
                    {
                        dirroot = enumer.Current.Value;
                    }
                }
                Action<string> setRoot = (string path) =>
                {
                    setPath(path);
                };
                Dispatcher.Invoke(setRoot, new Object[] { dirroot });
            };
            addClientProc("getRoot", getRoot);
        }

        //----< initialize getDirs processing into dispatcher dictionary >-------

        private void InitializeGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                if (currentpath != dirroot)
                {
                    Action insertUp = () =>
                    {
                        insertParent();
                    };
                    Dispatcher.Invoke(insertUp, new Object[] { });
                }
            };
            addClientProc("getDirs", getDirs);
        }
        //----< initialize getFiles processing into dispatcher dictionary >------

        private void InitializeGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }

        //----< Initialize action to get converted folder dir into dispatcher dictionary >------

        private void InitializeGetConvertedFolderDir()
        {
            Action<CsMessage> getConvertedFolderDir = (CsMessage rcvMsg) =>
            {
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("convertedFolderDir"))
                    {
                        convertedfolderpath = enumer.Current.Value;
                    }
                }
                Action<string> setFolderDir = (string path) =>
                {
                    setResultPath(path);
                };
                Dispatcher.Invoke(setFolderDir, new Object[] { convertedfolderpath });
            };
            addClientProc("getConvertedFolderDir", getConvertedFolderDir);
        }

        //----< initialize GetAllConvertedList processing into dispatcher dictionary >------

        private void InitializeGetAllConvertedList()
        {
            Action<CsMessage> getAllConvertedList = (CsMessage rcvMsg) =>
            {
                Action clrConvertedFiles = () =>
                {
                    clearConvertedFiles();
                };
                Dispatcher.Invoke(clrConvertedFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("allConvertedFileName"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addConvertedFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getAllConvertedList", getAllConvertedList);
        }

        //----< initialize GetConvertedList processing into dispatcher dictionary >------

        private void InitializeGetConvertedList()
        {
            Action<CsMessage> getConvertedList = (CsMessage rcvMsg) =>
            {
                convertedFiles.Clear();
                Action clrConvertedFiles = () =>
                {
                    clearConvertedFiles();
                };
                Dispatcher.Invoke(clrConvertedFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("convertedFile"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addConvertedFile(file);
                            convertedFiles.Add(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getConvertedList", getConvertedList);
        }

        //----< Initialize action to recieve files into dispatcher dictionary >---------------------------
        private void InitializeSendFile()
        {
            Action<CsMessage> sendFile = (CsMessage rcvMsg) => // input CsMessage, no return
            {
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("sendingFile"))
                    {
                        //recieve file
                        Action<string> doFile = (string sendingFile) =>
                        {
                            Console.Write("\n  File {0} has been downloaded!", selectedconvertedfile);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("sendFile", sendFile);
        }
    }
}
