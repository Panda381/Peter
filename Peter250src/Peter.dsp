# Microsoft Developer Studio Project File - Name="Peter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Peter - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Peter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Peter.mak" CFG="Peter - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Peter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Peter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Peter - Win32 Debug Optim" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Peter - Win32 Release"

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
# ADD RSC /l 0x405 /d "NDEBUG" /d "_LOADER" /d "NDEMO" /d "NDEMONLITE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib lib\tran.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Peter - Win32 Debug"

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
# ADD RSC /l 0x405 /d "_DEBUG" /d "_LOADER" /d "NDEMO" /d "NDEMONLITE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib lib\tran.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Peter - Win32 Debug Optim"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Peter___"
# PROP BASE Intermediate_Dir "Peter___"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugO"
# PROP Intermediate_Dir "DebugO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G4 /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /G4 /Zp4 /W4 /Gm /Zi /O2 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_OPTIM" /YX /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG"
# ADD RSC /l 0x405 /d "_DEBUG" /d "LOADERD" /d "NDEMO" /d "NDEMONLITE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib lib\tran.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib /pdbtype:sept /stub:"lib\stubcz.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib lib\tran.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Peter - Win32 Release"
# Name "Peter - Win32 Debug"
# Name "Peter - Win32 Debug Optim"
# Begin Group "Buffery"

# PROP Default_Filter "cpp"
# Begin Group "Buffery H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\BufIcon.h
# End Source File
# Begin Source File

SOURCE=.\BufInt.h
# End Source File
# Begin Source File

SOURCE=.\BufInx.h
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

SOURCE=.\BufProg.h
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

SOURCE=.\BufUndo.h
# End Source File
# Begin Source File

SOURCE=.\JPEG.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BufIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\BufInt.cpp
# End Source File
# Begin Source File

SOURCE=.\BufInx.cpp
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

SOURCE=.\BufProg.cpp
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

SOURCE=.\BufUndo.cpp
# End Source File
# Begin Source File

SOURCE=.\JPEG.cpp
# End Source File
# End Group
# Begin Group "Editory"

# PROP Default_Filter "cpp"
# Begin Group "Editory H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\EditIcon.h
# End Source File
# Begin Source File

SOURCE=.\EditLog.h
# End Source File
# Begin Source File

SOURCE=.\EditMap.h
# End Source File
# Begin Source File

SOURCE=.\EditMus.h
# End Source File
# Begin Source File

SOURCE=.\EditNum.h
# End Source File
# Begin Source File

SOURCE=.\EditSnd.h
# End Source File
# Begin Source File

SOURCE=.\EditSprt.h
# End Source File
# Begin Source File

SOURCE=.\EditText.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EditIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditMap.cpp
# End Source File
# Begin Source File

SOURCE=.\EditMus.cpp
# End Source File
# Begin Source File

SOURCE=.\EditNum.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSprt.cpp
# End Source File
# Begin Source File

SOURCE=.\EditText.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter "cpp h"
# Begin Group "Main H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Bitmap.h
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
# Begin Group "Select"

# PROP Default_Filter "cpp h"
# Begin Group "Select H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Select.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Select.cpp
# End Source File
# End Group
# Begin Group "Prog"

# PROP Default_Filter "cpp h"
# Begin Group "Prog H INC"

# PROP Default_Filter "h inc"
# Begin Source File

SOURCE=.\Prog.h
# End Source File
# Begin Source File

SOURCE=.\ProgClip.h
# End Source File
# Begin Source File

SOURCE=.\PROGCOL.H
# End Source File
# Begin Source File

SOURCE=.\ProgDrag.h
# End Source File
# Begin Source File

SOURCE=.\ProgExp.h
# End Source File
# Begin Source File

SOURCE=.\ProgFile.h
# End Source File
# Begin Source File

SOURCE=.\ProgHist.h
# End Source File
# Begin Source File

SOURCE=.\ProgInit.inc

!IF  "$(CFG)" == "Peter - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Peter - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Peter - Win32 Debug Optim"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProgLib.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Prog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgClip.cpp
# End Source File
# Begin Source File

SOURCE=.\PROGCOL.CPP
# End Source File
# Begin Source File

SOURCE=.\ProgDrag.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgExp.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgHist.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgLib.cpp
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Group "cur"

# PROP Default_Filter "cur"
# Begin Source File

SOURCE=.\Res\Cil.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Copy.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Delete.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Elip.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Fill.cur
# End Source File
# Begin Source File

SOURCE=.\Res\FillElip.cur
# End Source File
# Begin Source File

SOURCE=.\Res\FillRect.cur
# End Source File
# Begin Source File

SOURCE=.\Res\FillRoun.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Kapatko.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Koule.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Line.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Move.cur
# End Source File
# Begin Source File

SOURCE=.\Res\NoDrag.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Paint.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Pen.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Rect.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Round.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Ruka.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Select.cur
# End Source File
# Begin Source File

SOURCE=.\Res\SelMove.cur
# End Source File
# Begin Source File

SOURCE=.\Res\SplitH.cur
# End Source File
# Begin Source File

SOURCE=.\Res\SplitV.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Spray.cur
# End Source File
# End Group
# Begin Group "ico"

# PROP Default_Filter "ico"
# Begin Source File

SOURCE=".\Res\Lucka 2.ico"
# End Source File
# Begin Source File

SOURCE=".\Res\Lucka 3.ico"
# End Source File
# Begin Source File

SOURCE=.\Res\Lucka.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Peter.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Peter4.ico
# End Source File
# Begin Source File

SOURCE=".\Res\Petr 2.ico"
# End Source File
# Begin Source File

SOURCE=".\Res\Petr 3.ico"
# End Source File
# End Group
# Begin Group "bmp"

# PROP Default_Filter "bmp"
# Begin Source File

SOURCE=.\Res\Cz1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Cz2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Dotaz.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Eng1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Eng2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Fra1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Fra2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Ger1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Ger2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Icon.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ita1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ita2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\MainFram.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\MapSwc.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\MapSwcNm.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Modi.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\pol1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\pol2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\rus1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\rus2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Select.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Slo1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Slo2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Spa1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Spa2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Sprite.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\State.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\stdfonty.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ToolBar.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\wizard_256.bmp
# End Source File
# End Group
# Begin Group "dat"

# PROP Default_Filter "dat"
# Begin Source File

SOURCE=.\PalImp.dat
# End Source File
# Begin Source File

SOURCE=.\PalImpD.dat
# End Source File
# End Group
# Begin Group "exe"

# PROP Default_Filter "exe"
# Begin Source File

SOURCE=.\Loader0\Release\Peter.exe
# End Source File
# Begin Source File

SOURCE=.\Loader\Debug\Peter.exe
# End Source File
# Begin Source File

SOURCE=.\Loader\DebugO\Peter.exe
# End Source File
# Begin Source File

SOURCE=.\Loader\Lite\Peter.exe
# End Source File
# Begin Source File

SOURCE=.\Loader\LiteM\Peter.exe
# End Source File
# Begin Source File

SOURCE=.\Loader\Release\Peter.exe
# End Source File
# Begin Source File

SOURCE=.\Loader\ReleaseM\Peter.exe
# End Source File
# Begin Source File

SOURCE=.\Loader\Unicode\Peter.exe
# End Source File
# End Group
# Begin Group "txt"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\TextAlb.inc
# End Source File
# Begin Source File

SOURCE=.\TextAra.inc
# End Source File
# Begin Source File

SOURCE=.\TextBul.inc
# End Source File
# Begin Source File

SOURCE=.\TextCz.inc
# End Source File
# Begin Source File

SOURCE=.\TextDan.inc
# End Source File
# Begin Source File

SOURCE=.\TextEng.inc
# End Source File
# Begin Source File

SOURCE=.\TextFin.inc
# End Source File
# Begin Source File

SOURCE=.\TextFra.inc
# End Source File
# Begin Source File

SOURCE=.\TextGer.inc
# End Source File
# Begin Source File

SOURCE=.\TextHeb.inc
# End Source File
# Begin Source File

SOURCE=.\TextHol.inc
# End Source File
# Begin Source File

SOURCE=.\TextIsl.inc
# End Source File
# Begin Source File

SOURCE=.\TextIta.inc
# End Source File
# Begin Source File

SOURCE=.\TextMad.inc
# End Source File
# Begin Source File

SOURCE=.\TextNor.inc
# End Source File
# Begin Source File

SOURCE=.\TextPol.inc
# End Source File
# Begin Source File

SOURCE=.\TextPor.inc
# End Source File
# Begin Source File

SOURCE=.\TextRec.inc
# End Source File
# Begin Source File

SOURCE=.\TextRum.inc
# End Source File
# Begin Source File

SOURCE=.\TextRus.inc
# End Source File
# Begin Source File

SOURCE=.\TextSlo.inc
# End Source File
# Begin Source File

SOURCE=.\TextSpa.inc
# End Source File
# Begin Source File

SOURCE=.\TextSrb.inc
# End Source File
# Begin Source File

SOURCE=.\TextSwe.inc
# End Source File
# Begin Source File

SOURCE=.\TextTur.inc
# End Source File
# Begin Source File

SOURCE=.\TextVie.inc
# End Source File
# End Group
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# End Group
# End Target
# End Project
