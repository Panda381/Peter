# Microsoft Developer Studio Project File - Name="Setup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Setup - Win32 Debug Demo
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Setup.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Setup.mak" CFG="Setup - Win32 Debug Demo"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Setup - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Setup - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Setup - Win32 Debug Demo" (based on "Win32 (x86) Application")
!MESSAGE "Setup - Win32 Release MINI" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Setup - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G4 /Zp4 /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_OPTIM" /D "NDEMO" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "NDEBUG"
# ADD RSC /l 0x405 /d "NDEBUG" /d "NDEMO"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib ..\lib\tran.lib ole32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib

!ELSEIF  "$(CFG)" == "Setup - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G4 /Zp4 /W4 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NDEMO" /YX /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG"
# ADD RSC /l 0x405 /d "_DEBUG" /d "NDEMO"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib ..\lib\tran.lib ole32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /pdbtype:sept

!ELSEIF  "$(CFG)" == "Setup - Win32 Debug Demo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Setup___Win32_Debug_Demo"
# PROP BASE Intermediate_Dir "Setup___Win32_Debug_Demo"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDem"
# PROP Intermediate_Dir "DebugDem"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G4 /Zp4 /W4 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT BASE CPP /WX /Fr
# ADD CPP /nologo /G4 /Zp4 /ML /W4 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_DEMO" /YX /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG"
# ADD RSC /l 0x405 /d "_DEBUG" /d "_DEMO" /d "_DEMONLITE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib ..\lib\tran.lib ole32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib ..\lib\tran.lib ole32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /pdbtype:sept

!ELSEIF  "$(CFG)" == "Setup - Win32 Release MINI"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Setup___Win32_Release_MINI"
# PROP BASE Intermediate_Dir "Setup___Win32_Release_MINI"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_MINI"
# PROP Intermediate_Dir "Release_MINI"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G4 /Zp4 /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_OPTIM" /D "NDEMO" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G4 /Zp4 /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_OPTIM" /D "NDEMO" /D "MINI" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "NDEBUG" /d "NDEMO"
# ADD RSC /l 0x405 /d "NDEBUG" /d "NDEMO"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib ..\lib\tran.lib ole32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib ..\lib\tran.lib ole32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Setup - Win32 Release"
# Name "Setup - Win32 Debug"
# Name "Setup - Win32 Debug Demo"
# Name "Setup - Win32 Release MINI"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\BufPic.h
# End Source File
# Begin Source File

SOURCE=.\BufText.h
# End Source File
# Begin Source File

SOURCE=.\Compress.h
# End Source File
# Begin Source File

SOURCE=.\File.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\Memory.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\BufPic.cpp
# End Source File
# Begin Source File

SOURCE=.\BufText.cpp
# End Source File
# Begin Source File

SOURCE=.\Compress.cpp
# End Source File
# Begin Source File

SOURCE=.\File.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Memory.cpp
# End Source File
# Begin Source File

SOURCE=.\Stubs.c
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\Add0_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Add0_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Add1_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Add1_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Add2_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Add2_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Add3_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Add3_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Adr_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Adr_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\checkbox.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Demo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Digit.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\DInok.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Dinok_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Dinok_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\DInst.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Dinst_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Dinst_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\End0_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\End0_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\End1_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\End1_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Flag_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Flag_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemlogo1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemlogo2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemlogo3.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemlogo4.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemlogo5.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\idn_lite.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Insok.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Insok_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Insok_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Inst0_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Inst0_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Inst1_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Inst1_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Inst2_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Inst2_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Inst3_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Inst3_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Lic1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Lic1_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Lic1_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Lic2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Lic2_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Lic2_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Linka.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Lite.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Menu_cz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\peter.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Petr.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Phone_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Phone_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# Begin Source File

SOURCE=.\Res\Sel1_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Sel1_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Sel2_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Sel2_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Sel3_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Sel3_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Sel4_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Sel4_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Select.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Setup.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Shadow.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Shadowd.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\SwcOff.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\SwcOn.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Title_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Title_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\UIns0_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Uins0_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\UIns1_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Uins1_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\UIns2_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Uins2_en.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\UIns3_CZ.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Uins3_en.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Res\DelExe.exe
# End Source File
# End Target
# End Project
