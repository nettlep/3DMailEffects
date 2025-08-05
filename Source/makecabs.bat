@echo off

: Test to see if the files all exist

if not exist r_reg\3dme.exe goto not_built
if not exist r_share\3dme.exe goto not_built
if not exist r_sharebeta\3dme.exe goto not_built
if not exist r_regbeta\3dme.exe goto not_built

: Update the upd.exe file

echo Copying latest upd.exe file...
cd "Distribution Master"
copy ..\upd\release\upd.exe
cd ..

echo If OKAY,
pause

: Build them

echo Building registered.cab...
cd "Distribution Master"
copy ..\r_reg\3dme.exe
cabarc -r -p n j:\registered.cab *.*
if not exist j:\registered.cab goto no_reg
del 3dme.exe
cd ..

echo If OKAY,
pause

echo Building shareware.cab...
cd "Distribution Master"
copy ..\r_share\3dme.exe
cabarc -r -p n j:\shareware.cab *.*
if not exist j:\shareware.cab goto no_share
del 3dme.exe
cd ..

echo If OKAY,
pause

echo Building betaregistered.cab...
cd "Distribution Master"
copy ..\r_regbeta\3dme.exe
cabarc -r -p n j:\betaregistered.cab *.*
if not exist j:\betaregistered.cab goto no_breg
del 3dme.exe
cd ..

echo If OKAY,
pause

echo Building betashareware.cab...
cd "Distribution Master"
copy ..\r_sharebeta\3dme.exe
cabarc -r -p n j:\betashareware.cab *.*
if not exist j:\betashareware.cab goto no_bshare
del 3dme.exe
cd ..

: Okay...

echo Done.
goto end

:not_built
echo Some of the EXEs are missing!
goto end

:no_reg
echo Unable to build registered.cab
goto end

:no_breg
echo Unable to build betaregistered.cab
goto end

:no_share
echo Unable to build shareware.cab
goto end

:no_bshare
echo Unable to build betashareware.cab
goto end

:end
