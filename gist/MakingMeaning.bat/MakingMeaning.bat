@Echo off
setlocal
:Start
::初始设置=============================
Setlocal EnableDelayedExpansion
Title 制作内涵图
::准备工作=============================
Cls
Set /p "RAR=请把压缩文件（RAR）拖到窗口:"
Set /p "PIC=请把图片文件（JPG）拖到窗口:"
If /I not defined PIC Goto Start
If /I not defined RAR Goto Start
For %%i in (RAR PIC) do (
For %%j in (!%%i!) do (
Set "%%i=%%~j"
If /I "%%i"=="RAR" Set "Name=%%~dpnj"
Call :Check "%%~j" "%%i"
))
::开始合成=============================
Cls
Echo !RAR!+!PIC!...
Copy /b "!PIC!"+"!RAR!" "!Name!-New.jpg">nul 2>nul
If "%errorlevel%"=="0" (Echo 貌似成功了=.=...) else (echo 失败了同志！)
endlocal
Pause
Goto Start
::完成=============================


::"Function"===========================
:Check
If "%~2"=="RAR" Set "Exp=.RAR"
If "%~2"=="PIC" Set "Exp=.JPG"
Set "Check=%~1"
Set "Check=!Check:~-4!"
If /I "!Check!"=="!Exp!" Goto :eof
Echo 请重新拖入%~2文件。
Pause
endlocal
Goto Start

Goto :eof