
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

./cl.bat ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt" ^
/I "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\include" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um" ^
/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared" ^
/Qspectre- ^
/EHsc /GA /MT ^
./src/main.c ^
-Wall

REM chmod +x ./demo

echo done compiling