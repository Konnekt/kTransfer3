
cl -c -nologo -Zm200 -O1 -DNDEBUG -MT -EHsc -GR -W0  -I"." -I"..\..\include" -I"..\..\include\smartwin" -Fo hello.cpp

link /NOLOGO  /SUBSYSTEM:WINDOWS /incremental:no /OUT:"main-msvc.exe" hello.obj ..\..\lib\smartwin.lib comctl32.lib user32.lib Gdi32.lib opengl32.lib glu32.lib  kernel32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib uuid.lib imm32.lib comdlg32.lib Wininet.lib wsock32.lib

rm -f *.obj

