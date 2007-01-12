
rc -i".." -dDS_FIXEDSYS=0x0008L -dLANG_ENGLISH=0x09 -dSUBLANG_ENGLISH_US=0x01 -dSUBLANG_ENGLISH_UK=0x02 -dSUBLANG_NORWEGIAN_BOKMAL=0x01 -dLANG_NORWEGIAN=0x14 -fo WidgetMDIWindow.res WidgetMDIWindow.rc

cl -c -nologo -Zm200 -O1 -DNDEBUG -MT -EHsc -GR -W0  -I"." -I"..\..\include" -I"..\..\include\smartwin" -Fo Main.cpp MDIChild.cpp  WidgetMain.cpp

link /NOLOGO  /SUBSYSTEM:WINDOWS /incremental:no /OUT:"main-msvc.exe" Main.obj MDIChild.obj WidgetMain.obj WidgetMDIWindow.res ..\..\lib\smartwin.lib comctl32.lib user32.lib Gdi32.lib opengl32.lib glu32.lib  kernel32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib uuid.lib imm32.lib comdlg32.lib Wininet.lib wsock32.lib

rm -f *.obj
