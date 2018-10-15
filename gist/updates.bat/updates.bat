@echo off
title 自动更新
mode con cols=31 lines=14
rem 需要变更的值
set ym=请填写你的FTP域名
set zh=请填写你的FTP账号
set mm=请填写你的FTP密码
set ftpgh=请填写ftp上的版本号文件名
set plagh=请填写本地的版本号文件名
set plabd=请填写被更新文件名
set open=更新之后打开的文件名
rem 需要变更的值完毕

echo 正在检测是否需要自动更新...
echo o %ym% >ftp.txt
echo %zh% >>ftp.txt
echo %mm% >>ftp.txt

rem 可能你将下载的更新文件存储在了文件夹中，进入文件夹的代码是echo cd 【文件夹名】 >>ftp.txt
echo get %ftpgh% >>ftp.txt
echo bye >>ftp.txt
ftp -s:ftp.txt >nul
del ftp.txt

set ylbb=0
set xzbb=0
set /p ylbb=<%ftpgh%
set /p xzbb=<%plagh%
if not %ylbb%==%xzbb% goto gengxin
if %ylbb%==%xzbb% goto bugengxin

:gengxin
cls
echo 您的版本需要更新，正在更新中
echo o %ym% >ftp.txt
echo %zh% >>ftp.txt
echo %mm% >>ftp.txt

rem 可能你将下载的更新文件存储在了文件夹中，进入文件夹的代码是echo cd 【文件夹名】 >>ftp.txt
echo get %plabd% >>ftp.txt
echo bye >>ftp.txt
ftp -s:ftp.txt >nul
del ftp.txt
del %plagh%
copy %ftpgh% %plagh%
del %ftpgh%
cls
echo 更新完成！
pause>nul
start %open%
exit


:bugengxin
del %ftpgh%
cls
echo 您的版本不需要更新...
pause>nul
start %open%
exit