cd Debug
start Server.exe 9090 ../ConvertedPage
cd ..
cd WPF
start WPF.exe 9092 9090 ../SavePath demo2
start WPF.exe 9091 9090 ../SavePath demo1
pause

