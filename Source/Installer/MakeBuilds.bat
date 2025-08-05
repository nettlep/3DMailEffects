@echo off
Echo Run both wise builds and place the output files on J:\ then press a key...
pause

echo Building shareware CAB file...
cd "..\Distribution Master - Shareware"
cabarc -r -p N j:\shareware.cab *.*

echo Building registered CAB file...
cd "..\Distribution Master - Registered"
cabarc -r -p N j:\registered.cab *.*

Echo Done.
Echo.
Echo DON'T FORGET -- zip up the exe files into zip files for old distributions...
pause
