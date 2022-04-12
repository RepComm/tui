./cl.bat ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt" ^
/I "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\include" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared" ^
/I "C:\Program Files (x86)\Windows Kits\10\Lib" ^
./src/main.c -Wall -o ./demo.exe

@REM chmod +x ./demo

echo done compiling