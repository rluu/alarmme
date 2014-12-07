; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=AlarmME
AppVerName=AlarmME 1.0.0
AppPublisher=Ryan Luu
DefaultDirName={pf}\AlarmME
DefaultGroupName=AlarmME
AllowNoIcons=yes
LicenseFile=C:\programming\AlarmME-rel\LICENSE.txt
InfoBeforeFile=C:\programming\AlarmME-rel\README.txt
OutputDir=C:\programming\AlarmME-rel
OutputBaseFilename=AlarmME-setup-1.0.0
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags:
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\programming\AlarmME-rel\AlarmME.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\programming\AlarmME-rel\clockIcon.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\programming\AlarmME-rel\README.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\programming\AlarmME-rel\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\AlarmME"; Filename: "{app}\AlarmME.exe"; WorkingDir: "{app}"; IconFilename: "{app}\clockIcon.ico";
Name: "{group}\{cm:UninstallProgram,AlarmME}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\AlarmME"; Filename: "{app}\AlarmME.exe"; WorkingDir: "{app}"; IconFilename: "{app}\clockIcon.ico"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\AlarmME"; Filename: "{app}\AlarmME.exe"; WorkingDir: "{app}"; IconFilename: "{app}\clockIcon.ico"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\AlarmME.exe"; Description: "{cm:LaunchProgram,AlarmME}"; WorkingDir: "{app}"; Flags: nowait postinstall skipifsilent

