
:: swDll

cl -c -nologo -Zm200 -O1 -DNDEBUG -D_WINDOWS -D_USRDLL -DSWDLL_EXPORTS -D_WINDLL -MT -EHsc -GR -W0  -I"." -I"..\..\include" -I"..\..\include\smartwin" -Fo .\swDll.cpp .\stdafx.cpp 

link /NOLOGO /DLL /OPT:REF /OPT:ICF /SUBSYSTEM:WINDOWS /IMPLIB:"swDll.lib" /incremental:no /OUT:"swDll.dll" swDll.obj stdafx.obj ..\..\lib\smartwin.lib comctl32.lib user32.lib Gdi32.lib opengl32.lib glu32.lib  kernel32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib uuid.lib imm32.lib comdlg32.lib Wininet.lib wsock32.lib libcmt.lib


:: swDllMain

cl -c -nologo -Zm200 -O1 -DNDEBUG -MT -EHsc -GR -W0  -I"." -I"..\..\include" -I"..\..\include\smartwin" -Fo .\swDllMain.cpp 

link /NOLOGO  /SUBSYSTEM:CONSOLE /incremental:no /OUT:"main-msvc.exe" swDllMain.obj ..\..\lib\smartwin.lib swDll.lib comctl32.lib user32.lib Gdi32.lib opengl32.lib glu32.lib  kernel32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib uuid.lib imm32.lib comdlg32.lib Wininet.lib wsock32.lib


rm -f *.obj

