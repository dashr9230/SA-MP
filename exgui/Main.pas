unit Main;

interface

uses
  Windows, Messages, Forms, SysUtils, Registry, WinSock, ComObj, ShellAPI, ShlObj, hh,
  FindSort, Dialogs, IdHTTP, MMSystem, ExtCtrls, XPMan, ImgList, Controls,
  Menus, ComCtrls, StdCtrls, TeEngine, Series, TeeProcs, Chart, Tabs, Graphics,
  GIFImage, ToolWin, Classes, TlHelp32, ClipBrd, ActiveX;

{$L EliRT.obj}

function RT_GetVersion(pReserved:Pointer): LongWord; stdcall; external;
function xVirtualAllocEx(hProcess: LongWord; lpAddress: Pointer; dwSize: LongWord; flAllocationType: LongWord; flProtect: LongWord): Pointer; stdcall; external;
function xCreateRemoteThread(hProcess :LongWord; lpThreadAttributes :Pointer; dwStackSize: LongWord; lpStartAddress: Pointer; lpParameter: Pointer; dwCreationFlags: LongWord; var lpThreadId: Cardinal): LongWord; stdcall; external;

const
  WM_RECV = WM_USER+1;

type
  TfmMain = class(TForm)
    sbMain: TStatusBar;
    mmMain: TMainMenu;
    miFile: TMenuItem;
    miExportFavoritesList: TMenuItem;
    N1: TMenuItem;
    miExit: TMenuItem;
    miServers: TMenuItem;
    miAddServer: TMenuItem;
    miHelp: TMenuItem;
    miAbout: TMenuItem;
    tbMain: TToolBar;
    pmServers: TPopupMenu;
    tsServerLists: TTabSet;
    miTools: TMenuItem;
    ilMain: TImageList;
    tbSettings: TToolButton;
    tbCopyServerInfo: TToolButton;
    tbMasterServerUpdate: TToolButton;
    tbAbout: TToolButton;
    tbHelp: TToolButton;
    tbConnect: TToolButton;
    tbRefreshServer: TToolButton;
    tbAddServer: TToolButton;
    tbDeleteServer: TToolButton;
    tbServerProperties: TToolButton;
    XPManifest: TXPManifest;
    pnBreakable: TPanel;
    gbFilter: TGroupBox;
    edFilterMode: TLabeledEdit;
    edFilterMap: TLabeledEdit;
    cbFilterEmpty: TCheckBox;
    cbFilterPassworded: TCheckBox;
    cbFilterFull: TCheckBox;
    gbInfo: TGroupBox;
    tbSpacer1: TToolButton;
    tbSpacer2: TToolButton;
    tbSpacer4: TToolButton;
    tbSpacer5: TToolButton;
    tbSpacer3: TToolButton;
    miConnect: TMenuItem;
    N2: TMenuItem;
    miDeleteServer: TMenuItem;
    miRefreshServer: TMenuItem;
    N3: TMenuItem;
    miMasterServerUpdate: TMenuItem;
    N4: TMenuItem;
    miCopyServerInfo: TMenuItem;
    miServerProperties: TMenuItem;
    miSettings: TMenuItem;
    miHelpTopics: TMenuItem;
    N6: TMenuItem;
    miImportFavoritesList: TMenuItem;
    piConnect: TMenuItem;
    N7: TMenuItem;
    piDeleteServer: TMenuItem;
    piRefreshServer: TMenuItem;
    N9: TMenuItem;
    piCopyServerInfo: TMenuItem;
    piServerProperties: TMenuItem;
    pnLine: TPanel;
    lbSIAddressLab: TLabel;
    lbSIModeLab: TLabel;
    lbSIMapLab: TLabel;
    lbSIPlayersLab: TLabel;
    lbSIPingLab: TLabel;
    lbSIPing: TLabel;
    lbSIPlayers: TLabel;
    lbSIMap: TLabel;
    lbSIMode: TLabel;
    edSIAddress: TEdit;
    pnSIDivider: TPanel;
    chSIPingChart: TChart;
    chSIPingLineChart: TFastLineSeries;
    tmSIPingUpdate: TTimer;
    miView: TMenuItem;
    miFilterServerInfo: TMenuItem;
    N10: TMenuItem;
    miStatusBar: TMenuItem;
    pmCopy: TPopupMenu;
    piCopy: TMenuItem;
    pnRight: TPanel;
    pnPlayers: TPanel;
    lbPlayers: TListBox;
    hcPlayers: THeaderControl;
    pnRules: TPanel;
    lbRules: TListBox;
    hcRules: THeaderControl;
    pnMain: TPanel;
    hcServers: THeaderControl;
    lbServers: TListBox;
    spRight: TSplitter;
    Splitter1: TSplitter;
    lblPlayerName: TLabel;
    N11: TMenuItem;
    miSamp: TMenuItem;
    tmrQueryQueueProcess: TTimer;
    edName: TEdit;
    label_url: TLabel;
    AddtoFavorites1: TMenuItem;
    imLogo: TImage;
    imPadlock: TImage;
    imPadlocked: TImage;
    imDownArrow: TImage;
    imUpArrow: TImage;
    tmrServerListUpdate: TTimer;
    ToolButton1: TToolButton;

    function GetToken(TokenData: String; ItemIndex: Integer; TokenDelimiter: String): String;
    function GetClipBoardStr: String;
    procedure SetClipBoardStr(Str: String);

    procedure GetGTAExe(Owner: HWND);
    procedure FormCreate(Sender: TObject);
    procedure lbServersDrawItem(Control: TWinControl; Index: Integer;
      Rect: TRect; State: TOwnerDrawState);
    procedure hcServersSectionResize(HeaderControl: THeaderControl;
      Section: THeaderSection);
    procedure tbMainResize(Sender: TObject);
    procedure hcServersSectionClick(HeaderControl: THeaderControl;
      Section: THeaderSection);
    procedure pnBreakableResize(Sender: TObject);
    procedure hcServersDrawSection(HeaderControl: THeaderControl;
      Section: THeaderSection; const Rect: TRect; Pressed: Boolean);
    procedure tmSIPingUpdateTimer(Sender: TObject);
    procedure lbServersClick(Sender: TObject);
    procedure FilterChange(Sender: TObject);
    procedure UpdateServers;
    procedure lbServersContextPopup(Sender: TObject; MousePos: TPoint;
      var Handled: Boolean);
    procedure AddServer(Server: String);
    procedure ImportFavoritesClick(Sender: TObject);
    procedure ImportFavorites(FileName: String; AddToFavs: Boolean);
    procedure ExportFavoritesClick(Sender: TObject);
    procedure ExportFavorites(FileName: String; ExportPasswords: Boolean);
    procedure ExitClick(Sender: TObject);
    procedure miViewClick(Sender: TObject);
    procedure ToggleFilterServerInfo(Sender: TObject);
    procedure ToggleStatusBar(Sender: TObject);
    procedure ConnectClick(Sender: TObject);
    procedure AddServerClick(Sender: TObject);
    procedure DeleteServerClick(Sender: TObject);
    procedure RefreshServerClick(Sender: TObject);
    procedure MasterServerUpdateClick(Sender: TObject);
    procedure CopyServerInfoClick(Sender: TObject);
    procedure ServerPropertiesClick(Sender: TObject);
    procedure SettingsClick(Sender: TObject);
    procedure RemoteConsoleClick(Sender: TObject);
    procedure HelpTopicsClick(Sender: TObject);
    procedure AboutClick(Sender: TObject);
    procedure tsServerListsChange(Sender: TObject; NewTab: Integer;
      var AllowChange: Boolean);
    procedure QueryServerInfoParse(SrcIP: String; SrcPort: Word; Buf: PAnsiChar; DataLen: Integer);
    procedure QueryServerInfoError(SocketError: Integer);
    procedure QueryServerInfo(Server: String; bPing: Boolean; bInfo: Boolean; bPlayers: Boolean; bRules: Boolean);
    procedure ServerConnect(Server: String; Port: String; Password: String);
    procedure piCopyClick(Sender: TObject);
    procedure pmCopyPopup(Sender: TObject);
    procedure lbPlayersDrawItem(Control: TWinControl; Index: Integer;
      Rect: TRect; State: TOwnerDrawState);
    procedure hcPlayersSectionResize(HeaderControl: THeaderControl;
      Section: THeaderSection);
    procedure lbRulesDrawItem(Control: TWinControl; Index: Integer;
      Rect: TRect; State: TOwnerDrawState);
    procedure hcRulesSectionResize(HeaderControl: THeaderControl;
      Section: THeaderSection);
    procedure sbMainDrawPanel(StatusBar: TStatusBar; Panel: TStatusPanel;
      const Rect: TRect);
    procedure lbPlayersExit(Sender: TObject);
    procedure lbRulesExit(Sender: TObject);
    procedure WMRecv(var Message: TMessage); message WM_RECV;
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
    function BrowseForFolder(Owner: HWND; var Directory: String; StartDir, Title: String): Boolean;
    procedure miSampClick(Sender: TObject);
    procedure tmrQueryQueueProcessTimer(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure tmServerListUpdate(Sender: TObject);
    procedure label_urlClick(Sender: TObject);
    procedure imLogoClick(Sender: TObject);
    procedure CreateFASTDesktoplink1Click(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
  end;

  TPlayerInfo = record
    Name: String;
    Score: Integer;
  end;

  TRuleInfo = record
    Rule: String;
    Value: String;
  end;

  TServerInfo = record
    Address: String;
    DottedAddress: String;
    HasAddress: Boolean;
    Port: Integer;
    Tag: Word;

    HostName: String;
    Passworded: Boolean;
    Players: Integer;
    MaxPlayers: Integer;
    Ping: Integer;
    Mode: String;
    Map: String;

    ServerPassword: String;
    RconPassword: String;

    aPlayers: Array of TPlayerInfo;
    aRules: Array of TRuleInfo;
  end;

  TSortMode = (smHostName, smPlayers, smPing, smMode, smMap);

  TSortDir = (sdUp, sdDown);

const
  FavoritesFileVersion = 1;

var
  fmMain: TfmMain;
  Servers: Array of TServerInfo;
  Filtered: Boolean = true;
  SortMode: TSortMode = smHostName;
  OldSortMode: TSortMode = smHostName;
  SortDir: TSortDir = sdUp;
  OldSortDir: TSortDir = sdUp;
  QuerySocket: Integer;
  FileTag: Array[0..3] of Char = ('S','A','M','P');
  SelServer: Integer = -1;
  PingCounter: Integer = 0;
  MasterFile: Integer = 1;
  gta_sa_exe: String;
  QueryQueue: TStringList;
  ServersTopIndex: Integer = -1;
  byte_4ED6C4: Boolean = false;
  dword_4EF08C: TStringList;
  FavoritesChanged: Boolean;

implementation

uses
  ImportFavorites, ExportFavorites, ServerProperties,
  RconConfig, Settings, About, Rcon, MasterUpdate,
  unit_webrunform;

{$R *.dfm}

procedure sub_4E1CEC();
begin
  if byte_4ED6C4 <> true then begin
    CreateMutex(nil, true, 'kyeman and spookie woz ''ere, innit.');
    if GetLastError = ERROR_ALREADY_EXISTS then begin
      MessageBox(0, 'SA:MP is already running.'#10#10'You can only run one instance at a time.', 'SA:MP Error', MB_ICONERROR);
      ExitProcess(0);
    end;
    byte_4ED6C4:= true;
  end;
end;

function sub_4E1DA8(): String;
var
  szPath: Array[0..MAX_PATH] Of Char;
begin
  if SHGetSpecialFolderPath(0, szPath, CSIDL_PERSONAL, False) <> FALSE then
    Result:= szPath + '\GTA San Andreas User Files\SAMP\'
  else
    Result:= '';
end;

procedure sub_4E1E6C(a1, a2: String);
var
  v16: IUnknown;
  v15: IShellLink;
  v14: IPersistFile;
  v13, v12: String;
  v11: Array[0..264] Of Char;
  szPath: Array[0..MAX_PATH] Of Char;
  v10: WideString;
begin
  v12:= Application.ExeName;

  v16 := CreateComObject(CLSID_ShellLink);
  v15 := v16 as IShellLink;
  v14 := v16 as IPersistFile;

  v15.SetPath(PChar(v12));
  v15.SetWorkingDirectory(PChar(ExtractFilePath(v12)));
  v15.SetArguments(PChar(a1));

  SHGetSpecialFolderPath(0, szPath, CSIDL_DESKTOP, False);

  v13:= szPath + String('\') + a2 + '.lnk';

  v14.Save(PWideChar(v13), false);
end;

function CompareHostName(a, b: Pointer): Integer;
begin
  Result:= CompareText(TServerInfo(a^).HostName, TServerInfo(b^).HostName);
end;

function ComparePlayers(a, b: Pointer): Integer;
begin
  if TServerInfo(a^).Players > TServerInfo(b^).Players then
    Result:= 1
  else if TServerInfo(a^).Players = TServerInfo(b^).Players then
      Result:= 0
    else
      Result:= -1;
end;

function ComparePing(a, b: Pointer): Integer;
begin
  if TServerInfo(a^).Ping > TServerInfo(b^).Ping then
    Result:= 1
  else if TServerInfo(a^).Ping = TServerInfo(b^).Ping then
      Result:= 0
    else
      Result:= -1;
end;

function CompareMode(a, b: Pointer): Integer;
begin
  Result:= CompareText(TServerInfo(a^).Mode, TServerInfo(b^).Mode);
end;

function CompareMap(a, b: Pointer): Integer;
begin
  Result:= CompareText(TServerInfo(a^).Map, TServerInfo(b^).Map);
end;

procedure sub_4E220C(a1: String);
var
  v10: String;
  v9: Integer;

  function sub_4E2140(a1, a2: string): Integer;
  var
    v13: Integer;
  begin
    Result:= 0;
    for v13:= (Length(a2) - Length(a1) + 1) downto 1 do begin
      if Copy(a2, v13, Length(a1)) = a1 then begin
        Result:= v13;
        Exit;
      end;
    end;
  end;

begin
  if (Copy(a1, 2, 1) <> ':') then begin
    if (Copy(a1, 3, 1) <> '\') then begin
      if (Copy(a1, 1, 1) = '\') then begin
        a1:= 'C:' + a1;
      end else begin
        a1:= 'C:\' + a1;
      end;
    end else begin
      a1 := 'C:' + a1;
    end;
  end;
  if not DirectoryExists(a1) then begin
    v9:= sub_4E2140('\', a1);
    v10:= Copy(a1, 1, v9 - 1);
    if not DirectoryExists(v10) then
      sub_4E220C(v10);
    CreateDir(a1);
  end;
end;

function TfmMain.GetToken(TokenData: String; ItemIndex: Integer; TokenDelimiter: String): String;
var
  i, Len, DelimLen, TokenCount: Integer;
  TokenString: String;
begin
  TokenCount:= 0;
  i:= 0;
  TokenString:= '';
  Len:= Length(TokenData);
  DelimLen := Length(TokenDelimiter);
  if (Len > 0) then begin
    while i <= Len do begin
      if (TokenCount = (ItemIndex - 1)) then begin
        if (Copy(TokenData,i+1,DelimLen) = TokenDelimiter) then
          break;
        TokenString := TokenString + Copy(TokenData,i+1,1);
      end;
      if (Copy(TokenData,i+1,DelimLen) = TokenDelimiter) then begin
        Inc(TokenCount);
        Inc(i, DelimLen-1);
      end;
      Inc(i);
    end;
  end;
  Result:= TokenString;
end;

function GetIPFromHost(const HostName: string): string;
type
  TaPInAddr = array[0..10] of PInAddr;
  PaPInAddr = ^TaPInAddr;
var
  phe: PHostEnt;
  pptr: PaPInAddr;
  i: Integer;
  GInitData: TWSAData;
begin
  if dword_4EF08C.Values[HostName] <> '' then begin
    Result := '';
    Exit;
  end;
  WSAStartup($101, GInitData);
  Result := '';
  phe := GetHostByName(PChar(HostName));
  if phe = nil then begin
    Result:= '';
    dword_4EF08C.Values[HostName]:= '1';
    Exit;
  end;
  pPtr := PaPInAddr(phe^.h_addr_list);
  i := 0;
  while pPtr^[i] <> nil do
  begin
    Result := inet_ntoa(pptr^[i]^);
    Inc(i);
  end;
  WSACleanup;
end;

function sub_4E2628(a1: TColor): TColor;
begin
  Result:= a1;
  if GetRValue(ColorToRGB(a1)) > 16 then begin
    if GetGValue(ColorToRGB(a1)) > 16 then begin
      if GetBValue(ColorToRGB(a1)) > 16 then begin
        Result:= RGB(
          GetRValue(ColorToRGB(a1))-16,
          GetGValue(ColorToRGB(a1))-16,
          GetBValue(ColorToRGB(a1))-16);
      end;
    end;
  end;
end;

function TfmMain.GetClipBoardStr: String;
begin
  Result:= Clipboard.AsText;
end;

procedure TfmMain.SetClipBoardStr(Str: String);
begin
  Clipboard.AsText:= Str;
end;

procedure TfmMain.GetGTAExe(Owner: HWND);
var
  Reg: TRegistry;
  TmpStr, BrowseExe: String;
begin
  Reg:= TRegistry.Create;
  Reg.RootKey:= HKEY_CURRENT_USER;
  Reg.OpenKey('SOFTWARE\Rockstar Games\GTA San Andreas\Installation', false);
  if Reg.ValueExists('ExePath') then
    TmpStr:= Reg.ReadString('ExePath')
  else begin
    Reg.RootKey:= HKEY_LOCAL_MACHINE;
    Reg.OpenKey('SOFTWARE\Rockstar Games\GTA San Andreas\Installation', false);
    if Reg.ValueExists('ExePath') then
      TmpStr:= Reg.ReadString('ExePath');
  end;
  Reg.CloseKey;

  TmpStr:= ExtractFilePath(Copy(TmpStr, 2, Length(TmpStr)-2));

  if BrowseForFolder(Owner, BrowseExe, TmpStr, 'Please locate your GTA: San Andreas installtion...') then begin
    gta_sa_exe:= BrowseExe + '\gta_sa.exe';

    Reg.RootKey:= HKEY_CURRENT_USER;
    Reg.OpenKey('SOFTWARE\SAMP', true);
    Reg.WriteString('gta_sa_exe', gta_sa_exe);
    Reg.CloseKey;
    Reg.Free;
  end;
end;

procedure TfmMain.FormCreate(Sender: TObject);
var
  Dummy: Boolean;
  WSData: WSADATA;
  s_in: sockaddr_in;
  Reg: TRegistry;
begin
  dword_4EF08C:= TStringList.Create;
  FavoritesChanged:= false;

  Reg:= TRegistry.Create;
  Reg.RootKey:= HKEY_CURRENT_USER;
  Reg.OpenKey('SOFTWARE\SAMP', true);

  if Reg.ValueExists('gta_sa_exe') then
    gta_sa_exe:= Reg.ReadString('gta_sa_exe')
  else
    GetGTAExe(Handle);

  if Reg.ValueExists('PlayerName') then
    edName.Text:= Reg.ReadString('PlayerName');
  Reg.CloseKey;
  Reg.Free;

  Randomize;

  WSAStartup($0202, WSData);

  QuerySocket:= Socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
  ZeroMemory(@s_in, sizeof(s_in));
  s_in.sin_addr.S_addr:= INADDR_ANY;
  s_in.sin_family:= AF_INET;
  s_in.sin_port:= 0;
  bind(QuerySocket, s_in, sizeof(s_in));
  WSAAsyncSelect(QuerySocket, Handle, WM_RECV, FD_READ);

  QueryQueue:= TStringList.Create;
  tmrQueryQueueProcess.Enabled:= true;
  tmrServerListUpdate.Enabled:= true;

  sub_4E220C(sub_4E1DA8);

  if FileExists(sub_4E1DA8 + 'USERDATA.DAT') then
    ImportFavorites(sub_4E1DA8 + 'USERDATA.DAT', false);

  tsServerListsChange(Self, 0, Dummy);
  lbServersClick(Self);

  UpdateServers;
end;

procedure TfmMain.lbServersDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
var
  i, Idx: Integer;
  TempRect: TRect;
begin
  with (Control as TListBox) do begin
    Idx:= StrToIntDef(Items.Strings[Index], 0);
    if Idx >= Length(Servers) then Exit;

    Canvas.Pen.Color:= clBtnHighlight;
    Canvas.Pen.Style:= psClear;

    if odSelected in State then begin
      Canvas.Font.Color:= clHighlightText;
      Canvas.Brush.Color:= clHighlight;
    end else begin
      Canvas.Font.Color:= clWindowText;
      if (Index mod 2) = 1 then
        Canvas.Brush.Color:= clWindow
      else
        Canvas.Brush.Color:= sub_4E2628(clWindow);
    end;

    Inc(Rect.Right);
    Canvas.Rectangle(Rect);
    Dec(Rect.Right);
    Canvas.Pen.Style:= psSolid;
    Canvas.PenPos:= Point(Rect.Right, Rect.Bottom-1);
    Canvas.LineTo(Rect.Left, Rect.Bottom-1);
    for i:= 0 to 5 do begin
      Canvas.PenPos:= Point(hcServers.Sections.Items[i].Right-1, Rect.Top);
      Canvas.LineTo(hcServers.Sections.Items[i].Right-1, Rect.Bottom);
    end;

    if Servers[Idx].Passworded then
      BitBlt(Canvas.Handle, 7, Rect.Top + 1, 13, 14, imPadlocked.Canvas.Handle, 0, 0, SRCCOPY)
    else
      BitBlt(Canvas.Handle, 7, Rect.Top + 1, 13, 14, imPadlock.Canvas.Handle, 0, 0, SRCCOPY);

    TempRect:= Classes.Rect(hcServers.Sections.Items[1].Left + 2, Rect.Top + 2, hcServers.Sections.Items[1].Right - 2, Rect.Bottom - 2);
    DrawText(Canvas.Handle, PChar(Servers[Idx].HostName), -1, TempRect, DT_LEFT);

    TempRect:= Classes.Rect(hcServers.Sections.Items[2].Left + 2, Rect.Top + 2, hcServers.Sections.Items[2].Right - 2, Rect.Bottom - 2);
    DrawText(Canvas.Handle, PChar(IntToStr(Servers[Idx].Players) + ' / ' + IntToStr(Servers[Idx].MaxPlayers)), -1, TempRect, DT_LEFT);

    TempRect:= Classes.Rect(hcServers.Sections.Items[3].Left + 2, Rect.Top + 2, hcServers.Sections.Items[3].Right - 2, Rect.Bottom - 2);
    if Servers[Idx].Ping = 9999 then
      DrawText(Canvas.Handle, '-', -1, TempRect, DT_LEFT)
    else
      DrawText(Canvas.Handle, PChar(IntToStr(Servers[Idx].Ping)), -1, TempRect, DT_LEFT);

    TempRect:= Classes.Rect(hcServers.Sections.Items[4].Left + 2, Rect.Top + 2, hcServers.Sections.Items[4].Right - 2, Rect.Bottom - 2);
    DrawText(Canvas.Handle, PChar(Servers[Idx].Mode), -1, TempRect, DT_LEFT);

    TempRect:= Classes.Rect(hcServers.Sections.Items[5].Left + 2, Rect.Top + 2, hcServers.Sections.Items[5].Right - 2, Rect.Bottom - 2);
    DrawText(Canvas.Handle, PChar(Servers[Idx].Map), -1, TempRect, DT_LEFT);
  end;
end;

procedure TfmMain.hcServersSectionResize(HeaderControl: THeaderControl;
  Section: THeaderSection);
begin
  lbServers.Repaint;
end;

procedure TfmMain.tbMainResize(Sender: TObject);
begin
  ToolButton1.Width:= ((Sender as TToolBar).Width-ToolButton1.Left)-imLogo.Width;
  imLogo.Repaint;
end;

procedure TfmMain.hcServersSectionClick(HeaderControl: THeaderControl;
  Section: THeaderSection);
begin
  case Section.Index of
    1:
    begin
      if SortMode = smHostName then begin
        if SortDir = sdUp then
          SortDir:= sdDown
        else
          SortDir:= sdUp;
      end else begin
        SortMode:= smHostName;
        SortDir:= sdUp;
      end;
    end;
    2:
    begin
      if SortMode = smPlayers then begin
        if SortDir = sdUp then
          SortDir:= sdDown
        else
          SortDir:= sdUp;
      end else begin
        SortMode:= smPlayers;
        SortDir:= sdDown;
      end;
    end;
    3:
    begin
      if SortMode = smPing then begin
        if SortDir = sdUp then
          SortDir:= sdDown
        else
          SortDir:= sdUp;
      end else begin
        SortMode:= smPing;
        SortDir:= sdUp;
      end;
    end;
    4:
    begin
      if SortMode = smMode then begin
        if SortDir = sdUp then
          SortDir:= sdDown
        else
          SortDir:= sdUp;
      end else begin
        SortMode:= smMode;
        SortDir:= sdUp;
      end;
    end;
    5:
    begin
      if SortMode = smMap then begin
        if SortDir = sdUp then
          SortDir:= sdDown
        else
          SortDir:= sdUp;
      end else begin
        SortMode:= smMap;
        SortDir:= sdUp;
      end;
    end;
  end;

  UpdateServers;
end;

procedure TfmMain.pnBreakableResize(Sender: TObject);
begin
  gbInfo.Width:= pnBreakable.Width - gbFilter.Width + 1;
  chSIPingChart.Width:= gbInfo.Width - 272;
  if chSIPingChart.Width > 50 then
    chSIPingChart.Visible:= true
  else
    chSIPingChart.Visible:= false;
end;

var
  ReRender: Boolean;
procedure TfmMain.hcServersDrawSection(HeaderControl: THeaderControl;
  Section: THeaderSection; const Rect: TRect; Pressed: Boolean);
var
  DoIt: Boolean;
  TempRect: TRect;
begin
  DoIt:= false;

  Inc(TempRect.Left, 2);
  Inc(TempRect.Top, 2);
  Dec(TempRect.Right, 2);
  Dec(TempRect.Bottom, 2);

  if (Section.Index = 1) and (SortMode = smHostName) then begin
    DoIt:= true;
  end else if (Section.Index = 2) and (SortMode = smPlayers) then begin
    DoIt:= true;
  end else if (Section.Index = 3) and (SortMode = smPing) then begin
    DoIt:= true;
  end else if (Section.Index = 4) and (SortMode = smMode) then begin
    DoIt:= true;
  end else if (Section.Index = 5) and (SortMode = smMap) then begin
    DoIt:= true;
  end;

  TempRect:= Rect;
  Inc(TempRect.Left, 2);
  Inc(TempRect.Top, 1);

  if DoIt then begin
    if SortDir = sdDown then
      BitBlt(HeaderControl.Canvas.Handle, Rect.Left + 2, Rect.Top + 2, 9, 12, imDownArrow.Canvas.Handle, 0, 0, SRCCOPY)
    else
      BitBlt(HeaderControl.Canvas.Handle, Rect.Left + 2, Rect.Top + 2, 9, 12, imUpArrow.Canvas.Handle, 0, 0, SRCCOPY);
    Inc(TempRect.Left, 10);
  end;

  DrawText(HeaderControl.Canvas.Handle, PChar(Section.Text), -1, TempRect, DT_LEFT);

  if not ReRender then begin
    ReRender:= true;
    HeaderControl.Repaint;
    ReRender:= false;
  end;
end;

procedure TfmMain.tmServerListUpdate(Sender: TObject);
begin
  if QueryQueue.Count > 0 then
    UpdateServers;
  lbPlayers.Invalidate;
  lbRules.Invalidate;
end;

procedure TfmMain.tmSIPingUpdateTimer(Sender: TObject);
var
  Idx: Integer;
  PingOnly: Boolean;
begin
  if lbServers.ItemIndex = -1 then Exit;

  if GetForegroundWindow <> Handle then Exit;

  Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
  if Idx >= Length(Servers) then Exit;
  
  if PingCounter = 5 then begin
    PingCounter:= 0;
    PingOnly:= false;
  end else begin
    Inc(PingCounter);
    PingOnly:= true;
  end;

  if PingOnly then
    QueryServerInfo(Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port) + '#' + IntToStr(Servers[Idx].Tag), true, false, false, false)
  else
    QueryServerInfo(Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port) + '#' + IntToStr(Servers[Idx].Tag), true, true, true, true);
end;

procedure TfmMain.lbServersClick(Sender: TObject);
var
  Enabled: Boolean;
begin
  if SelServer <> lbServers.ItemIndex then begin
    SelServer:= lbServers.ItemIndex;
    chSIPingChart.Series[0].Clear;
  end;
  lbPlayers.Clear;
  lbRules.Clear;

  Enabled:= lbServers.ItemIndex <> -1;

  tbDeleteServer.Enabled:= MasterFile = 0;
  miDeleteServer.Enabled:= tbDeleteServer.Enabled;

  tbConnect.Enabled:= Enabled;
  miConnect.Enabled:= Enabled;
  tbRefreshServer.Enabled:= Enabled;
  miRefreshServer.Enabled:= Enabled;
  tbCopyServerInfo.Enabled:= Enabled;
  miCopyServerInfo.Enabled:= Enabled;
  tbServerProperties.Enabled:= Enabled;
  miServerProperties.Enabled:= Enabled;

  if lbServers.ItemIndex = -1 then begin
    edSIAddress.Text:= '- - -';
    lbSIPlayers.Caption:= '- - -';
    lbSIPing.Caption:= '- - -';
    lbSIMode.Caption:= '- - -';
    lbSIMap.Caption:= '- - -';
    gbInfo.Caption:= ' Server Info ';
    Exit;
  end;

  RefreshServerClick(Self);
end;

procedure TfmMain.FilterChange(Sender: TObject);
begin
  UpdateServers;
end;

procedure TfmMain.UpdateServers;
var
  ActualIdx, Idx, i: Integer;
  OldServ: String;
  FilterList: TStringList;
  ItemFiltered: Boolean;
  Sorted: Boolean;
  TrackingChanges: Boolean;
  TotServers, TotSlots, TotPlayers: Integer;
  NewServs: TStringList;
  TopIndexes: Array[1..3] of Integer;
  TopIndexSaved: Integer;
begin
  if QueryQueue.Count > 0 then begin
    lbServers.Items.BeginUpdate;
    TrackingChanges:= true;
  end;

  TopIndexSaved:= lbServers.TopIndex;

  NewServs:= TStringList.Create;

  ServersTopIndex:= lbServers.TopIndex;
  TopIndexes[1]:= lbServers.TopIndex;
  TopIndexes[2]:= lbPlayers.TopIndex;
  TopIndexes[3]:= lbRules.TopIndex;

  Idx:= -1;
  ActualIdx:= lbServers.ItemIndex;
  if ActualIdx <> -1 then begin
    Idx:= StrToInt(lbServers.Items.Strings[ActualIdx]);
    if Idx < Length(Servers) then
      OldServ:= Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port);
  end;

  Sorted:= false;
  if (OldSortMode <> SortMode) or (OldSortDir <> SortDir) then begin
    OldSortMode:= SortMode;
    OldSortDir:= SortDir;
    Sorted:= true;
    case SortMode of
      smHostName: SortArray(Servers[0], 0, SizeOf(TServerInfo), 0, Length(Servers)-1, CompareHostName);
      smPlayers : SortArray(Servers[0], 0, SizeOf(TServerInfo), 0, Length(Servers)-1, ComparePlayers);
      smPing    : SortArray(Servers[0], 0, SizeOf(TServerInfo), 0, Length(Servers)-1, ComparePing);
      smMode    : SortArray(Servers[0], 0, SizeOf(TServerInfo), 0, Length(Servers)-1, CompareMode);
      smMap     : SortArray(Servers[0], 0, SizeOf(TServerInfo), 0, Length(Servers)-1, CompareMap);
    end;
  end;

  if Filtered then begin
    FilterList:= TStringList.Create;
    for i:= 0 to Length(Servers) -1 do begin
      ItemFiltered:= false;
      // Check the mode filter...
      if edFilterMode.Text <> '' then
        if Pos(Lowercase(edFilterMode.Text), Lowercase(Servers[i].Mode)) = 0 then
          ItemFiltered:= true;
      // Check the mode filter...
      if edFilterMap.Text <> '' then
        if Pos(Lowercase(edFilterMap.Text), Lowercase(Servers[i].Map)) = 0 then
          ItemFiltered:= true;
      // Check the "Not Full" filter...
      if cbFilterFull.Checked then
        if Servers[i].Players = Servers[i].MaxPlayers then
          ItemFiltered:= true;
      // Check the "Not Empty" filter...
      if cbFilterEmpty.Checked then
        if Servers[i].Players = 0 then
          ItemFiltered:= true;
      // Check the "Not Passworded" filter...
      if cbFilterPassworded.Checked then
        if Servers[i].Passworded then
          ItemFiltered:= true;
      // Server hasn't responded yet.
      if (Servers[i].MaxPlayers < 1) and (MasterFile <> 0) then
        ItemFiltered:= true;
      // If this server hasn't been filtered...
      if not ItemFiltered then
        FilterList.Add(IntToStr(i));
    end;

    if SortDir = sdDown then begin
      for i:= FilterList.Count -1 downto 0 do
        NewServs.Add(FilterList.Strings[i]);
    end else
      NewServs.Text:= FilterList.Text;
    FilterList.Free;
  end else begin
    if SortDir = sdDown then begin
      for i:= Length(Servers) -1 downto 0 do
        if (Servers[i].MaxPlayers > 0) or (MasterFile = 0) then
          NewServs.Add(IntToStr(i));
    end else begin
      for i:= 0 to Length(Servers) -1 do
        if (Servers[i].MaxPlayers > 0) or (MasterFile = 0) then
          NewServs.Add(IntToStr(i));
    end;
  end;

  if lbServers.Items.Text <> NewServs.Text then begin
    lbServers.Items.Text:= NewServs.Text;
    lbServers.TopIndex:= TopIndexes[1];
  end;
  if (ActualIdx <> -1) and (Idx <> -1) and (Idx < Length(Servers)) then begin
    NewServs.Clear;
    for i:= 0 to Length(Servers[Idx].aPlayers)-1 do
      NewServs.Add(IntToStr(i));
    if lbPlayers.Items.Text <> NewServs.Text then begin
      lbPlayers.Items.Text:= NewServs.Text;
      lbPlayers.TopIndex:= TopIndexes[2];
    end;
    NewServs.Clear;

    label_url.Caption:= '';

    for i:= 0 to Length(Servers[Idx].aRules)-1 do begin
      NewServs.Add(IntToStr(i));
      if Servers[Idx].aRules[i].Rule = 'weburl' then
        label_url.Caption:= Trim(Servers[Idx].aRules[i].Value);
    end;
    if lbRules.Items.Text <> NewServs.Text then begin
      lbRules.Items.Text:= NewServs.Text;
      lbRules.TopIndex:= TopIndexes[3];
    end;
  end;
  NewServs.Free;

  if Sorted then begin
    for i:= 0 to lbServers.Count -1 do begin
      Idx:= StrToInt(lbServers.Items.Strings[i]);
      if Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port) = OldServ then begin
        lbServers.ItemIndex:= i;
        //lbServersClick(Self);
        break;
      end;
    end;
  end else
    if ActualIdx <> -1 then
      if ActualIdx < lbServers.Items.Count then
        lbServers.ItemIndex:= ActualIdx;

  if Filtered then begin
    if lbServers.ItemIndex <> -1 then begin
      Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
      if Idx < Length(Servers) then begin
        edSIAddress.Text:= Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port);
        lbSIPlayers.Caption:= IntToStr(Servers[Idx].Players) + ' / ' + IntToStr(Servers[Idx].MaxPlayers);
        lbSIPing.Caption:= IntToStr(Servers[Idx].Ping);
        lbSIMode.Caption:= Servers[Idx].Mode;
        lbSIMap.Caption:= Servers[Idx].Map;
        gbInfo.Caption:= ' Server Info: ' + Servers[Idx].HostName + ' ';
      end;
    end;
  end;

  TotServers:= lbServers.Items.Count;
  TotSlots:= 0;
  TotPlayers:= 0;
  for i:= 0 to Length(Servers) -1 do begin
    Inc(TotSlots, Servers[i].MaxPlayers);
    Inc(TotPlayers, Servers[i].Players);
  end;
  sbMain.SimpleText:= 'Servers: '+IntToStr(TotPlayers)+' players, playing on '+IntToStr(TotServers)+' servers. ('+IntToStr(TotSlots)+' player slots available)';

  if lbServers.ItemIndex = -1 then
    lbServers.ItemIndex:= 0;
  if lbServers.Items.Count >= TopIndexSaved then
    lbServers.TopIndex:= TopIndexSaved;
  if TrackingChanges = true then
    lbServers.Items.EndUpdate;
  Application.ProcessMessages;
end;

procedure TfmMain.lbServersContextPopup(Sender: TObject; MousePos: TPoint;
  var Handled: Boolean);
begin
  Handled:= lbServers.ItemIndex = -1;
end;

procedure TfmMain.ImportFavoritesClick(Sender: TObject);
var
  OD: TOpenDialog;
  fmImportFavorites: TfmImportFavorites;
  AddToFavs: Boolean;
begin
  tsServerLists.TabIndex:= 0;

  OD:= TOpenDialog.Create(Self);
  OD.DefaultExt:= 'fav';
  OD.Filter:= 'SA-MP Favorites List (*.fav)|*.fav';
  OD.Options:= [ofEnableSizing,ofFileMustExist];
  OD.Title:= 'Import Favorites';
  if not OD.Execute then begin
    OD.Free;
    Exit;
  end;

  fmImportFavorites:= TfmImportFavorites.Create(Application);
  fmImportFavorites.ShowModal;
  AddToFavs:= fmImportFavorites.rbAddToCurrent.Checked;
  fmImportFavorites.Free;

  ImportFavorites(OD.FileName, AddToFavs);
  OD.Free;

  ExportFavorites(sub_4E1DA8 + 'USERDATA.DAT', true);
end;

procedure TfmMain.ImportFavorites(FileName: String; AddToFavs: Boolean);
var
  ImportFile: File;
  Temp, Temp2, i, j, x: Integer;
  Dubble: Boolean;
  ThisTag: Array[0..3] of Char;
begin
  AssignFile(ImportFile, FileName);
  FileMode:= 0;
  Reset(ImportFile, 1);
  BlockRead(ImportFile, ThisTag[0], 4);
  if ThisTag <> FileTag then begin
    MessageDlg('Invalid SA-MP file.', mtError, [mbOk], 0);
    CloseFile(ImportFile);
    Exit;
  end;
  BlockRead(ImportFile, Temp, 4);
  if Temp <> FavoritesFileVersion then begin
    MessageDlg('Bad SA-MP favorites file version.'#13#10#13#10'Your client may need updating.', mtError, [mbOk], 0);
    CloseFile(ImportFile);
    Exit;
  end;

  // Number of servers in file
  BlockRead(ImportFile, Temp2, 4);

  if not AddToFavs then
    SetLength(Servers, 0);

  for j:= 1 to Temp2 do begin
    Dubble:= false;
    i:= Length(Servers);
    SetLength(Servers, i+1);

    // Address
    BlockRead(ImportFile, Temp, 4);
    SetLength(Servers[i].Address, Temp);
    if Temp <> 0 then
      BlockRead(ImportFile, Servers[i].Address[1], Temp);

    // Port
    BlockRead(ImportFile, Servers[i].Port, 4);

    for x:= 0 to Length(Servers)-2 do begin
      if (Servers[x].Address = Servers[i].Address) and (Servers[x].Port = Servers[i].Port) then begin
        Dubble:= true;
        Break;
      end;
    end;

    // Hostname
    BlockRead(ImportFile, Temp, 4);
    SetLength(Servers[i].HostName, Temp);
    BlockRead(ImportFile, Servers[i].HostName[1], Temp);
    // Server password
    BlockRead(ImportFile, Temp, 4);
    SetLength(Servers[i].ServerPassword, Temp);
    if Temp <> 0 then
      BlockRead(ImportFile, Servers[i].ServerPassword[1], Temp);
    // Rcon password
    BlockRead(ImportFile, Temp, 4);
    SetLength(Servers[i].RconPassword, Temp);
    if Temp <> 0 then
      BlockRead(ImportFile, Servers[i].RconPassword[1], Temp);

    Servers[i].Ping:= 9999;
    Servers[i].Tag:= Random($FFFF);

    if Dubble then
      SetLength(Servers, i)
    else
      QueryQueue.Add(Servers[i].Address + ':' + IntToStr(Servers[i].Port) + '#' + IntToStr(Servers[i].Tag));
      //QueryServerInfo(Servers[i].Address + ':' + IntToStr(Servers[i].Port), true, true, false, false);
  end;
  CloseFile(ImportFile);

  UpdateServers;
end;

procedure TfmMain.ExportFavoritesClick(Sender: TObject);
var
  SD: TSaveDialog;
  fmExportFavorites: TfmExportFavorites;
  ExportPasswords: Boolean;
begin
  tsServerLists.TabIndex:= 0;

  SD:= TSaveDialog.Create(Self);
  SD.DefaultExt:= 'fav';
  SD.Filter:= 'SA-MP Favorites List (*.fav)|*.fav';
  SD.Options:= [ofHideReadOnly,ofEnableSizing];
  SD.Title:= 'Export Favorites';
  if not SD.Execute then begin
    SD.Free;
    Exit;
  end else begin
    if FileExists(SD.FileName) then begin
      if MessageDlg('File '''+SD.FileName+''' already exists. Overwrite?', mtConfirmation, [mbYes, mbNo], 0) = mrNo then begin
        SD.Free;
        Exit;
      end;
    end;
  end;

  fmExportFavorites:= TfmExportFavorites.Create(Application);
  fmExportFavorites.ShowModal;
  ExportPasswords:= fmExportFavorites.cbIncludeSavedPasswords.Checked;
  fmExportFavorites.Free;

  ExportFavorites(SD.FileName, ExportPasswords);
  SD.Free;
end;

procedure TfmMain.ExportFavorites(FileName: String; ExportPasswords: Boolean);
var
  ExportFile: File;
  Temp, i: Integer;
begin
  AssignFile(ExportFile, FileName);
  FileMode:= 1;
  Rewrite(ExportFile, 1);
  BlockWrite(ExportFile, FileTag[0], 4);
  Temp:= FavoritesFileVersion;
  BlockWrite(ExportFile, Temp, 4);
  Temp:= Length(Servers);
  BlockWrite(ExportFile, Temp, 4);
  for i:= 0 to Length(Servers)-1 do begin
    Temp:= Length(Servers[i].Address);
    BlockWrite(ExportFile, Temp, 4);
    if Temp <> 0 then
      BlockWrite(ExportFile, Servers[i].Address[1], Temp);
    BlockWrite(ExportFile, Servers[i].Port, 4);
    Temp:= Length(Servers[i].HostName);
    BlockWrite(ExportFile, Temp, 4);
    BlockWrite(ExportFile, Servers[i].HostName[1], Temp);
    if ExportPasswords then begin
      Temp:= Length(Servers[i].ServerPassword);
      BlockWrite(ExportFile, Temp, 4);
      if Temp <> 0 then
        BlockWrite(ExportFile, Servers[i].ServerPassword[1], Temp);
      Temp:= Length(Servers[i].RconPassword);
      BlockWrite(ExportFile, Temp, 4);
      if Temp <> 0 then
        BlockWrite(ExportFile, Servers[i].RconPassword[1], Temp);
    end else begin
      Temp:= 0;
      BlockWrite(ExportFile, Temp, 4);
      BlockWrite(ExportFile, Temp, 4);
    end;
  end;
  CloseFile(ExportFile);
end;

procedure TfmMain.ExitClick(Sender: TObject);
begin
  Close;
end;

procedure TfmMain.miViewClick(Sender: TObject);
begin
  miFilterServerInfo.Checked:= Filtered;
  miStatusBar.Checked:= sbMain.Visible;
end;

procedure TfmMain.ToggleFilterServerInfo(Sender: TObject);
begin
  if Filtered then begin
    Filtered:= false;
    pnBreakable.Height:= 16;
    gbInfo.Caption:= ' Server Info ';
  end else begin
    Filtered:= true;
    pnBreakable.Height:= 100;
    pnBreakable.Top:= 0;
    if lbServers.ItemIndex <> -1 then
      gbInfo.Caption:= ' Server Info: ' + Servers[StrToInt(lbServers.Items.Strings[lbServers.ItemIndex])].HostName + ' ';
  end;
  UpdateServers;
end;

procedure TfmMain.ToggleStatusBar(Sender: TObject);
begin
  sbMain.Visible:= not sbMain.Visible;
  sbMain.Top:= 10000;
end;

procedure TfmMain.ConnectClick(Sender: TObject);
var
  Idx: Integer;
  SrvPwd: String;
  NickName: String;
begin
  if lbServers.ItemIndex = -1 then Exit;
  Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
  if Idx >= Length(Servers) then Exit;

  SrvPwd:= Servers[Idx].ServerPassword;
  if Servers[Idx].Passworded then
    if not InputQuery('Server Password', 'This server requires a password...', SrvPwd) then
      Exit;
  if edName.Text = '' then begin
    if not InputQuery('Who are you?', 'Enter your nickname/handle...', NickName) then
      Exit;
    if NickName = '' then
      Exit;
    edName.Text:= NickName;
  end;

  ServerConnect(Servers[Idx].Address, IntToStr(Servers[Idx].Port), SrvPwd);
end;

procedure TfmMain.AddServerClick(Sender: TObject);
var
  Server: String;
  Dummy: Boolean;
begin
  Server:= GetClipBoardStr;
  if (MasterFile <> 0) and (lbServers.ItemIndex <> -1) then begin
    Server:= Servers[StrToIntDef(lbServers.Items.Strings[lbServers.ItemIndex], 0)].Address + ':' +
              IntToStr(Servers[StrToIntDef(lbServers.Items.Strings[lbServers.ItemIndex], 0)].Port);

    tsServerLists.TabIndex:= 0;
    tsServerListsChange(tsServerLists, 0, Dummy);
  end;
  if InputQuery('Add Server', 'Enter new server HOST:PORT...', Server) then
    if Server <> '' then begin
      AddServer(Server);
    end;
end;

procedure TfmMain.AddServer(Server: String);
var
  i, j: Integer;
  Dupe: Boolean;
begin
      Server:= Trim(Server);
      i:= Length(Servers);
      SetLength(Servers, i + 1);
      if Pos(':', Server) <> 0 then begin
        Servers[i].Address:= Copy(Server, 1, Pos(':', Server)-1);
        Servers[i].Port:= StrToIntDef(Copy(Server, Pos(':', Server)+1, 5), 7777);
      end else begin
        Servers[i].Address:= Server;
        Servers[i].Port:= 7777;
      end;
      Servers[i].HostName:= '(Retrieving info...) ' + Servers[i].Address + ':' + IntToStr(Servers[i].Port);
      Dupe:= False;
      for j:= 0 to i-1 do
        if (Servers[j].Address = Servers[i].Address) and (Servers[j].Port = Servers[i].Port) then begin
          SetLength(Servers, i);
          MessageDlg('This server is already on your list.', mtError, [mbOk], 0);
          Dupe:= True;
        end;
        if not Dupe then begin
          //QueryServerInfo(Server, true, true, false, false);
          QueryQueue.Add(Server);
          ExportFavorites(sub_4E1DA8 + 'USERDATA.DAT', true);
          FavoritesChanged:= true;
        end;
      UpdateServers;
end;

procedure TfmMain.DeleteServerClick(Sender: TObject);
var
  Idx, i: Integer;
begin
  if lbServers.ItemIndex = -1 then Exit;
  if tsServerLists.TabIndex <> 0 then Exit;

  Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
  lbServers.Items.Delete(lbServers.ItemIndex);
  for i:= Idx to Length(Servers)-2 do
    Servers[i]:= Servers[i+1];
  SetLength(Servers, Length(Servers)-1);
  UpdateServers;

  ExportFavorites(sub_4E1DA8 + 'USERDATA.DAT', true);
  FavoritesChanged:= true;
end;

procedure TfmMain.RefreshServerClick(Sender: TObject);
var
  Idx: Integer;
begin
  if lbServers.ItemIndex = -1 then Exit;
  Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
  if Idx >= Length(Servers) then Exit;

  QueryServerInfo(Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port) + '#' + IntToStr(Servers[Idx].Tag), true, true, true, true);
end;

procedure TfmMain.MasterServerUpdateClick(Sender: TObject);
var
  SL: TStringList;
  i: Integer;
  IdHTTP: TIdHTTP;
  fmMasterUpdate: TfmMasterUpdate;
begin
  fmMasterUpdate:= TfmMasterUpdate.Create(Application);
  fmMasterUpdate.Show;
  Enabled:= false;
  Application.ProcessMessages;

  IdHTTP:= TIdHTTP.Create(Application);
  with IdHTTP do begin
    ReadTimeout:= 20000; // 20secs
    AllowCookies:= true;

    ProxyParams.BasicAuthentication:= False;
    ProxyParams.ProxyPort:= 0;

    Port:= 80;
    Host:= 'lists.sa-mp.com';
    Request.Host:= IdHTTP.Host;

    Request.ContentLength:= -1;
    Request.ContentRangeEnd:= 0;
    Request.ContentRangeStart:= 0;
    Request.ContentType:= 'text/html';
    Request.Accept:= 'text/html, */*';
    Request.BasicAuthentication:= False;
    Request.UserAgent:= 'Mozilla/3.0 (compatible; SA:MP v0.3.7)';

    HTTPOptions:= [hoForceEncodeParams];
  end;

  SL:= TStringList.Create;
  try
    if MasterFile = 1 then
      SL.Text:= IdHTTP.Get('/0.3.7/internet')
    else if MasterFile = 2 then
      SL.Text:= IdHTTP.Get('/0.3.7/hosted');
  except
    // STATUS: BAD RESPONCE FROM MASTER SERVER
  end;
  IdHTTP.Free;
  lbServers.Clear;
  lbPlayers.Clear;
  lbRules.Clear;
  SetLength(Servers, 0);
  tmrQueryQueueProcess.Enabled := false;
  tmrServerListUpdate.Enabled := false;
  SetLength(Servers, SL.Count);

  Enabled:= true;
  fmMasterUpdate.Close;
  fmMasterUpdate.Free;

  for i:= 0 to SL.Count -1 do begin
    //Application.ProcessMessages;
    Servers[i].HostName:= '(Retrieving info...) ' + SL.Strings[i];
    Servers[i].Address:= Copy(SL.Strings[i], 1, Pos(':', SL.Strings[i])-1);
    Servers[i].Port:= StrToIntDef(Copy(SL.Strings[i], Pos(':', SL.Strings[i])+1, 5), 7777);
    Servers[i].Ping:= 9999;
    Servers[i].Tag:= Random($FFFF);
    //QueryServerInfo(SL.Strings[i], true, true, false, false);
    //Sleep(10);
    QueryQueue.Add(SL.Strings[i] + '#' + IntToStr(Servers[i].Tag));
  end;

  tmrQueryQueueProcess.Enabled := true;
  tmrServerListUpdate.Enabled := true;
  SL.Free;
  dword_4EF08C.Clear;

  //UpdateServers;

end;

procedure TfmMain.CopyServerInfoClick(Sender: TObject);
var
  Idx: Integer;
  Str: String;
begin
  if lbServers.ItemIndex = -1 then Exit;
  Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
  if Idx >= Length(Servers) then Exit;
  
  Str:= 'HostName: ' + Servers[Idx].HostName + #13#10 +
        'Address:  ' + Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port) + #13#10 +
        'Players:  ' + IntToStr(Servers[Idx].Players) + ' / ' + IntToStr(Servers[Idx].MaxPlayers) + #13#10 +
        'Ping:     ' + IntToStr(Servers[Idx].Ping) + #13#10 +
        'Mode:     ' + Servers[Idx].Mode + #13#10 +
        'Language: ' + Servers[Idx].Map;
  SetClipBoardStr(Str);
end;

procedure TfmMain.ServerPropertiesClick(Sender: TObject);
var
  fmServerProperties: TfmServerProperties;
  Idx: Integer;
begin
  if lbServers.ItemIndex = -1 then Exit;

  Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
  if Idx >= Length(Servers) then Exit;
  
  fmServerProperties:= TfmServerProperties.Create(Application);
  with fmServerProperties do begin
    lbHostName.Caption:= Servers[Idx].HostName;
    edAddress.Text:= Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port);
    lbPlayers.Caption:= IntToStr(Servers[Idx].Players) + ' / ' + IntToStr(Servers[Idx].MaxPlayers);
    lbPing.Caption:= IntToStr(Servers[Idx].Ping);
    lbMode.Caption:= Servers[Idx].Mode;
    lbMap.Caption:= Servers[Idx].Map;
    edServerPassword.Text:= Servers[Idx].ServerPassword;
    edRconPassword.Text:= Servers[Idx].RconPassword;
    edServerPassword.Enabled:= Servers[Idx].Passworded;
    if not Servers[Idx].Passworded then
      edServerPassword.Color:= clBtnFace;
    ShowModal;
    Free;
  end;
end;

procedure TfmMain.RemoteConsoleClick(Sender: TObject);
var
  fmRconConfig: TfmRconConfig;
  Idx: Integer;
begin
  fmRconConfig:= TfmRconConfig.Create(Application);
  if lbServers.ItemIndex <> -1 then begin
    Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
    fmRconConfig.edHost.Text:= Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port);
    fmRconConfig.edPassword.Text:= Servers[Idx].RconPassword;
  end;
  fmRconConfig.ShowModal;
  fmRconConfig.Free;
end;

procedure TfmMain.SettingsClick(Sender: TObject);
var
  fmSettings: TfmSettings;
begin
  fmSettings:= TfmSettings.Create(Application);
  fmSettings.ShowModal;
  fmSettings.Free;
end;

procedure TfmMain.HelpTopicsClick(Sender: TObject);
begin
  ShellExecute(Handle, 'open', 'http://wiki.sa-mp.com/', nil, nil, SW_SHOWNORMAL);
end;

procedure TfmMain.AboutClick(Sender: TObject);
begin
  fmAbout.ShowModal;
end;

procedure TfmMain.tsServerListsChange(Sender: TObject; NewTab: Integer;
  var AllowChange: Boolean);
begin
  if (tsServerLists.TabIndex = 0) and (FavoritesChanged = True) then begin
    ExportFavorites(sub_4E1DA8 + 'USERDATA.DAT', true);
    FavoritesChanged:= False;
  end;

  QueryQueue.Clear;
  lbServers.Clear;
  lbPlayers.Clear;
  lbRules.Clear;
  SetLength(Servers, 0);

  if NewTab = 0 then begin
    if FileExists(sub_4E1DA8 + 'USERDATA.DAT') then
      ImportFavorites(sub_4E1DA8 + 'USERDATA.DAT', false);
  end;

  {
  if NewTab <> 0 then begin
    lbServers.Clear;
    SetLength(Servers, 0);
  end;
  }

  UpdateServers;

  tbMasterServerUpdate.Enabled:= NewTab <> 0;
  miMasterServerUpdate.Enabled:= NewTab <> 0;

  //tbAddServer.Enabled:= NewTab = 0;
  miAddServer.Enabled:= NewTab = 0;
  tbDeleteServer.Enabled:= NewTab = 0;
  miDeleteServer.Enabled:= NewTab = 0;
  piDeleteServer.Visible:= NewTab = 0;

  if lbServers.Count > 0 then
    lbServers.ItemIndex:= 0;
  MasterFile:= NewTab;
  lbServersClick(Self);
  if NewTab <> 0 then
    MasterServerUpdateClick(Self);
  dword_4EF08C.Clear;
end;

procedure TfmMain.QueryServerInfoParse(SrcIP: String; SrcPort: Word; Buf: PAnsiChar; DataLen: Integer);
var
  StrIP: String;
  Tag: Word;
  i, j, Idx: Integer;
  Magic: array[0..3] of char;
  ping: Cardinal;
  TempWord: Word;
  TempCard: Cardinal;
  TempByte: Byte;
  BufPos: Integer;
  Value: Double;
  RepaintServerList,
  RepaintPlayerList,
  RepaintRulesList: Boolean;
  TempInt: Integer;
  Port: Word;
begin


  if DataLen < 11 then Exit; // 10b is min size: 4b magic, 4b ip, 2b port, 1b id

  Move(Buf[0], Magic[0], 4);
  if Magic <> 'SAMP' then Exit;

  StrIP:= IntToStr(Byte(Buf[4]))+'.'+IntToStr(Byte(Buf[5]))+'.'+
          IntToStr(Byte(Buf[6]))+'.'+IntToStr(Byte(Buf[7]));

  Move(Buf[8], Port, 2);

  Tag:= SrcPort;

  if SrcIP <> StrIP then Exit;

  Idx:= -1;
  for i:= 0 to Length(Servers)-1 do begin
    if Servers[i].DottedAddress = '' then begin
      if not Servers[i].HasAddress then begin
        Servers[i].DottedAddress:= GetIPFromHost(Servers[i].Address);
        Servers[i].HasAddress:= true;
      end;
    end;
    if (Servers[i].Address = SrcIP) or (Servers[i].DottedAddress = SrcIP) then begin
      if Servers[i].Port = SrcPort then begin
        Idx:= i;
        break;
      end;
    end;
    if (Servers[i].Address = SrcIP) and (Servers[i].Port = SrcPort) then begin
      Idx:= i;
      break;
    end;
  end;
  
  if Idx = -1 then begin
   //fmMain.Caption:= 'Server not found ' + StrIP;
   Exit;
  end;

  if (Servers[i].Tag <> 0) and (Servers[i].Tag <> Port) then begin
    //fmMain.Caption:= 'Invalid tag for ' + StrIP;
    Exit;
  end;

  RepaintServerList:= false;
  RepaintPlayerList:= false;
  RepaintRulesList:= false;

  case Buf[10] of // PacketID
    'p': // Ping
    begin
      //OutputDebugString(PChar('[P] Packet from ' + Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port)));
      if DataLen = 15 then begin
        Move(Buf[11], ping, 4);
        timeBeginPeriod(5);
        Servers[Idx].Ping:= timeGetTime - ping;

        Value:= Servers[Idx].Ping;
        if Value < 1 then Value:= 1;
        i:= chSIPingChart.Series[0].AddY(Value, '', clBlue);
        if i > 60 then begin
          for j:= 1 to 61 do begin
            chSIPingChart.Series[0].YValue[j-1]:= chSIPingChart.Series[0].YValue[j];
          end;
          chSIPingChart.Series[0].Delete(61);
        end;

        chSIPingChart.LeftAxis.Maximum:= chSIPingChart.Series[0].MaxYValue / 2 * 3;

        timeEndPeriod(5);

        RepaintServerList:= true;
      end;
    end;

    'i': // Info
    begin
      BufPos:= 11;

      //OutputDebugString(PChar('[I] Packet from ' + Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port)));

      Move(Buf[BufPos], TempByte, 1);
      Inc(BufPos, 1);
      Servers[Idx].Passworded:= TempByte <> 0;

      Move(Buf[BufPos], TempWord, 2);
      if TempWord > 1000 then TempWord:= 1000;
      Inc(BufPos, 2);
      Servers[Idx].Players:= TempWord;

      Move(Buf[BufPos], TempWord, 2);
      if TempWord > 1000 then TempWord:= 1000;
      Inc(BufPos, 2);
      Servers[Idx].MaxPlayers:= TempWord;

      if Servers[Idx].Players > Servers[Idx].MaxPlayers then
        Servers[Idx].Players:= Servers[Idx].MaxPlayers;

      Servers[Idx].HostName:= '-';
      Servers[Idx].Mode:= '-';
      Servers[Idx].Map:= '-';

      Move(Buf[BufPos], TempCard, 4);
      if (TempCard > 0) and (TempCard < 64) then begin
        Inc(BufPos, 4);
        SetLength(Servers[Idx].HostName, TempCard);
        Move(Buf[BufPos], Servers[Idx].HostName[1], TempCard);
        Inc(BufPos, TempCard);

        Move(Buf[BufPos], TempCard, 4);
        if (TempCard > 0) and (TempCard < 40) then begin
          Inc(BufPos, 4);
          SetLength(Servers[Idx].Mode, TempCard);
          Move(Buf[BufPos], Servers[Idx].Mode[1], TempCard);
          Inc(BufPos, TempCard);

          Move(Buf[BufPos], TempCard, 4);
          if (TempCard > 0) and (TempCard < 40) then begin
            Inc(BufPos, 4);
            SetLength(Servers[Idx].Map, TempCard);
            Move(Buf[BufPos], Servers[Idx].Map[1], TempCard);
            //Inc(BufPos, TempCard);
          end;
        end;
      end;

      RepaintServerList:= true;

      QueryServerInfo(Servers[Idx].Address+':'+IntToStr(Servers[Idx].Port)+'#'+IntToStr(Servers[Idx].Tag), true, false, false, false);
    end;

    'c': // Players
    begin
      BufPos:= 11;
      //OutputDebugString(PChar('[C] Packet from ' + Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port)));
      Move(Buf[BufPos], TempWord, 2);
      if TempWord > 100 then TempWord:= 100;
      Inc(BufPos, 2);
      Servers[Idx].Players:= TempWord;
      SetLength(Servers[Idx].aPlayers, TempWord);

      for i:= 0 to TempWord -1 do begin
        Move(Buf[BufPos], TempByte, 1);
        Inc(BufPos, 1);
        if BufPos > DataLen then break;
        SetLength(Servers[Idx].aPlayers[i].Name, TempByte);
        Move(Buf[BufPos], Servers[Idx].aPlayers[i].Name[1], TempByte);
        Inc(BufPos, TempByte);
        if BufPos > DataLen then break;
        Move(Buf[BufPos], TempInt, 4);
        if TempInt > 1000000 then TempInt:= 1000000;
        if TempInt < 0 then TempInt:= 0;
        Servers[Idx].aPlayers[i].Score:= TempInt;
        Inc(BufPos, 4);
        if BufPos > DataLen then break;
      end;

      RepaintPlayerList:= true;
    end;

    'r': // Rules
    begin
      BufPos:= 11;
      //OutputDebugString(PChar('[R] Packet from ' + Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port)));
      Move(Buf[BufPos], TempWord, 2);
      if TempWord > 30 then TempWord:= 30;
      Inc(BufPos, 2);
      SetLength(Servers[Idx].aRules, TempWord);

      for i:= 0 to TempWord -1 do begin
        if BufPos > DataLen then break;
        Move(Buf[BufPos], TempByte, 1);
        Inc(BufPos, 1);
        if BufPos > DataLen then break;
        SetLength(Servers[Idx].aRules[i].Rule, TempByte);
        Move(Buf[BufPos], Servers[Idx].aRules[i].Rule[1], TempByte);
        Inc(BufPos, TempByte);

        if BufPos > DataLen then break;

        Move(Buf[BufPos], TempByte, 1);
        Inc(BufPos, 1);
        if BufPos > DataLen then break;
        SetLength(Servers[Idx].aRules[i].Value, TempByte);
        Move(Buf[BufPos], Servers[Idx].aRules[i].Value[1], TempByte);
        Inc(BufPos, TempByte);
      end;

      RepaintRulesList:= true;
    end;
  end;

  if QueryQueue.Count <= 0 then
    UpdateServers;

  if RepaintServerList then
    lbServers.Invalidate;
  if RepaintPlayerList then
    lbPlayers.Invalidate;
  if RepaintRulesList then
    lbRules.Invalidate;
end;

procedure TfmMain.QueryServerInfoError(SocketError: Integer);
var
  err: array[0..512] of Char;
begin
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nil, SocketError, 0, @err, 512, nil);
  MessageDlg(err, mtError, [mbOk], 0);
end;

procedure TfmMain.QueryServerInfo(Server: String; bPing: Boolean; bInfo: Boolean; bPlayers: Boolean; bRules: Boolean);
var
  Buf: PByteArray;
  Ticks: Cardinal;

  ToAddr: TSockAddr;
  ToLen: Integer;

  Host: String;
  Port: Word;
  Tag: Word;

  ColPos, TagPos: Integer;
begin
  Tag:= 0;

  if Pos(':', Server) <> 0 then begin
    if Pos('#', Server) <> 0 then begin
      ColPos:= Pos(':', Server);
      TagPos:= Pos('#', Server);

      Host:= Copy(Server, 1, ColPos-1);
      Port:= StrToIntDef(Copy(Server, ColPos+1, TagPos-(ColPos+1)), 7777);
      Tag:= StrToIntDef(Copy(Server, TagPos+1, Length(Server)-TagPos), 0);
    end else begin
      ColPos:= Pos(':', Server);

      Host:= Copy(Server, 1, ColPos-1);
      Port:= StrToIntDef(Copy(Server, ColPos+1, Length(Server)-ColPos+1), 7777);
    end;
  end else begin
    Host:= Server;
    Port:= 7777;
  end;

  if Tag = 0 then
    Tag:= Port;

  Host:= GetIPFromHost(Host);
  if (Length(Host) < 7) or (Length(Host) > 15) then
    Exit;

  GetMem(Buf, 15);
  Buf[0]:= Byte('S'); // Magic
  Buf[1]:= Byte('A');
  Buf[2]:= Byte('M');
  Buf[3]:= Byte('P');

  Buf[4]:= StrToIntDef(GetToken(Host, 1, '.'), 0); // IP
  Buf[5]:= StrToIntDef(GetToken(Host, 2, '.'), 0);
  Buf[6]:= StrToIntDef(GetToken(Host, 3, '.'), 0);
  Buf[7]:= StrToIntDef(GetToken(Host, 4, '.'), 0);

  //Move(Port, Buf[8], 2); // Port

  ZeroMemory(@ToAddr, SizeOf(ToAddr));
  ToAddr.sin_family:= AF_INET;
  ToAddr.sin_port:= htons(Port);
  ToAddr.sin_addr.S_addr:= inet_addr(PChar(Host));
  ToLen:= SizeOf(ToAddr);

  Move(Tag, Buf[8], 2); // Tag

  if bInfo = true then begin
    Buf[10]:= Byte('i'); // Info Packet Id
    sendto(QuerySocket, Buf[0], 11, 0, ToAddr, ToLen);
    Sleep(1);
  end;

  if bPing = true then begin
    Buf[10]:= Byte('p'); // Ping Packet Id
    timeBeginPeriod(1);
    Ticks:= timeGetTime;
    timeEndPeriod(1);
    Move(Ticks, Buf[11], 4);
    sendto(QuerySocket, Buf[0], 15, 0, ToAddr, ToLen);
    Sleep(1);
  end;

  if bPlayers = true then begin
    Buf[10]:= Byte('c'); // Players Packet Id
    sendto(QuerySocket, Buf[0], 11, 0, ToAddr, ToLen);
    Sleep(1);
  end;

  if bRules = true then begin
    Buf[10]:= Byte('r'); // Rules Packet Id
    sendto(QuerySocket, Buf[0], 11, 0, ToAddr, ToLen);
    Sleep(1);
  end;

  FreeMem(Buf, 15);
end;

procedure TfmMain.ServerConnect(Server: String; Port: String; Password: String);
procedure GetDebugPrivs;
var
  hToken: THandle;
  xTokenPriv: TTokenPrivileges;
  iRetLen: DWord;
begin
  If OpenProcessToken(GetCurrentProcess, TOKEN_ADJUST_PRIVILEGES or TOKEN_QUERY, hToken) Then Begin
    LookupPrivilegeValue(nil, 'SeDebugPrivilege', xTokenPriv.Privileges[0].Luid);
    xTokenPriv.PrivilegeCount:= 1;
    xTokenPriv.Privileges[0].Attributes:= SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, False, xTokenPriv, 0, nil, iRetLen);
  End;
end;
const
  ACL_REVISION = 2;
var
  StartInfo: TStartupInfo;
  ProcInfo: TProcessInformation;
  Created: Boolean;
  hThread: THandle;
  pLibRemote: Pointer;
  NumBytes, ThreadID: Cardinal;
  CmdLine, SAMP_DLL: String;
begin
  if not FileExists(gta_sa_exe) then begin
    MessageDlg('GTA: San Andreas executable not found.'#13#10'('+gta_sa_exe+')'#13#10#13#10'Please locate it now.', mtError, [mbOk], 0);
    GetGTAExe(Handle);
  end;
  if not FileExists(gta_sa_exe) then begin
    MessageDlg('GTA: San Andreas executable STILL not found.'#13#10'('+gta_sa_exe+')'#13#10#13#10'Aborting launch.', mtError, [mbOk], 0);
    Exit;
  end;

  FillChar(StartInfo, SizeOf(TStartupInfo), 0);
  FillChar(ProcInfo, SizeOf(TProcessInformation), 0);
  StartInfo.cb:= SizeOf(TStartupInfo);

  CmdLine:= ' -c -n ' + edName.Text + ' -h ' + GetIPFromHost(Server) + ' -p ' + Port;
  if Password <> '' then
    CmdLine:= CmdLine + ' -z ' + Password;

  Created:= CreateProcess(nil, PChar('"' + gta_sa_exe + '"' + CmdLine), {@SA}nil, nil, false,
            CREATE_NEW_PROCESS_GROUP+NORMAL_PRIORITY_CLASS+CREATE_SUSPENDED,
            nil, PChar(ExtractFilePath(gta_sa_exe)), StartInfo, ProcInfo);

  if not Created then begin
    MessageDlg('Unable to execute.', mtError, [mbOk], 0);
    Exit;
  end;

  if RT_GetVersion(nil) shr 31 = 0 then
    GetDebugPrivs;

  SAMP_DLL:= ExtractFilePath(gta_sa_exe) + 'samp.dll';
  SetLength(SAMP_DLL, Length(SAMP_DLL)+1);
  SAMP_DLL[Length(SAMP_DLL)]:= #0;

  pLibRemote:= xVirtualAllocEx(ProcInfo.hProcess, nil, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
  WriteProcessMemory(ProcInfo.hProcess, pLibRemote, PChar(SAMP_DLL), Length(SAMP_DLL), NumBytes);
  hThread:= xCreateRemoteThread(ProcInfo.hProcess, nil, 0, GetProcAddress(GetModuleHandle('kernel32'), 'LoadLibraryA'), pLibRemote, 0, ThreadID);

  WaitForSingleObject(hThread, 2000);
  CloseHandle(hThread);
  VirtualFreeEx(ProcInfo.hProcess, pLibRemote, MAX_PATH, MEM_RELEASE);
  ResumeThread(ProcInfo.hThread);
  //CloseHandle(ProcInfo.hProcess);
end;

procedure TfmMain.piCopyClick(Sender: TObject);
begin
  SetClipBoardStr(edSIAddress.Text);
end;

procedure TfmMain.pmCopyPopup(Sender: TObject);
begin
  piCopy.Enabled:= edSIAddress.Text <> '- - -';
end;

procedure TfmMain.lbPlayersDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
var
  Idx: Integer;
  TempRect: TRect;
begin
  if lbServers.ItemIndex = -1 then Exit;

  with (Control as TListBox) do begin
    Idx:= StrToIntDef(lbServers.Items.Strings[lbServers.ItemIndex], 0);
    if Idx >= Length(Servers) then Exit;
    
    if Index >= Length(Servers[Idx].aPlayers) then
      Exit;

    Canvas.Pen.Color:= clBtnHighlight;
    Canvas.Pen.Style:= psClear;

    if odSelected in State then begin
      Canvas.Font.Color:= clHighlightText;
      Canvas.Brush.Color:= clHighlight;
    end else begin
      Canvas.Font.Color:= clWindowText;
      if (Index mod 2) = 1 then
        Canvas.Brush.Color:= clWindow
      else
        Canvas.Brush.Color:= sub_4E2628(clWindow);
    end;

    Inc(Rect.Right);
    Canvas.Rectangle(Rect);
    Dec(Rect.Right);
    Canvas.Pen.Style:= psSolid;
    Canvas.PenPos:= Point(Rect.Right, Rect.Bottom-1);
    Canvas.LineTo(Rect.Left, Rect.Bottom-1);

    Canvas.PenPos:= Point(hcPlayers.Sections.Items[0].Right-2, Rect.Top);
    Canvas.LineTo(hcPlayers.Sections.Items[0].Right-2, Rect.Bottom);
    Canvas.PenPos:= Point(hcPlayers.Sections.Items[1].Right-2, Rect.Top);
    Canvas.LineTo(hcPlayers.Sections.Items[1].Right-2, Rect.Bottom);

    TempRect:= Classes.Rect(hcPlayers.Sections.Items[0].Left + 2, Rect.Top + 2, hcPlayers.Sections.Items[0].Right - 2, Rect.Bottom - 2);
    DrawText(Canvas.Handle, PChar(Servers[Idx].aPlayers[Index].Name), -1, TempRect, DT_LEFT);

    TempRect:= Classes.Rect(hcPlayers.Sections.Items[1].Left + 2, Rect.Top + 2, hcPlayers.Sections.Items[1].Right - 2, Rect.Bottom - 2);
    DrawText(Canvas.Handle, PChar(IntToStr(Servers[Idx].aPlayers[Index].Score)), -1, TempRect, DT_LEFT);
  end;
end;

procedure TfmMain.hcPlayersSectionResize(HeaderControl: THeaderControl;
  Section: THeaderSection);
begin
  lbPlayers.Repaint;
end;

procedure TfmMain.lbRulesDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
var
  Idx: Integer;
  TempRect: TRect;
begin
  if lbServers.ItemIndex = -1 then Exit;

  with (Control as TListBox) do begin
    Idx:= StrToIntDef(lbServers.Items.Strings[lbServers.ItemIndex], 0);
    if Idx >= Length(Servers) then Exit;
    
    if Index >= Length(Servers[Idx].aRules) then
      Exit;

    Canvas.Pen.Color:= clBtnHighlight;
    Canvas.Pen.Style:= psClear;

    if odSelected in State then begin
      Canvas.Font.Color:= clHighlightText;
      Canvas.Brush.Color:= clHighlight;
    end else begin
      Canvas.Font.Color:= clWindowText;
      if (Index mod 2) = 1 then
        Canvas.Brush.Color:= clWindow
      else
        Canvas.Brush.Color:= sub_4E2628(clWindow);
    end;

    Inc(Rect.Right);
    Canvas.Rectangle(Rect);
    Dec(Rect.Right);
    Canvas.Pen.Style:= psSolid;
    Canvas.PenPos:= Point(Rect.Right, Rect.Bottom-1);
    Canvas.LineTo(Rect.Left, Rect.Bottom-1);

    Canvas.PenPos:= Point(hcRules.Sections.Items[0].Right-2, Rect.Top);
    Canvas.LineTo(hcRules.Sections.Items[0].Right-2, Rect.Bottom);
    Canvas.PenPos:= Point(hcRules.Sections.Items[1].Right-2, Rect.Top);
    Canvas.LineTo(hcRules.Sections.Items[1].Right-2, Rect.Bottom);

    TempRect:= Classes.Rect(hcRules.Sections.Items[0].Left + 2, Rect.Top + 2, hcRules.Sections.Items[0].Right - 2, Rect.Bottom - 2);
    DrawText(Canvas.Handle, PChar(Servers[Idx].aRules[Index].Rule), -1, TempRect, DT_LEFT);

    TempRect:= Classes.Rect(hcRules.Sections.Items[1].Left + 2, Rect.Top + 2, hcRules.Sections.Items[1].Right - 2, Rect.Bottom - 2);
    DrawText(Canvas.Handle, PChar(Servers[Idx].aRules[Index].Value), -1, TempRect, DT_LEFT);
  end;
end;

procedure TfmMain.hcRulesSectionResize(HeaderControl: THeaderControl;
  Section: THeaderSection);
begin
  lbRules.Repaint;
end;

procedure TfmMain.sbMainDrawPanel(StatusBar: TStatusBar;
  Panel: TStatusPanel; const Rect: TRect);
var
  Pcnt: Integer;
begin
  StatusBar.Canvas.Brush.Color:= clBtnFace;
  StatusBar.Canvas.Rectangle(Rect);

  StatusBar.Canvas.Brush.Color:= $00804000;
  Pcnt:= Round(((Rect.Right-Rect.Left) / 100.0) * 75.0);
  StatusBar.Canvas.Rectangle(Rect.Left, Rect.Top, Rect.Left+Pcnt, Rect.Bottom);
end;

procedure TfmMain.lbPlayersExit(Sender: TObject);
begin
  lbPlayers.ItemIndex:= -1;
end;

procedure TfmMain.lbRulesExit(Sender: TObject);
begin
  lbRules.ItemIndex:= -1;
end;

procedure TfmMain.WMRecv(var Message: TMessage);
var
  lpBuffer: Array[0..2048] of Char;
  BufLen: Integer;

  FromAddr: TSockAddr;
  FromLen: Integer;

  SrcAddr: String;
  SrcPort: Word;
begin
  ZeroMemory(@lpBuffer, sizeof(lpBuffer));

  ZeroMemory(@FromAddr, sizeof(FromAddr));
  FromAddr.sin_family:= AF_INET;
  FromLen:= SizeOf(FromAddr);

  BufLen:= recvfrom(QuerySocket, lpBuffer, 2048, 0, FromAddr, FromLen);
  SrcAddr:= inet_ntoa(FromAddr.sin_addr);
  SrcPort:= htons(FromAddr.sin_port);

  while (BufLen > 0) do begin
    //OutputDebugString( PChar('[*] of size ' + IntToStr(BufLen)) );
    QueryServerInfoParse(SrcAddr,SrcPort,lpBuffer,BufLen);
    ZeroMemory(@lpBuffer, sizeof(lpBuffer));
    BufLen:= recvfrom(QuerySocket, lpBuffer, 2048, 0, FromAddr, FromLen);
    SrcAddr:= inet_ntoa(FromAddr.sin_addr);
    SrcPort:= htons(FromAddr.sin_port);
  end;

end;

procedure TfmMain.FormDestroy(Sender: TObject);
var
  Reg: TRegistry;
begin
  Reg:= TRegistry.Create;
  Reg.RootKey:= HKEY_CURRENT_USER;
  Reg.OpenKey('SOFTWARE\SAMP', true);
  Reg.WriteString('PlayerName', edName.Text);
  Reg.CloseKey;
  Reg.Free;
  
  CloseSocket(QuerySocket);
  ShutDown(QuerySocket, 0);
  WSACleanup;

  QueryQueue.Free;

  if (tsServerLists.TabIndex = 0) and (FavoritesChanged = True) then begin
    ExportFavorites(sub_4E1DA8 + 'USERDATA.DAT', True);
    FavoritesChanged:= False;
  end;

  Sleep(0);
end;

procedure TfmMain.FormShow(Sender: TObject);
var
  ServFull, ServAddr, ServPort, ServPass: String;
  NewServer: String;
begin
  //SetProcessAffinityMask(GetCurrentProcess(),1);
  lbServers.DoubleBuffered:= true;
  lbPlayers.DoubleBuffered:= true;
  lbRules.DoubleBuffered:= true;
  lbServers.SetFocus;

  if ParamCount > 0 then begin
    ServFull:= ParamStr(1);
    if Copy(ServFull, 0, 7) = 'samp://' then begin
      if ParamCount > 1 then
        ServPass:= ParamStr(2);
      ServFull:= StringReplace(ServFull, 'samp://', '', [rfReplaceAll, rfIgnoreCase]);
      ServFull:= StringReplace(ServFull, '/', '', [rfReplaceAll, rfIgnoreCase]);
      if Pos(':', ServFull) <> 0 then begin
        ServAddr:= Copy(ServFull, 0, Pos(':', ServFull)-1);
        ///////////////////////////////////////////////
        //
        // Delphi 7 compiler bug(?)
        //
        // With this structure:
        // ServPort:= IntToStr(StrToIntDef(Copy(ServFull, Pos(':', ServFull)+1, Length(ServFull)-Pos(':', ServFull)+1), 7777));
        // the compiler seems to gets confused and will not generate
        // LStrLAsg call after Sysutils::IntToStr to set ServPort variable.
        // Workaround is to break down the structure.
        //
        ///////////////////////////////////////////////
        ServPort:= Copy(ServFull, Pos(':', ServFull)+1, Length(ServFull)-Pos(':', ServFull)+1);
        ServPort:= IntToStr(StrToIntDef(ServPort, 7777));
      end else begin
        ServAddr:= Copy(ServFull, 0, Length(ServFull));
        ServPort:= '7777';
      end;
      wnd_webrunform.Label1.Caption:= 'Do you want to add ' + ServAddr + ':' + ServPort + ' to your favorites ' + #13#10 + 'or play on this server now?';
      Case wnd_webrunform.ShowModal Of
        mrOk: begin
          ServerConnect(ServAddr, ServPort, ServPass);
          Application.Terminate;
        end;
        mrYes: begin
          sub_4E1CEC;
          NewServer:= ServAddr + ':' + ServPort;
          if InputQuery('Add Server', 'Enter new server HOST:PORT...', NewServer) <> False then
            if NewServer <> '' then
              AddServer(NewServer);
        end;
        mrCancel: ;
      end;
    end else begin
      ServFull:= ParamStr(1);
      if ParamCount > 1 then
        ServPass:= ParamStr(2);
      if Pos(':', ServFull) <> 0 then begin
        ServAddr:= Copy(ServFull, 1, Pos(':', ServFull)-1);
        ServPort:= IntToStr(StrToIntDef(Copy(ServFull, Pos(':', ServFull)+1, 5), 7777));
      end else begin
        ServAddr:= ServFull;
        ServPort:= '7777';
      end;
      ServerConnect(ServAddr, ServPort, ServPass);
      Application.Terminate;
    end;
  end;
  sub_4E1CEC;
end;

function BrowseCallbackProc(hwnd: HWND; uMsg: UINT; lParam, lpData: LPARAM): Integer; stdcall;
begin
  case uMsg of
    BFFM_INITIALIZED:
    Begin
      SetWindowText(hWnd, 'GTA: San Andreas Installation');
      SendMessage(hwnd, BFFM_SETSELECTION, 1, lpData);
    End;
  end;
  Result:= 0;
end;

function TfmMain.BrowseForFolder(Owner: HWND; var Directory: String; StartDir, Title: String): Boolean;
 var
  BrowseInfo: TBrowseInfo;
  DisplayName: Array[0..MAX_PATH] Of Char;
  TempPath : Array[0..MAX_PATH] Of Char;
begin
  Result:= False;
  FillChar(BrowseInfo, SizeOf(TBrowseInfo), #0);
  With BrowseInfo Do Begin
    hwndOwner:= Owner;
    pidlRoot:= nil;
    pszDisplayName:= @DisplayName;
    lpszTitle:= PChar(Title);
    ulFlags:= BIF_RETURNONLYFSDIRS;
    lParam:= Integer(PChar(StartDir));
    lpfn:= BrowseCallbackProc;
  End;
  If SHGetPathFromIDList(SHBrowseForFolder(BrowseInfo), TempPath) Then Begin
    Directory:= TempPath;
    Result:= True;
  End;
end;

procedure TfmMain.miSampClick(Sender: TObject);
begin
  ShellExecute(Handle, 'open', 'http://www.sa-mp.com/', nil, nil, SW_SHOWNORMAL);
end;

procedure TfmMain.tmrQueryQueueProcessTimer(Sender: TObject);
begin
  //Application.ProcessMessages;
  if QueryQueue.Count > 0 then begin
    QueryServerInfo(QueryQueue.Strings[0], false, true, false, false);
    //QueryServerInfo(QueryQueue.Strings[0], true, false, false, false);
    //UpdateServers;
    QueryQueue.Delete(0);
  end;
end;

procedure TfmMain.label_urlClick(Sender: TObject);
begin
  ShellExecute(0, 'open', PAnsiChar('http://' + (Sender as TLabel).Caption), '', '', SW_SHOWNORMAL);
end;

procedure TfmMain.CreateFASTDesktoplink1Click(Sender: TObject);
var
  Idx: Integer;
begin
  if lbServers.ItemIndex = -1 then Exit;

  Idx:= StrToInt(lbServers.Items.Strings[lbServers.ItemIndex]);
  if Idx >= Length(Servers) then Exit;

  sub_4E1E6C(Servers[Idx].Address + ':' + IntToStr(Servers[Idx].Port), Servers[Idx].HostName);
end;

procedure TfmMain.FormResize(Sender: TObject);
begin
  imLogo.Left:= fmMain.Width - imLogo.Width;
  imLogo.Repaint;
end;

procedure TfmMain.imLogoClick(Sender: TObject);
begin
  fmAbout.ShowModal;
end;

end.

