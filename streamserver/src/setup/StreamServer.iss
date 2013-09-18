; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "StreamServer"
#define MyAppVersion "1.5"
#define MyAppPublisher "memoryboxes@gmail.com"
#define MyAppURL "memoryboxes@gmail.com"
#define MyAppExeName "StreamServer.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={{04F7A7E5-367C-4596-9ECF-6592953A9D9F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
InfoBeforeFile=E:\work\audio_stream_broadcast\vlc\StreamServer\info.txt
OutputDir=E:\work\setup
OutputBaseFilename=StreamServer
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\StreamServer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\libvlc.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\libvlccore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\readme.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\ledscreen.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\audio.xml"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\Alarm.mp3"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\BX_IV.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\BXGPRS.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\StreamServer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\streamserver.ini"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\work\audio_stream_broadcast\vlc\StreamServer\Release\plugins\*"; DestDir: "{app}\plugins"; Flags: ignoreversion
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

