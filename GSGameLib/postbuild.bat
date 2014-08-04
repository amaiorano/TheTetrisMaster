@echo off

echo Running post-build script...

rem *** INPUT PARAM IS CURRENT DIRECTORY
echo Current directory is %1

pushd %1

setlocal

set GS_3RD_PARTY=.\gsgamelib\3rdparty
set GS_BIN=.\gsgamelib\bin

if not exist %GS_3RD_PARTY% goto :ERROR

md %GS_BIN%

echo Copying AUDIERE files...
@echo on
copy /Y %GS_3RD_PARTY%\audiere\audiere.dll %GS_BIN%
@echo off
goto :END



:ERROR
echo Error encountered while running script
pause


:END
echo Post-build script ended with errorlevel %errorlevel%
popd