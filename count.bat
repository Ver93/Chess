@echo off
setlocal EnableDelayedExpansion

:: Set the folder to search — current directory
set "folder=%CD%/core"
set /a totalLines=0

:: Check if "sub" argument is passed
if /i "%~1"=="sub" (
    echo Counting only in current folder: "%folder%"
    
    :: Count .cpp files in current folder only
    for %%F in ("%folder%\*.cpp") do (
        for /f %%L in ('type "%%F" ^| find /v /c ""') do (
            set /a totalLines+=%%L
        )
    )

    :: Count .h files in current folder only
    for %%F in ("%folder%\*.h") do (
        for /f %%L in ('type "%%F" ^| find /v /c ""') do (
            set /a totalLines+=%%L
        )
    )
) else (
    echo Counting recursively in "%folder%" and subfolders

    :: Count .cpp files recursively
    for /R "%folder%" %%F in (*.cpp) do (
        for /f %%L in ('type "%%F" ^| find /v /c ""') do (
            set /a totalLines+=%%L
        )
    )

    :: Count .h files recursively
    for /R "%folder%" %%F in (*.h) do (
        for /f %%L in ('type "%%F" ^| find /v /c ""') do (
            set /a totalLines+=%%L
        )
    )
)

echo Total lines in .cpp and .h files: %totalLines%