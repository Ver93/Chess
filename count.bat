@echo off
setlocal enabledelayedexpansion

:: File name in same directory
set "filePath=c.txt"

:: Check if file exists
if not exist "%filePath%" (
    echo File not found: %filePath%
    goto :eof
)

:: Sort and overwrite original file
set "tempFile=%TEMP%\sorted_lines.txt"
sort "%filePath%" > "%tempFile%"
copy /Y "%tempFile%" "%filePath%" >nul

set "prevLine="
set /a totalLines=0
set /a duplicateLines=0
set /a uniqueLines=0

for /f "usebackq delims=" %%A in ("%filePath%") do (
    set "currentLine=%%A"
    set /a totalLines+=1

    if "!currentLine!"=="!prevLine!" (
        set /a duplicateLines+=1
    ) else (
        set /a uniqueLines+=1
    )
    set "prevLine=!currentLine!"
)

echo.
echo File: %filePath% (now sorted)
echo --------------------------
echo Total lines     : %totalLines%
echo Unique lines    : %uniqueLines%
echo Duplicate lines : %duplicateLines%
echo --------------------------

:: Clean up
del "%tempFile%" >nul 2>&1
endlocal
pause