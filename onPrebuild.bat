@echo off

SET source="%~dp0copy_to_build"
ECHO Copying from %source%

SET dest="%1"
ECHO Copying to %dest%


XCOPY %source% %dest% /s /e /y