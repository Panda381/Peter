@echo off
if "%1" == "" goto Konec
if not exist %1\*.* goto Konec

attrib -H %1\*.*
if exist %1\*.gid del %1\*.gid
if exist %1\*.exe del %1\*.exe
if exist %1\*.obj del %1\*.obj
if exist %1\*.res del %1\*.res
if exist %1\*.pch del %1\*.pch
if exist %1\*.ilk del %1\*.ilk
if exist %1\*.pdb del %1\*.pdb
if exist %1\*.idb del %1\*.idb
if exist %1\*.cnt del %1\*.cnt
if exist %1\*.hlp del %1\*.hlp
if exist %1\*.tlb del %1\*.tlb
if exist %1\*.sbr del %1\*.sbr
if exist %1\*.bsc del %1\*.bsc
if exist %1\resource.aps del %1\resource.aps

:Konec
rd %1
