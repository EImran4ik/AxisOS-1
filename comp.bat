@echo off
set "MINGW_PATH=C:\MinGW\bin"
set "PATH=%PATH%;%MINGW_PATH%"
set "BIN_DIR=C:\Users\Asus\Downloads\AxisOS\AxisOS's bin files"

if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

echo [1/3] Compiling Stage 1...
nasm -f bin AxisOS-Bootloader-Stage1.asm -o "%BIN_DIR%\stage1.bin"
if %errorlevel% neq 0 goto error

echo [2/3] Compiling Stage 2...
nasm -f elf32 AxisOS-Bootloader-Stage2.asm -o "%BIN_DIR%\stage2.o"
if %errorlevel% neq 0 goto error

echo [3/3] Compiling Kernel...
"%MINGW_PATH%\gcc.exe" -m32 -ffreestanding -nostdlib -fno-asynchronous-unwind-tables -c kernel.c -o "%BIN_DIR%\kernel.o"
if %errorlevel% neq 0 goto error

echo [4/3] Linking...
cd "%BIN_DIR%"
"%MINGW_PATH%\ld.exe" -m i386pe -Ttext 0x8000 --section-alignment 0x1000 -o kernel.elf stage2.o kernel.o
if %errorlevel% neq 0 goto error

"%MINGW_PATH%\objcopy.exe" -O binary kernel.elf kernel.bin
if %errorlevel% neq 0 goto error

copy /b stage1.bin + kernel.bin AxisOS.bin

echo.
echo =====================================
echo SUCCESS! AxisOS.bin created!
echo =====================================
dir AxisOS.bin
echo =====================================
pause
exit

:error
echo.
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
echo ERROR: Compilation failed!
echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
pause