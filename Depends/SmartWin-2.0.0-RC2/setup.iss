; Copyright (C) 2005 Peter Kümmel. All rights reserved.
; For conditions of distribution and use, see LICENSE.txt

[Setup]
AppName=SmartWin++
AppVerName=SmartWin++ 2.0.0
AppPublisher=Thomas Hansen
AppPublisherURL=http://smartwinlib.org/
AppSupportURL=http://smartwinlib.org/
AppUpdatesURL=http://smartwinlib.org/
DefaultDirName=c:\smartwin
DefaultGroupName=SmartWin++
LicenseFile=..\SmartWin\License.txt
InfoAfterFile=..\SmartWin\ReadMe.txt
OutputBaseFilename=smartwin-2.0.0
Compression=lzma/max
SolidCompression=yes
OutputDir=..\

[Icons]
Name: "{group}\SmartWin++ 2.0.0 - Documentation"; Filename: "{app}\docs\index.chm"; WorkingDir: "{app}\docs"
Name: "{group}\Uninstall SmartWin++ 2.0.0"; Filename: "{uninstallexe}"

[Languages]
Name: "eng"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\SmartWin\*"; DestDir: "{app}"; Flags: recursesubdirs
;Source: "..\SmartWin\readme.txt"; DestDir: "c:\SmartWin";


[CODE]

var
    choosePage: TInputOptionWizardPage;
    sdkPage:    TInputDirWizardPage;
    mingwPage:  TInputDirWizardPage;

    programFiles:   string;
    sdkPath:        string;
    mingwPath:      string;
    
    id: integer;

procedure createPathPages();
begin

    // Create the choose page
    
    id :=  wpInfoAfter;
    choosePage := CreateInputOptionPage(id,
    'Build library and examples', '',
    'Check fields if you want to build the library and all examples.'#13#10 +
    'Beware, this would take a while!'#13#10 +
    'Note that build process is dependant on some system variables which might not exist on your system, if process doesnt finish with success open the makefiles and edit them by hand!'#13#10+
    'Note also that this is not nessecary for the install, if you want to you can build the library yourself by opening the SmartWinUnitTests.sln file or SmartWin.dev file'#13#10+
    'Note also that if you want to test the SmartSOAPTester1 project you need to create a virtual web directory (run:inetmgr) with the same name as the project pointing to that directory'#13#10,
    False, False);
    // Add items
    choosePage.Add('Build with GCC/MinGW');
    choosePage.Add('Build with a Microsoft compiler');
    // Set initial values (optional)
    choosePage.Values[0] := false;
    choosePage.Values[1] := false;
    id := choosePage.id;
    
    

    programFiles := GetEnv('ProgramFiles');
    sdkPath := '';
    if fileExists( programFiles + '\Microsoft SDK\SetEnv.Bat' ) then begin
        sdkPath := 'exists';
        end
    else if fileExists( programFiles + '\Microsoft Platform SDK\SetEnv.Cmd' ) then begin
        sdkPath := 'exists';
        end
    else if fileExists( programFiles + '\Microsoft Visual Studio .NET 2003\SDK\v1.1\Bin\sdkvars.bat' ) then begin
        sdkPath := 'exists'
        end
    else begin
        // Create the page to find the sdk
        sdkPage := CreateInputDirPage(id,
            'Prepare compiling', 'Microsoft Compiler Visual C++ 2003, 2005 or Toolkit 2003',
            'Please select the installation of the Microsoft Platform SDK.'#13#10#13#10, False, '');
            sdkPage.Add('Search for e.g. "Microsoft SDK" or "Microsoft Platform SDK"');
            sdkPage.Values[0] := ExpandConstant('{pf}\');
        id := sdkPage.id;
    end;
    

    mingwPath := '';
    if dirExists( programFiles + '\Dev-Cpp\bin' ) then begin
        mingwPath := programFiles + '\Dev-Cpp\bin';
        end
    else if dirExists( programFiles + '\MinGW\bin' ) then begin
        mingwPath := programFiles + '\MinGW\bin';
        end
    else begin
        // Create the page to find MinGW
        mingwPage :=    CreateInputDirPage(id,
            'Prepare compiling', 'GNU Compiler, MinGW / Dev-C++',
            'Please select bin folder of your MinGW or DevC++ installation.'#13#10 +
            'You can skip this if mingw is already in your search path.'#13#10#13#10, False, '');
            mingwPage.Add('Search for e.g. "MinGW\bin" or "Dev-Cpp\bin"');
            mingwPage.Values[0] := ExpandConstant('{pf}\');
    end;

end;



function ShouldSkipPage(PageID: Integer): Boolean;
begin
    result := false;
    if mingwPath = '' then begin
        if (PageID = mingwPage.ID) and (choosePage.values[0] = false) then    result := true;
        if (PageID = mingwPage.ID) and (choosePage.values[0] = true) then     result := false;
    end;
    if sdkPath = '' then begin
        if (PageID = sdkPage.ID) and (choosePage.values[1] = false) then    result := true;
        if (PageID = sdkPage.ID) and (choosePage.values[1] = true) then     result := false;
    end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
    ResultCode: integer;
    str: String;
begin
    if CurStep=ssDone then begin
        if choosePage.values[0] = true then begin
            if mingwPath = '' then begin
                mingwPath := '"' + mingwPage.Values[0] + '"';
            end;
            Exec(ExpandConstant('{app}\make.mingw.bat'), mingwPath,'', SW_SHOW, ewWaitUntilTerminated, ResultCode)
        end;
        if choosePage.values[1] = true then begin
            str := '';
            if sdkPath = '' then begin
                str := '"' + sdkPage.Values[0] + '"';
            end
            Exec(ExpandConstant('{app}\make.msvc.bat'), str, '', SW_SHOW, ewWaitUntilTerminated, ResultCode)
        end;
    end;
end;


procedure InitializeWizard();
begin
    createPathPages();
end;

