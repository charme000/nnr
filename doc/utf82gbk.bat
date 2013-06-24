@echo off
echo 将%1从utf-8编码转换到gbk编码
for %%f in (%1) do copy %%f %%f.utf8
for %%f in (%1) do iconv -c -f utf-8 -t gbk %%f.utf8 > %%f