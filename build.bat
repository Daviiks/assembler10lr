@echo off
chcp 65001 > nul

echo Cleaning previous build...
del *.obj 2>nul
del *.dll 2>nul
del *.exe 2>nul

echo Building 64-bit DLL...
nasm -f win64 dllmain.asm -o dllmain.obj
if errorlevel 1 (
    echo ERROR: NASM compilation failed
    pause
    exit /b 1
)

echo Linking DLL...
gcc -shared -o dllmain.dll dllmain.obj
if errorlevel 1 (
    echo ERROR: Linking failed
    pause
    exit /b 1
)

echo Building main program...
gcc main.c -o main.exe
if errorlevel 1 (
    echo ERROR: C compilation failed
    pause
    exit /b 1
)

echo Build successful!
echo.
echo Running program...
main.exe

pause