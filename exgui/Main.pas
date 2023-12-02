unit Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, XPMan, ImgList, Menus, ComCtrls, StdCtrls, TeEngine,
  Series, TeeProcs, Chart, Tabs, ToolWin;

type
  TfmMain = class(TForm)
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
    procedure tmrServerListUpdateTimer(Sender: TObject);
    procedure tmSIPingUpdateTimer(Sender: TObject);
    procedure lbServersClick(Sender: TObject);
    procedure FilterChange(Sender: TObject);
    procedure UpdateServers;
    procedure lbServersContextPopup(Sender: TObject; MousePos: TPoint;
      var Handled: Boolean);
    procedure gbInfoContextPopup(Sender: TObject; MousePos: TPoint;
      var Handled: Boolean);
    procedure ImportFavoritesClick(Sender: TObject);
    procedure ImportFavorites;
    procedure ExportFavoritesClick(Sender: TObject);
    procedure ExportFavorites;
    procedure ExitClick(Sender: TObject);
    procedure miViewClick(Sender: TObject);
    procedure ToggleFilterServerInfo(Sender: TObject);
    procedure ToggleStatusBar(Sender: TObject);
    procedure ConnectClick(Sender: TObject);
    procedure AddServerClick(Sender: TObject);
    procedure AddServer;
    procedure DeleteServerClick(Sender: TObject);
    procedure RefreshServerClick(Sender: TObject);
    procedure MasterServerUpdateClick(Sender: TObject);
    procedure CopyServerInfoClick(Sender: TObject);
    procedure ServerPropertiesClick(Sender: TObject);
    procedure RemoteConsoleClick(Sender: TObject);
    procedure SettingsClick(Sender: TObject);
    procedure HelpTopicsClick(Sender: TObject);
    procedure AboutClick(Sender: TObject);
    procedure tsServerListsChange(Sender: TObject; NewTab: Integer;
      var AllowChange: Boolean);
    procedure QueryServerInfoParse;
    procedure QueryServerInfoError;
    procedure QueryServerInfo;
    procedure ServerConnect;
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
    procedure WMRecv;
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure BrowseForFolder;
    procedure miSampClick(Sender: TObject);
    procedure tmrQueryQueueProcessTimer(Sender: TObject);
    procedure label_urlClick(Sender: TObject);
    procedure CreateFASTDesktoplink1Click;
    procedure FormResize(Sender: TObject);
    procedure imLogoClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmMain: TfmMain;

implementation

{$R *.dfm}

procedure TfmMain.FormCreate(Sender: TObject);
begin
  // TODO: TfmMain.FormCreate
end;

procedure TfmMain.lbServersDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
begin
  // TODO: TfmMain.lbServersDrawItem
end;

procedure TfmMain.hcServersSectionResize(HeaderControl: THeaderControl;
  Section: THeaderSection);
begin
  // TODO: TfmMain.hcServersSectionResize
end;

procedure TfmMain.tbMainResize(Sender: TObject);
begin
  // TODO: TfmMain.tbMainResize
end;

procedure TfmMain.hcServersSectionClick(HeaderControl: THeaderControl;
  Section: THeaderSection);
begin
  // TODO: TfmMain.hcServersSectionClick
end;

procedure TfmMain.pnBreakableResize(Sender: TObject);
begin
  // TODO: TfmMain.pnBreakableResize
end;

procedure TfmMain.hcServersDrawSection(HeaderControl: THeaderControl;
  Section: THeaderSection; const Rect: TRect; Pressed: Boolean);
begin
  // TODO: TfmMain.hcServersDrawSection
end;

procedure TfmMain.tmrServerListUpdateTimer(Sender: TObject);
begin
  // TODO: TfmMain.tmrServerListUpdateTimer
end;

procedure TfmMain.tmSIPingUpdateTimer(Sender: TObject);
begin
  // TODO: TfmMain.tmSIPingUpdateTimer
end;

procedure TfmMain.lbServersClick(Sender: TObject);
begin
  // TODO: TfmMain.lbServersClick
end;

procedure TfmMain.FilterChange(Sender: TObject);
begin
  // TODO: TfmMain.FilterChange
end;

procedure TfmMain.UpdateServers;
begin
  // TODO: TfmMain.UpdateServers
end;

procedure TfmMain.lbServersContextPopup(Sender: TObject; MousePos: TPoint;
  var Handled: Boolean);
begin
  // TODO: TfmMain.lbServersContextPopup
end;

procedure TfmMain.gbInfoContextPopup(Sender: TObject; MousePos: TPoint;
  var Handled: Boolean);
begin
  // TODO: TfmMain.gbInfoContextPopup
end;

procedure TfmMain.ImportFavoritesClick(Sender: TObject);
begin
  // TODO: TfmMain.ImportFavoritesClick
end;

procedure TfmMain.ImportFavorites;
begin
  // TODO: TfmMain.ImportFavorites
end;




procedure TfmMain.ExportFavoritesClick(Sender: TObject);
begin
  // TODO: TfmMain.ExportFavoritesClick
end;

procedure TfmMain.ExportFavorites;
begin
  // TODO: TfmMain.ExportFavorites
end;

procedure TfmMain.ExitClick(Sender: TObject);
begin
  // TODO: TfmMain.ExitClick
end;

procedure TfmMain.miViewClick(Sender: TObject);
begin
  // TODO: TfmMain.miViewClick
end;

procedure TfmMain.ToggleFilterServerInfo(Sender: TObject);
begin
  // TODO: TfmMain.ToggleFilterServerInfo
end;

procedure TfmMain.ToggleStatusBar(Sender: TObject);
begin
  // TODO: TfmMain.ToggleStatusBar
end;

procedure TfmMain.ConnectClick(Sender: TObject);
begin
  // TODO: TfmMain.ConnectClick
end;

procedure TfmMain.AddServerClick(Sender: TObject);
begin
  // TODO: TfmMain.AddServerClick
end;

procedure TfmMain.AddServer;
begin
  // TODO: TfmMain.AddServer
end;

procedure TfmMain.DeleteServerClick(Sender: TObject);
begin
  // TODO: TfmMain.DeleteServerClick
end;

procedure TfmMain.RefreshServerClick(Sender: TObject);
begin
  // TODO: TfmMain.RefreshServerClick
end;

procedure TfmMain.MasterServerUpdateClick(Sender: TObject);
begin
  // TODO: TfmMain.MasterServerUpdateClick
end;

procedure TfmMain.CopyServerInfoClick(Sender: TObject);
begin
  // TODO: TfmMain.CopyServerInfoClick
end;

procedure TfmMain.ServerPropertiesClick(Sender: TObject);
begin
  // TODO: TfmMain.ServerPropertiesClick
end;

procedure TfmMain.RemoteConsoleClick(Sender: TObject);
begin
  // TODO: TfmMain.RemoteConsoleClick
end;

procedure TfmMain.SettingsClick(Sender: TObject);
begin
  // TODO: TfmMain.SettingsClick
end;

procedure TfmMain.HelpTopicsClick(Sender: TObject);
begin
  // TODO: TfmMain.HelpTopicsClick
end;

procedure TfmMain.AboutClick(Sender: TObject);
begin
  // TODO: TfmMain.AboutClick
end;

procedure TfmMain.tsServerListsChange(Sender: TObject; NewTab: Integer;
  var AllowChange: Boolean);
begin
  // TODO: TfmMain.tsServerListsChange
end;

procedure TfmMain.QueryServerInfoParse;
begin
  // TODO: TfmMain.QueryServerInfoParse
end;

procedure TfmMain.QueryServerInfoError;
begin
  // TODO: TfmMain.QueryServerInfoError
end;

procedure TfmMain.QueryServerInfo;
begin
  // TODO: TfmMain.QueryServerInfo
end;

procedure TfmMain.ServerConnect;
begin
  // TODO: TfmMain.ServerConnect
end;

procedure TfmMain.piCopyClick(Sender: TObject);
begin
  // TODO: TfmMain.piCopyClick
end;

procedure TfmMain.pmCopyPopup(Sender: TObject);
begin
  // TODO: TfmMain.pmCopyPopup
end;

procedure TfmMain.lbPlayersDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
begin
  // TODO: TfmMain.lbPlayersDrawItem
end;

procedure TfmMain.hcPlayersSectionResize(HeaderControl: THeaderControl;
  Section: THeaderSection);
begin
  // TODO: TfmMain.hcPlayersSectionResize
end;

procedure TfmMain.lbRulesDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
begin
  // TODO: TfmMain.lbRulesDrawItem
end;

procedure TfmMain.hcRulesSectionResize(HeaderControl: THeaderControl;
  Section: THeaderSection);
begin
  // TODO: TfmMain.hcRulesSectionResize
end;

procedure TfmMain.sbMainDrawPanel(StatusBar: TStatusBar;
  Panel: TStatusPanel; const Rect: TRect);
begin
  // TODO: TfmMain.sbMainDrawPanel
end;

procedure TfmMain.lbPlayersExit(Sender: TObject);
begin
  // TODO: TfmMain.lbPlayersExit
end;

procedure TfmMain.lbRulesExit(Sender: TObject);
begin
  // TODO: TfmMain.lbRulesExit
end;

procedure TfmMain.WMRecv;
begin
  // TODO: TfmMain.WMRecv
end;

procedure TfmMain.FormDestroy(Sender: TObject);
begin
  // TODO: TfmMain.FormDestroy
end;

procedure TfmMain.FormShow(Sender: TObject);
begin
  // TODO: TfmMain.FormShow
end;

procedure TfmMain.BrowseForFolder;
begin
  // TODO: TfmMain.BrowseForFolder
end;

procedure TfmMain.miSampClick(Sender: TObject);
begin
  // TODO: TfmMain.miSampClick
end;

procedure TfmMain.tmrQueryQueueProcessTimer(Sender: TObject);
begin
  // TODO: TfmMain.tmrQueryQueueProcessTimer
end;

procedure TfmMain.label_urlClick(Sender: TObject);
begin
  // TODO: TfmMain.label_urlClick
end;

procedure TfmMain.CreateFASTDesktoplink1Click;
begin
  // TODO: TfmMain.CreateFASTDesktoplink1Click
end;

procedure TfmMain.FormResize(Sender: TObject);
begin
  // TODO: TfmMain.FormResize
end;

procedure TfmMain.imLogoClick(Sender: TObject);
begin
  // TODO: TfmMain.imLogoClick
end;

end.
