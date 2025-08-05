# Microsoft Developer Studio Project File - Name="3Dme" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=3Dme - Win32 d_reg
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "3Dme.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "3Dme.mak" CFG="3Dme - Win32 d_reg"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "3Dme - Win32 d_share" (based on "Win32 (x86) Application")
!MESSAGE "3Dme - Win32 d_reg" (based on "Win32 (x86) Application")
!MESSAGE "3Dme - Win32 r_share" (based on "Win32 (x86) Application")
!MESSAGE "3Dme - Win32 r_reg" (based on "Win32 (x86) Application")
!MESSAGE "3Dme - Win32 r_sharebeta" (based on "Win32 (x86) Application")
!MESSAGE "3Dme - Win32 r_regbeta" (based on "Win32 (x86) Application")
!MESSAGE "3Dme - Win32 r_lite" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "3Dme - Win32 d_share"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "d_share"
# PROP BASE Intermediate_Dir "d_share"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "d_share"
# PROP Intermediate_Dir "d_share"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "3Dme - Win32 d_reg"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "d_reg"
# PROP BASE Intermediate_Dir "d_reg"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "d_reg"
# PROP Intermediate_Dir "d_reg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_REGISTERED_" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_REGISTERED_" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_share"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "r_share"
# PROP BASE Intermediate_Dir "r_share"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "r_share"
# PROP Intermediate_Dir "r_share"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Compressing...
PostBuild_Cmds=upx -q --best r_share\3dme.exe	CSum\Release\CSum r_share\3dme.exe	copy r_share\3dme.exe "Distribution Master - Shareware"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_reg"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "r_reg"
# PROP BASE Intermediate_Dir "r_reg"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "r_reg"
# PROP Intermediate_Dir "r_reg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_REGISTERED_" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_REGISTERED_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Compressing...
PostBuild_Cmds=upx -q --best r_reg\3dme.exe	CSum\Release\CSum r_reg\3dme.exe	copy r_reg\3dme.exe "Distribution Master - Registered"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_sharebeta"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "r_sharebeta"
# PROP BASE Intermediate_Dir "r_sharebeta"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "r_sharebeta"
# PROP Intermediate_Dir "r_sharebeta"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_BETA_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Compressing...
PostBuild_Cmds=upx -q --best r_sharebeta\3dme.exe	CSum\Release\CSum r_sharebeta\3dme.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_regbeta"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "r_regbeta"
# PROP BASE Intermediate_Dir "r_regbeta"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "r_regbeta"
# PROP Intermediate_Dir "r_regbeta"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_REGISTERED_" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_REGISTERED_" /D "_BETA_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Compressing...
PostBuild_Cmds=upx -q --best r_regbeta\3dme.exe	CSum\Release\CSum r_regbeta\3dme.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_lite"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "r_lite"
# PROP BASE Intermediate_Dir "r_lite"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "r_lite"
# PROP Intermediate_Dir "r_lite"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_REGISTERED_" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_LITE_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 winmm.lib cabsdk\lib\fdi.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Compressing...
PostBuild_Cmds=upx -q --best r_lite\3dme.exe	CSum\Release\CSum r_lite\3dme.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "3Dme - Win32 d_share"
# Name "3Dme - Win32 d_reg"
# Name "3Dme - Win32 r_share"
# Name "3Dme - Win32 r_reg"
# Name "3Dme - Win32 r_sharebeta"
# Name "3Dme - Win32 r_regbeta"
# Name "3Dme - Win32 r_lite"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3Dme.cpp

!IF  "$(CFG)" == "3Dme - Win32 d_share"

!ELSEIF  "$(CFG)" == "3Dme - Win32 d_reg"

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_share"

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_reg"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_sharebeta"

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_regbeta"

# ADD BASE CPP /O2
# ADD CPP /O2

!ELSEIF  "$(CFG)" == "3Dme - Win32 r_lite"

# ADD BASE CPP /O2
# ADD CPP /O2

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\3Dme.rc
# End Source File
# Begin Source File

SOURCE=.\3DmeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\3ds.cpp
# End Source File
# Begin Source File

SOURCE=.\3DUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountList.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountPreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AlarmPreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppPrefs.cpp
# End Source File
# Begin Source File

SOURCE=.\AppPrefsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BitBut.cpp
# End Source File
# Begin Source File

SOURCE=.\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Cab.cpp
# End Source File
# Begin Source File

SOURCE=.\Clip.cpp
# End Source File
# Begin Source File

SOURCE=.\Conf.cpp
# End Source File
# Begin Source File

SOURCE=.\Download.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadThread.cpp
# End Source File
# Begin Source File

SOURCE=.\EggDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EmailList.cpp
# End Source File
# Begin Source File

SOURCE=.\EmailListbox.cpp
# End Source File
# Begin Source File

SOURCE=.\EmailPreviewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\Http.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\logger.cpp
# End Source File
# Begin Source File

SOURCE=.\MailThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Mapper.cpp
# End Source File
# Begin Source File

SOURCE=.\Mime.cpp
# End Source File
# Begin Source File

SOURCE=.\pop.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegDownloadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReminderPreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\RunDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ThreeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreeThread.cpp
# End Source File
# Begin Source File

SOURCE=.\TipOfTheDayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TMap.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdatesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\WebMail.cpp
# End Source File
# Begin Source File

SOURCE=.\winDIB.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3Dme.h
# End Source File
# Begin Source File

SOURCE=.\3DmeDlg.h
# End Source File
# Begin Source File

SOURCE=.\3ds.h
# End Source File
# Begin Source File

SOURCE=.\3DUtils.h
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\AccountList.h
# End Source File
# Begin Source File

SOURCE=.\AccountPreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=.\AlarmPreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=.\AppPrefs.h
# End Source File
# Begin Source File

SOURCE=.\AppPrefsDlg.h
# End Source File
# Begin Source File

SOURCE=.\BitBut.h
# End Source File
# Begin Source File

SOURCE=.\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\Cab.h
# End Source File
# Begin Source File

SOURCE=.\Clip.h
# End Source File
# Begin Source File

SOURCE=.\Conf.h
# End Source File
# Begin Source File

SOURCE=.\Download.h
# End Source File
# Begin Source File

SOURCE=.\DownloadThread.h
# End Source File
# Begin Source File

SOURCE=.\EggDlg.h
# End Source File
# Begin Source File

SOURCE=.\EmailList.h
# End Source File
# Begin Source File

SOURCE=.\EmailListbox.h
# End Source File
# Begin Source File

SOURCE=.\EmailPreviewDlg.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\Fubar.h
# End Source File
# Begin Source File

SOURCE=.\Fubar_off.h
# End Source File
# Begin Source File

SOURCE=.\Fubar_on.h
# End Source File
# Begin Source File

SOURCE=.\Http.h
# End Source File
# Begin Source File

SOURCE=.\hyperlink.h
# End Source File
# Begin Source File

SOURCE=.\logger.h
# End Source File
# Begin Source File

SOURCE=.\MailThread.h
# End Source File
# Begin Source File

SOURCE=.\Mapper.h
# End Source File
# Begin Source File

SOURCE=.\Mime.h
# End Source File
# Begin Source File

SOURCE=.\pop.h
# End Source File
# Begin Source File

SOURCE=.\Primitive.h
# End Source File
# Begin Source File

SOURCE=.\ProxyDialog.h
# End Source File
# Begin Source File

SOURCE=.\RayPlaneLine.h
# End Source File
# Begin Source File

SOURCE=.\RegDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegDownloadDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReminderPreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=.\Render.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RunDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScriptNotice.h
# End Source File
# Begin Source File

SOURCE=.\Socket.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ThreeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ThreeThread.h
# End Source File
# Begin Source File

SOURCE=.\timer
# End Source File
# Begin Source File

SOURCE=.\TipOfTheDayDlg.h
# End Source File
# Begin Source File

SOURCE=.\TMap.h
# End Source File
# Begin Source File

SOURCE=.\UpdatesDlg.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h
# End Source File
# Begin Source File

SOURCE=.\Vertex.h
# End Source File
# Begin Source File

SOURCE=.\VMath.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser.h
# End Source File
# Begin Source File

SOURCE=.\WebMail.h
# End Source File
# Begin Source File

SOURCE=.\winDIB.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\3Dme-small.ico"
# End Source File
# Begin Source File

SOURCE=.\res\3Dme.ico
# End Source File
# Begin Source File

SOURCE=.\res\3Dme.rc2
# End Source File
# Begin Source File

SOURCE=.\res\about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\accountinfo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bigcheck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bigx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\blank.ico
# End Source File
# Begin Source File

SOURCE=.\res\check.ico
# End Source File
# Begin Source File

SOURCE=.\res\checknow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\currentemails.bmp
# End Source File
# Begin Source File

SOURCE=.\res\did_you_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\egg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enablealarms.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enablechecking.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enableflashing.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enablepopups.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enablereminders.bmp
# End Source File
# Begin Source File

SOURCE=.\res\err.bmp
# End Source File
# Begin Source File

SOURCE=.\res\forward.bmp
# End Source File
# Begin Source File

SOURCE=.\res\globaloptions.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hand_cur.cur
# End Source File
# Begin Source File

SOURCE=.\res\hide.bmp
# End Source File
# Begin Source File

SOURCE=.\res\home.bmp
# End Source File
# Begin Source File

SOURCE=.\res\install.ico
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\markread.bmp
# End Source File
# Begin Source File

SOURCE=.\res\optionsandsettings.bmp
# End Source File
# Begin Source File

SOURCE=.\res\playsound.ico
# End Source File
# Begin Source File

SOURCE=.\res\preferences.bmp
# End Source File
# Begin Source File

SOURCE=.\res\refresh.bmp
# End Source File
# Begin Source File

SOURCE=.\res\scrolllock.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tip_left.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tip_of_the_day.bmp
# End Source File
# Begin Source File

SOURCE=.\res\x.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\Protection.txt
# End Source File
# Begin Source File

SOURCE=.\todo.txt
# End Source File
# End Target
# End Project
# Section 3Dme : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:WebBrowserControl
# 	2:10:HeaderFile:webbrowser.h
# 	2:8:ImplFile:webbrowser.cpp
# End Section
# Section 3Dme : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser.h
# 	2:16:DefaultSinkClass:WebBrowserControl
# End Section
