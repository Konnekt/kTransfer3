
cl -c -Zm200 -O1 -DNDEBUG -MT -EHsc -GR -W0  -I"." -I"..\include" Application.cpp BasicTypes.cpp Bitmap.cpp CanvasClasses.cpp CommandLine.cpp DestructionClass.cpp Font.cpp Icon.cpp ImageList.cpp LibraryLoader.cpp Message.cpp  Widget.cpp WidgetSplitter.cpp boost\signals\connection.cpp boost\signals\named_slot_map.cpp boost\signals\signal_base.cpp boost\signals\slot.cpp boost\signals\trackable.cpp

link /lib /NOLOGO /OUT:"..\lib\smartwin.lib" Application.obj BasicTypes.obj Bitmap.obj CanvasClasses.obj CommandLine.obj DestructionClass.obj Font.obj Icon.obj ImageList.obj LibraryLoader.obj Message.obj Widget.obj WidgetSplitter.obj connection.obj named_slot_map.obj signal_base.obj slot.obj trackable.obj


rm -f *.obj

