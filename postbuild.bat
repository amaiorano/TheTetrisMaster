@echo off

echo # Running post-build script...

rem *** Input param 1 is current directory
echo # Current directory is %1

rem *** Input param 2 is configuration (debug or release)
echo # Current configuration is %2


rem *** Set local variables
setlocal

set CFG=%2
if "%CFG%"=="" goto :ERROR

set COPY_CMD=xcopy.exe /S /R /K /Y /D

set DATA_DIR=.\Data
set OUTPUT_DIR=".\output\The Tetris Master"
set THEME_DIR=%OUTPUT_DIR%\Themes

set TETRIS_EXE=".\Tetris\%CFG%\Tetris.exe"
set LAUNCHER_EXE=".\ThemeLauncher\%CFG%\TTM Theme Launcher.exe"
set EDITOR_EXE=".\ThemeEditor\%CFG%\TTM Theme Editor.exe"
set AUDIERE_DLL=".\GSGameLib\gsgamelib\bin\audiere.dll"

set README_TXT=".\Tetris\ReadMe.txt"

rem *** Store actual current directory go to input one
pushd %1

echo # Creating output directory: %OUTPUT_DIR%
if not exist %OUTPUT_DIR% md %OUTPUT_DIR%

rem *** Always copy themes
goto :COPY_DATA
echo # Copying data to output directory
rem *** Only copy data if folders not present in output directory
if not exist %THEME_DIR% goto :COPY_DATA
echo # Data already exists in output directory, skipping
goto :END_COPY_DATA

:COPY_DATA
if not exist %THEME_DIR% md %THEME_DIR%
%COPY_CMD% %DATA_DIR% %OUTPUT_DIR%
if not "%errorlevel%"=="0" goto :ERROR
:END_COPY_DATA

echo # Copying executables and dlls
rem *** Because VS.NET does not have an option to ALWAYS run custom build-events,
rem *** we have to run this script for each project. So don't check for errors
rem *** because some of these will fail depending on the order the script is called.
%COPY_CMD% %TETRIS_EXE% %OUTPUT_DIR%
rem	if not "%errorlevel%"=="0" goto :ERROR

%COPY_CMD% %LAUNCHER_EXE% %OUTPUT_DIR%
rem	if not "%errorlevel%"=="0" goto :ERROR

%COPY_CMD% %EDITOR_EXE% %OUTPUT_DIR%
rem	if not "%errorlevel%"=="0" goto :ERROR

%COPY_CMD% %AUDIERE_DLL% %OUTPUT_DIR%
if not "%errorlevel%"=="0" goto :ERROR

echo # Copying ReadMe file
%COPY_CMD% %README_TXT% %OUTPUT_DIR%
if not "%errorlevel%"=="0" goto :ERROR

goto :END
:ERROR
echo # Error encountered, bailing...

:END
echo # Post-build script ended with errorlevel %errorlevel%

rem *** Restore original directory
popd
@echo on
