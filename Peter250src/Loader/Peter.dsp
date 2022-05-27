# Microsoft Developer Studio Project File - Name="Loader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Loader - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Peter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Peter.mak" CFG="Loader - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Loader - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Loader - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Loader - Win32 Debug Optim" (based on "Win32 (x86) Application")
!MESSAGE "Loader - Win32 Release Mini" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Loader - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G4 /Zp4 /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_OPTIM" /YX /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x405 /d "NDEBUG"
# ADD RSC /l 0x405 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib msacm32.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib ole32.lib uuid.lib ..\lib\tran.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Loader - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G4 /Zp4 /W4 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG"
# ADD RSC /l 0x405 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib msacm32.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib ole32.lib uuid.lib ..\lib\tran.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Loader - Win32 Debug Optim"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Loader__"
# PROP BASE Intermediate_Dir "Loader__"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugO"
# PROP Intermediate_Dir "DebugO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LOADER" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G4 /Zp4 /W4 /Gm /Zi /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_OPTIM" /YX /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG"
# ADD RSC /l 0x405 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib ..\lib\tran.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib /pdbtype:sept /stub:"../lib/stubcz.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib msacm32.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib ole32.lib uuid.lib ..\lib\tran.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Loader - Win32 Release Mini"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Loader___Win32_Release_Mini"
# PROP BASE Intermediate_Dir "Loader___Win32_Release_Mini"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseM"
# PROP Intermediate_Dir "ReleaseM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G4 /Zp4 /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_OPTIM" /YX /FD /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /G4 /Zp4 /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_OPTIM" /D "_MINI" /YX /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x405 /d "NDEBUG"
# ADD RSC /l 0x405 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib ole32.lib uuid.lib ..\lib\tran.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /stub:"../lib/stubcz.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib msacm32.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib ole32.lib uuid.lib ..\lib\tran.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Loader - Win32 Release"
# Name "Loader - Win32 Debug"
# Name "Loader - Win32 Debug Optim"
# Name "Loader - Win32 Release Mini"
# Begin Group "Buffery"

# PROP Default_Filter "cpp"
# Begin Group "Buffery H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\BufD3D.h
# End Source File
# Begin Source File

SOURCE=.\BufIcon.h
# End Source File
# Begin Source File

SOURCE=.\BufInt.h
# End Source File
# Begin Source File

SOURCE=.\BufList.h
# End Source File
# Begin Source File

SOURCE=.\BufMap.h
# End Source File
# Begin Source File

SOURCE=.\BufMus.h
# End Source File
# Begin Source File

SOURCE=.\BufPic.h
# End Source File
# Begin Source File

SOURCE=.\BufReal.h
# End Source File
# Begin Source File

SOURCE=.\BufSnd.h
# End Source File
# Begin Source File

SOURCE=.\BufSprt.h
# End Source File
# Begin Source File

SOURCE=.\BufText.h
# End Source File
# Begin Source File

SOURCE=.\BUFWIN.H
# End Source File
# Begin Source File

SOURCE=.\BufXFile.h
# End Source File
# Begin Source File

SOURCE=.\JPEG.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BufD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\BufIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\BufInt.cpp
# End Source File
# Begin Source File

SOURCE=.\BufList.cpp
# End Source File
# Begin Source File

SOURCE=.\BufMap.cpp
# End Source File
# Begin Source File

SOURCE=.\BufMus.cpp
# End Source File
# Begin Source File

SOURCE=.\BufPic.cpp
# End Source File
# Begin Source File

SOURCE=.\BufReal.cpp
# End Source File
# Begin Source File

SOURCE=.\BufSnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BufSprt.cpp
# End Source File
# Begin Source File

SOURCE=.\BufText.cpp
# End Source File
# Begin Source File

SOURCE=.\BUFWIN.CPP
# End Source File
# Begin Source File

SOURCE=.\BufXFile.cpp
# End Source File
# Begin Source File

SOURCE=.\JPEG.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter "cpp"
# Begin Group "Main H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\Compress.h
# End Source File
# Begin Source File

SOURCE=.\D3DX4.h
# End Source File
# Begin Source File

SOURCE=.\D3DX5.h
# End Source File
# Begin Source File

SOURCE=.\D3DX6.h
# End Source File
# Begin Source File

SOURCE=.\D3DX7.h
# End Source File
# Begin Source File

SOURCE=.\D3DX8.h
# End Source File
# Begin Source File

SOURCE=.\D3GL0.h
# End Source File
# Begin Source File

SOURCE=.\D3GL1.h
# End Source File
# Begin Source File

SOURCE=.\D3GL2.h
# End Source File
# Begin Source File

SOURCE=.\D3NO.h
# End Source File
# Begin Source File

SOURCE=.\File.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Memory.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Compress.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DX4.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DX5.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DX6.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DX7.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DX8.cpp
# End Source File
# Begin Source File

SOURCE=.\D3GL0.cpp
# End Source File
# Begin Source File

SOURCE=.\D3GL1.cpp
# End Source File
# Begin Source File

SOURCE=.\D3GL2.cpp
# End Source File
# Begin Source File

SOURCE=.\D3NO.cpp
# End Source File
# Begin Source File

SOURCE=.\File.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Memory.cpp
# End Source File
# Begin Source File

SOURCE=.\Stubs.c
# End Source File
# End Group
# Begin Group "Comp"

# PROP Default_Filter "cpp"
# Begin Group "Comp H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Comp.h
# End Source File
# Begin Source File

SOURCE=.\CompCom.h
# End Source File
# Begin Source File

SOURCE=.\CompIco.h
# End Source File
# Begin Source File

SOURCE=.\CompLog.h
# End Source File
# Begin Source File

SOURCE=.\CompMap.h
# End Source File
# Begin Source File

SOURCE=.\CompMus.h
# End Source File
# Begin Source File

SOURCE=.\CompNum.h
# End Source File
# Begin Source File

SOURCE=.\CompPic.h
# End Source File
# Begin Source File

SOURCE=.\CompSnd.h
# End Source File
# Begin Source File

SOURCE=.\CompSpr.h
# End Source File
# Begin Source File

SOURCE=.\CompTxt.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Comp.cpp
# End Source File
# Begin Source File

SOURCE=.\CompCom.cpp
# End Source File
# Begin Source File

SOURCE=.\CompIco.cpp
# End Source File
# Begin Source File

SOURCE=.\CompLog.cpp
# End Source File
# Begin Source File

SOURCE=.\CompMap.cpp
# End Source File
# Begin Source File

SOURCE=.\CompMus.cpp
# End Source File
# Begin Source File

SOURCE=.\CompNum.cpp
# End Source File
# Begin Source File

SOURCE=.\CompPic.cpp
# End Source File
# Begin Source File

SOURCE=.\CompSnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CompSpr.cpp
# End Source File
# Begin Source File

SOURCE=.\CompTxt.cpp
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=..\PalImp.dat
# End Source File
# Begin Source File

SOURCE=..\Res\Peter.ico
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# End Group
# Begin Group "Load"

# PROP Default_Filter "cpp"
# Begin Group "Load H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Load.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Load.cpp
# End Source File
# End Group
# Begin Group "Exec"

# PROP Default_Filter "cpp"
# Begin Group "Exec H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Exec.h
# End Source File
# Begin Source File

SOURCE=.\ExecCom.h
# End Source File
# Begin Source File

SOURCE=.\ExecIco.h
# End Source File
# Begin Source File

SOURCE=.\ExecLog.h
# End Source File
# Begin Source File

SOURCE=.\ExecMap.h
# End Source File
# Begin Source File

SOURCE=.\ExecMus.h
# End Source File
# Begin Source File

SOURCE=.\ExecNum.h
# End Source File
# Begin Source File

SOURCE=.\ExecPic.h
# End Source File
# Begin Source File

SOURCE=.\ExecSnd.h
# End Source File
# Begin Source File

SOURCE=.\ExecSpr.h
# End Source File
# Begin Source File

SOURCE=.\ExecTxt.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Exec.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecCom.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecIco.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecLog.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecMap.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecMus.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecNum.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecPic.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecSnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecSpr.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecTxt.cpp
# End Source File
# End Group
# End Target
# End Project
