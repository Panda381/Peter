Gemtree Peter 2.50 (for Windows 95 and later)
FREEWARE / PUBLIC DOMAIN

Author: (c) 1997-2022 Miroslav Nemecek
author's homepage: http://panda38.sweb.cz
author's email: Panda38@seznam.cz
alternative email: hardyplotter2@gmail.com


Author's homepage of the program Peter: https://www.breatharian.eu/Petr/en
Peter's official website (no longer maintained): http://www.gemtree.com and http://www.gemtree.cz
Programs and games created in Peter: https://www.breatharian.eu/Petr/en/program
community server: https://www.itnetwork.cz/pro-deti/petr
GitHub: https://github.com/Panda381/Peter


LICENSE
-------
The Peter program can be used freely for non-commercial and commercial
purposes. Similarly, you can freely use and modify the Peter source code.
However, I don't recommend using the source code, because I've been
working on Peter I was just learning to program in C++ ;-) and there
are many things I would be doing today differently.

Peter is offered for use as is, without any no further support and no
further development or fixes.

Recommendations
---------------
Start the installation by running Peter250.exe (preferably as
Administrator) or by unpacking the ZIP without the installer.
Under Windows 10, add the following from the Help folder:
Peter.exe, PETER_CZ.CHM a PETER_EN.CHM.


Notes
-----
- Under some systems, installation and uninstallation may not work, in
  which case it may be necessary to use a version without installation.
  Alternatively, uninstall by re-running the installer. The installer
  usually works by running it under Administrator.

- Some antivirus programs may mislabel programs created by in Peter as
  containing a virus (mallware). This is because Peter saves the edited
  program directly to an EXE file. The EXE file contains interpreter and
  the attached program in binary form. Some antiviruses consider the
  interpreter's activity to be the manifestation of a virus. Typically,
  these are programs stored in a non-executable form (without an interpreter).

- Help cannot be displayed under higher systems. It is necessary to either
  install WinHlp32.exe from Microsoft website or copy the files Peter.exe,
  PETER_CZ.CHM and PETER_EN.CHM (this is a modification of Peter by user
  Jiri Peroutka) into the Peter folder from the Help directory. You can also
  view the Help in HTML format using Help\HELP_EN\index.html.

- Some programs use MP3 sounds (e.g. ghost voices in the game "Thirteen
  Ghosts"). If the sounds do not work, you may need to install an MP3 codec
  (Misc\L3CODECX.EXE program).


Version with installer
----------------------
Installation problems: when you try to uninstall it the standard way,
Windows XP (or higher) may report an invalid application. In this case,
use the Peter installer again to uninstall (the mini version is sufficient,
and delete the installation folder manually) or just delete the Peter
installation folder and the corresponding shortcuts manually. To install
on Vista and above, you may need to use the version without the installer
(just unzip it) or an alternative installer from the community server
https://www.itnetwork.cz/pro-deti/petr . Or usually just run the installer
as Administrator.

Peter250.exe - Peter 2.50 full installation (400 MB)
https://www.breatharian.eu/Petr/download/Peter250.exe
Contains: application Petr, helps (Czech and English), about 160 sample
2400 pictures, 1000 icons (objects), 1500 sounds, 560 music,
400 sprites, resources for generating sprites.

Peter250mini.exe - Peter 2.50 mini installation (5 MB)
https://www.breatharian.eu/Petr/download/Peter250mini.exe
Contains: application Petr, helpy (Czech and English), sample programs
Maze and Tutorial, sample library files.

Peter241Lite.exe - Petr 2.41 Lite installation (5 MB)
https://www.breatharian.eu/Petr/download/Peter241Lite.exe
Older version with limited features. May be useful for younger or beginner
users because it does not contain any dangerous features (such as file
and memory operations). Can be installed and used together with version 2.50
because it installs in a different folder.

Peter223LiteCZ.exe - Petr 2.23 Lite installation (2,5 MB)
https://www.breatharian.eu/Petr/download/Peter223LiteCZ.exe
An even older version with even more limited features. Help is only in Czech.

File descriptions (aliases) are in Czech, English and German - depending
on the chosen language. The language when running the sample programs
depends on the language selected in the Peter environment (if it is running)
or on the system language.


Version without installer
-------------------------
Peter does not require installation for its operation, it is enough to extract
the archive ZIP file to the installation folder (use WinZip or even better
WinRar to extract it). Thus, multi-user environment is not automatically
supported, i.e. when modifying the sample program, the program will change
in the installation folder. But this can be remedied by creating a shortcut,
and setting "Run In" to the user's working folder and "Destination" to Peter's
installation folder.

Peter250.zip - Peter 2.50 full, ZIP without installation (370 MB)
https://www.breatharian.eu/Petr/download/Peter250.zip

Peter250mini.zip - Peter 2.50 mini, ZIP without installation (4 MB)
https://www.breatharian.eu/Petr/download/Peter250mini.zip

Peter241Lite.zip - Peter 2.41 Lite, ZIP without installation (4 MB)
https://www.breatharian.eu/Petr/download/Peter241Lite.zip

Peter223LiteCZ.zip - Peter 2.23 Lite, ZIP without installation, Czech help (1,8 MB)
https://www.breatharian.eu/Petr/download/Peter223LiteCZ.zip

Peter251_help.zip - Peter 2.51 help, ZIP without installation (6 MB)
https://www.breatharian.eu/Petr/download/Peter251help.zip
A modification of the Peter by Jiri Peroutka, with help rebuilt in CHM format
for Windows 10. The original version of the Peter uses help in HLP format, which
was supported by older systems. Newer Windows systems no longer support the HLP
format and it is necessary to install HLP support from Microsoft's website - the
WinHlp32.exe program. Instead, this help in CHM format can be used. It can be run
either directly or from within the Peter using a modified version of Peter 2.51.


Compilation
-----------
I wrote the Gemtree Petr program 25 years ago, when Windows 95 was the standard
operating system and the display had 256 palette colors. The functionality of
Peter is based on this. It only supports 256 palette color graphics (or possibly
limited True-Color in 3D graphics), and some features are now obsolete and
problematic in the new environment. And I just learned to program in C++ on
Peter, so the code looks like that. :-) I would have done many things differently
today.

Peter's source code is a bit problematic to compile. (by the way, sorry to
foreigners - the comments in the code are only in Czech) It requires Microsoft
VC 6.0 compiler to compile, and Windows 95 or Windows 98 environment. Reasons:

1) Standard C libraries are excluded from the compilation, Peter handles
   everything himself (e.g. as a memory allocation manager). However, this
   requires that the code uses some workarounds to accommodate the compiler.
   This makes it very difficult to adapt the compilation to another compiler.

2) The images in Peter are in compressed BMP format 256 colours with palettes.
   The display must be in 16-bit color mode. The images are edited with the
   internal editor MS VC++ 6.0. If the display was in more colors, the content
   of the images was destroyed. And if a system other than Windows 95-98 was used,
   the images were not compressed.

I've tried compiling the program on a current system with current MS VC++,
apparently it would be possible, but would require changes:

- enable linking of standard C libraries
- use standard C libraries (malloc) instead of the custom manager
- edit images with an external editor
- do not use math coprocessor rounding mode switching (solve differently)
- cancel initialization of global objects

I was eventually able to compile it, but it didn't work - it would have required
some reworking modification of the EXE files. :-)

How to compile:

Before compilation, run the commands "Setup\Res\Source\!Konv.bat", which prepares
images for the installer, and "HLP\Source\!Konv.bat", which prepares images for
help. The DOS utility alchemy.exe is used, so it is necessary to have enough DOS
memory, and Windows max XP is required, newer systems no longer support DOS
programs. Windows 98 can be run under the VirtualPC emulator on newer systems.
If VirtualPC is no longer supported on the system, you can first emulate Windows
XP in VirtualBox and then emulate Windows 98 in VirtualPC.

After converting the images, open the project in MS VC 6.0. For all projects you
must compile all variants of Release. The DelExe project must be compiled first,
then you can go alphabetically from top to bottom (Loader must come before Peter).
The environment should be Windows 98. When trying to run the compilation e.g. under
Windows XP, it will be necessary to run VC in W98 compatible mode. Peter's internal
images are edited by VC's internal editor, Peter is ready to load them (in 8-bit
BMP format with RLE). When editing the image, the display must not be set to
true-color, 16-bit color mode must be selected, otherwise the editor will corrupt
the image colors. Images must be edited under W98 and not under NT and higher, as
higher systems do not save images compressed and the programs are then much larger.
If you edit images with another editor, you must delete the *.aps files for the
compiler to load the new image. When changing the version number, correct the version
in the files Main.h, Setup\Main.h, HLP\Main_en.rtf, HLP\Main_en.rtf, HLP\Peter_en.hpj
and HLP\Peter_en.hpj.

Folders in the project:

DataInst is installer of data files (e.g. installer of sample programs), no longer
  used today.

DelExe is utility to delete the installer after uninstalling Peter.

Gener is utility to complete the Peter's installer. It compiles contents of
  subdirectories (installation groups) into package: "0"=main program with helps,
  "1"=applications of the Peter, "2"=item library, "3"=picture library, "4"=sprite
  library, "5"=sound library, "6"=music library, "7"=other libraries, "8"=sources
  to generate sprites. Peter.exe file determines date and time of installed files.

HLP is a help (compiled from RTF files to HLP using HCW compiler).

Loader is interpreter of a program created in the Peter. Peter adds to it (into
  EXE header) section ".petprg" which contains edited and executed program.
  Interpreter at startup compiles it into pseudocode which is executed later - it
  creates EXECITEM structures, which include addresses of functions to be called
  fot the entries.

Loader0 is simplified loader. It does not execute the program and serves only as
  header of the program to enable its distribution in non-executable form (to be
  smaller file).

Pov2Spr is converter of pictures to sprites.

Setup is Peter's installer.
