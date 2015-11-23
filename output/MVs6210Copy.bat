set CURRENT=%~dp0
cd %CURRENT%
hexbin.exe MVs6210.hex MVs6210.MVC i
copy MVs6210.MVC I:\MVs6210.MVC 
pause