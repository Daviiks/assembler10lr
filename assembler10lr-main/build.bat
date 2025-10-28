@echo off
chcp 65001 > nul

echo ==========================================
echo    Mixed ASM + C DLL (With Debug)
echo ==========================================

echo.
echo 1. Cleaning...
del *.obj 2>nul
del *.dll 2>nul
del *.exe 2>nul
del *.lib 2>nul
del *.exp 2>nul

echo.
echo 2. Compiling ASM part...
nasm -f win64 dll_asm.asm -o dll_asm.obj
if errorlevel 1 (
    echo ASM compilation failed!
    pause
    exit /b 1
)

echo.
echo 3. Compiling C part...
gcc -c dll_c.c -o dll_c.obj
if errorlevel 1 (
    echo C compilation failed!
    pause
    exit /b 1
)

echo.
echo 4. Linking DLL...
gcc -shared -o shared_mixed.dll dll_asm.obj dll_c.obj -lkernel32
if errorlevel 1 (
    echo DLL linking failed!
    pause
    exit /b 1
)

echo.
echo 5. Compiling main program...
gcc main.c -o main.exe
if errorlevel 1 (
    echo Main program compilation failed!
    pause
    exit /b 1
)

echo.
echo ==========================================
echo    BUILD SUCCESSFUL!
echo ==========================================

echo.
echo Starting processes in separate windows...
echo.

echo Starting Process 1 in new window...
start "Process 1" cmd /k "main.exe \"Process 1\""

timeout /t 3 /nobreak >nul

echo Starting Process 2 in new window...
start "Process 2" cmd /k "main.exe \"Process 2\""

echo.
echo Both processes are running in separate windows!
echo They will wait for Enter key press.
echo.
pause