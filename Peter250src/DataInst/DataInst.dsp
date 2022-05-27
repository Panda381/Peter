# Microsoft Developer Studio Project File - Name="DataInst" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DataInst - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DataInst.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DataInst.mak" CFG="DataInst - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DataInst - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DataInst - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DataInst - Win32 Release"

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
# ADD BSC32 /nologo /o"Release/Setup.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib ..\lib\tran.lib ole32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:"Release/Setup.exe"

!ELSEIF  "$(CFG)" == "DataInst - Win32 Debug"

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
# ADD BSC32 /nologo /o"Debug/Setup.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib winmm.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib uuid.lib ..\lib\tran.lib ole32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib /out:"Debug/Setup.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DataInst - Win32 Release"
# Name "DataInst - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=.\Res\checkbox.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\DataInst.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Demo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Dinst.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\DInstok.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre01.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre02.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre03.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre04.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre05.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre06.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre07.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre08.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre09.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre10.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre11.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre12.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre13.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre14.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre15.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre16.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre17.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre18.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre19.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre20.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre21.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre22.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre23.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre24.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre25.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre26.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre27.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre28.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre29.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre30.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre31.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre32.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre33.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre34.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre35.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre36.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre37.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre38.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre39.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre40.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre41.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre42.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre43.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre44.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre45.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre46.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre47.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre48.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre49.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre50.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre51.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre52.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre53.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre54.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre55.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre56.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre57.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre58.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre59.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre60.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre61.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre62.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre63.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre64.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre65.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre66.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre67.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre68.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre69.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre70.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre71.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre72.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre73.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre74.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre75.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre76.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre77.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre78.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre79.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre80.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre81.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre82.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre83.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre84.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre85.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre86.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre87.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre88.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre89.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Gemtre90.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Instok.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Licence.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Licence2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\peter.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Petr.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\PictuD0A.BMP
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur0A.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur0B.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur1A.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur1B.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur2A.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur2B.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur3A.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur3B.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur4A.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Pictur4B.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# Begin Source File

SOURCE=.\Res\Shadow.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Shadowd.bmp
# End Source File
# End Group
# End Target
# End Project
