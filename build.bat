@echo off
chcp 65001 >nul
echo 🔧 Compiling Chess Engine...

setlocal

set "SCRIPT_DIR=%~dp0"
set "BIN_DIR=%SCRIPT_DIR%bin"
set "ENGINE_EXE=%BIN_DIR%\Engine.exe"

C:\msys64\ucrt64\bin\g++.exe ^
    -std=c++23 ^
    -O3 -march=native -flto -DNDEBUG ^
    "%SCRIPT_DIR%\core\main.cpp" ^
    "%SCRIPT_DIR%\core\AttackGlobals.cpp" ^
    -I "C:\msys64\ucrt64\include" ^
    -L "C:\msys64\ucrt64\lib" ^
    -lsfml-graphics -lsfml-window -lsfml-system ^
    -o "%ENGINE_EXE%"

IF %errorlevel% EQU 0 (
    echo ✅ Build succeeded!
    echo 🔄 Running Engine...

    pushd "%BIN_DIR%"

    IF "%1"=="" (
        call Engine.exe
    ) ELSE (
        call Engine.exe %*
    )

    popd
) ELSE (
    echo Build failed with error code %errorlevel%.
)

endlocal