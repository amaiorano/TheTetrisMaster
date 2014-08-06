@echo off

echo # Running post-build script...
echo # Current directory is %1
echo # Current configuration is %2

rem *** Set local variables
setlocal

set CFG=%2
if "%CFG%"=="" goto :ERROR

set COPY_CMD=xcopy.exe /S /R /K /Y /D

set SRC_DATA_DIR=.\Data
set SRC_EXE_DIR=.\_output\%CFG%
set OUTPUT_DIR=".\_game\The Tetris Master\"

set SRC_FILES=("%SRC_EXE_DIR%\Tetris\Tetris.exe" "%SRC_EXE_DIR%\ThemeLauncher\TTM Theme Launcher.exe" "%SRC_EXE_DIR%\ThemeEditor\TTM Theme Editor.exe" ".\GSGameLib\gsgamelib\external\audiere\audiere.dll" ".\Tetris\ReadMe.txt")

pushd %1

:COPY_DATA
%COPY_CMD% %SRC_DATA_DIR% %OUTPUT_DIR%
if not "%errorlevel%"=="0" goto :ERROR

echo # Copying executables and dlls
for %%i in %SRC_FILES% do %COPY_CMD% %%i %OUTPUT_DIR%

goto :END
:ERROR
echo # Error encountered, bailing...

:END
echo # Post-build script ended with errorlevel %errorlevel%

popd
@echo on
