@echo off
call utf82gbk %1
echo 将%1中的charset从UTF-8改为gb2312
fr %1 -f:charset=UTF-8 -t:charset=gb2312