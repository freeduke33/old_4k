@echo off
if exist *.bak del *.bak
pkzip -u ada_4sys *.asm *.inc *.mac *.c *.h *.ww arc.bat *.lib dirinfo
