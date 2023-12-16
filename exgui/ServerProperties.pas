unit ServerProperties;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Menus;

type
  TfmServerProperties = class(TForm)
    lbAddressLab: TLabel;
    lbPlayersLab: TLabel;
    lbPingLab: TLabel;
    lbModeLab: TLabel;
    lbMapLab: TLabel;
    lbMap: TLabel;
    lbMode: TLabel;
    lbPing: TLabel;
    lbPlayers: TLabel;
    edAddress: TEdit;
    lbHostName: TLabel;
    lbServerPassword: TLabel;
    lbRconPassword: TLabel;
    edServerPassword: TEdit;
    edRconPassword: TEdit;
    bnSave: TButton;
    bnCancel: TButton;
    bnConnect: TButton;
    pmCopy: TPopupMenu;
    piCopy: TMenuItem;
    procedure bnSaveClick(Sender: TObject);
    procedure bnCancelClick(Sender: TObject);
    procedure bnConnectClick(Sender: TObject);
    procedure pmCopyPopup(Sender: TObject);
    procedure piCopyClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmServerProperties: TfmServerProperties;

implementation

uses Main;

{$R *.dfm}

procedure TfmServerProperties.bnSaveClick(Sender: TObject);
var
  Idx: Integer;
begin
  Idx:= StrToInt(fmMain.lbServers.Items.Strings[fmMain.lbServers.ItemIndex]);
  Servers[Idx].ServerPassword:= edServerPassword.Text;
  Servers[Idx].RconPassword:= edRconPassword.Text;
  Close;
end;

procedure TfmServerProperties.bnCancelClick(Sender: TObject);
begin
  Close;
end;

procedure TfmServerProperties.bnConnectClick(Sender: TObject);
var
  Idx: Integer;
begin
  Idx:= StrToInt(fmMain.lbServers.Items.Strings[fmMain.lbServers.ItemIndex]);
  Servers[Idx].ServerPassword:= edServerPassword.Text;
  Servers[Idx].RconPassword:= edRconPassword.Text;
  fmMain.ConnectClick(fmMain);
  Close;
end;

procedure TfmServerProperties.pmCopyPopup(Sender: TObject);
begin
  piCopy.Enabled:= edAddress.Text <> '- - -';
end;

procedure TfmServerProperties.piCopyClick(Sender: TObject);
begin
  fmMain.SetClipBoardStr(edAddress.Text);
end;

end.
