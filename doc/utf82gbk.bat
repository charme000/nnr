@echo off
echo ��%1��utf-8����ת����gbk����
for %%f in (%1) do copy %%f %%f.utf8
for %%f in (%1) do iconv -c -f utf-8 -t gbk %%f.utf8 > %%f