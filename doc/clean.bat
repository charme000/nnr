@echo off
echo 清空以前输出
if exist html\html del /f /q html\html\*.*
if exist html\latex del /f /q html\latex\*.*
if exist html\rtf del /f /q html\rtf\*.*
if exist html del /f /q html\*.*