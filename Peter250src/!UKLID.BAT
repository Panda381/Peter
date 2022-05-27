@echo off
if not exist HLP\!uklid.bat goto NHlp
cd HLP
call !uklid.bat
cd..
:NHlp

if not exist Setup\Res\!uklid.bat goto NSetup
cd Setup\Res
call !uklid.bat
cd..\..
:NSetup

if not exist DataInst\Res\!uklid.bat goto NDataInst
cd DataInst\Res
call !uklid.bat
cd..\..
:NDataInst

call _uklid DataInst\Debug
call _uklid DataInst\Release
call _uklid Debug
call _uklid DebugDem
call _uklid DebugO
call _uklid DebugU
call _uklid DelExe\Debug
call _uklid DelExe\Release
call _uklid Demo
call _uklid Lite
call _uklid Peter___Win32_Lite_Islandsoft
call _uklid LiteDeb
call _uklid LiteM
call _uklid Install
call _uklid Gener\Debug
call _uklid Gener\Demo
call _uklid Gener\Install
call _uklid Gener\Release
call _uklid Gener\Lite
call _uklid Loader\Debug
call _uklid Loader\DebugO
call _uklid Loader\Release
call _uklid Loader\ReleaseM
call _uklid Loader\Unicode
call _uklid Loader\Lite
call _uklid Loader\LiteM
call _uklid Loader0\Debug
call _uklid Loader0\Release
call _uklid Pov2Spr\Debug
call _uklid Pov2Spr\Release
call _uklid Release
call _uklid ReleaseM
call _uklid Setup\Debug
call _uklid Setup\DebugDem
call _uklid Setup\Demo
call _uklid Setup\Release
call _uklid Setup\Lite
call _uklid Unicode

if exist BACKUP\*.bak del BACKUP\*.bak
if exist BACKUP\*.exe del BACKUP\*.exe
if exist BACKUP\nul rd BACKUP
if exist BOOL\nul rd BOOL
if exist FUNCTION\nul rd FUNCTION
if exist ICON\nul rd ICON
if exist MAP\nul rd MAP
if exist MUSIC\nul rd MUSIC
if exist NUMBER\nul rd NUMBER
if exist PICTURE\nul rd PICTURE
if exist PROGRAM\*.exe del PROGRAM\*.exe
if exist PROGRAM\nul rd PROGRAM
if exist SOUND\nul rd SOUND
if exist SPRITE\nul rd SPRITE
if exist TEXT\nul rd TEXT

if exist *.opt del *.opt
if exist *.ncb del *.ncb
if exist *.plg del *.plg
if exist *.aps del *.aps
if exist Pet*.ini del Pet*.ini
if exist DataInst\*.aps del DataInst\*.aps
if exist DataInst\*.plg del DataInst\*.plg
if exist DelExe\*.plg del DelExe\*.plg
if exist Gener\*.plg del Gener\*.plg
if exist Loader\*.aps del Loader\*.aps
if exist Loader\*.plg del Loader\*.plg
if exist Loader0\*.aps del Loader0\*.aps
if exist Loader0\*.plg del Loader0\*.plg
if exist Pov2Spr\*.aps del Pov2Spr\*.aps
if exist Pov2Spr\*.plg del Pov2Spr\*.plg
if exist Setup\*.aps del Setup\*.aps
if exist Setup\*.plg del Setup\*.plg
