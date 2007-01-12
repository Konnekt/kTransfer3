
cl -c -Zm200 -O1 -DNDEBUG -MT -EHsc -GR -W0  -I"." -I"..\include" ..\SmartNetwork\HttpRequest.cpp ..\SmartNetwork\HttpResponse.cpp ..\SmartNetwork\TcpTextStream.cpp ..\SmartUtil\dateTime.cpp ..\SmartUtil\TimeSpan.cpp ..\SmartXML\XmlAttribute.cpp ..\SmartXML\XmlDocument.cpp ..\SmartXML\XmlNamespace.cpp ..\SmartXML\XmlNode.cpp

link /lib /NOLOGO /OUT:"..\lib\SmartSOAP.lib" HttpRequest.obj HttpResponse.obj TcpTextStream.obj DateTime.obj TimeSpan.obj XmlAttribute.obj XmlDocument.obj XmlNamespace.obj XmlNode.obj

rm -f *.obj

cd Tester1
call make.msvc.bat
cd ..

