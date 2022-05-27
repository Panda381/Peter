@echo off
cls
echo Break with Ctrl+Break...
echo.
if exist Continue.bat call Continue.bat
if exist Continue.bat del Continue.bat
call ..\Render Gemtree  0
