@echo off
call utf82gbk %1
echo ��%1�е�charset��UTF-8��Ϊgb2312
fr %1 -f:charset=UTF-8 -t:charset=gb2312