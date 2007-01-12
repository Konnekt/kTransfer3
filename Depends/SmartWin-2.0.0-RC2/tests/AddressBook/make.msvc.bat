
cl -c -nologo -Zm200 -O1 -DNDEBUG -MT -EHsc -GR -W0  -I"." -I"..\..\include" -I"..\..\include\smartwin" -Fo Main.cpp CentralWidget.cpp MainWindow.cpp TabWidget.cpp

link /NOLOGO  /SUBSYSTEM:WINDOWS /incremental:no /OUT:"main-msvc.exe" Main.obj CentralWidget.obj MainWindow.obj TabWidget.obj ..\..\lib\smartwin.lib comctl32.lib user32.lib Gdi32.lib opengl32.lib glu32.lib  kernel32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib uuid.lib imm32.lib comdlg32.lib Wininet.lib wsock32.lib

rm -f *.obj

