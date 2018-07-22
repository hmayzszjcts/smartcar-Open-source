::@echo off 

@for /d /r %%c in (Debug,settings,Release,FN15_Debug,FN15_Release,FX15_Debug,FX15_Release,DZ10_Debug,DZ10_Release) do @if exist %%c ( rd /s /q "%%c" & echo     删除目录%%c) 

@for /r  %%c in (*.tmp,*.bak ,*.dep,*.sfr,Backup*,*.tmp.c ) do del "%%c"
:: *.ewd 是下载配置文件
pause