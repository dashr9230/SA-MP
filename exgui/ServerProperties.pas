unit ServerProperties;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, StdCtrls;

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

{$R *.dfm}

procedure TfmServerProperties.bnSaveClick(Sender: TObject);
begin
  // TODO: TfmServerProperties.bnSaveClick
end;

procedure TfmServerProperties.bnCancelClick(Sender: TObject);
begin
  // TODO: TfmServerProperties.bnCancelClick
end;

procedure TfmServerProperties.bnConnectClick(Sender: TObject);
begin
  // TODO: TfmServerProperties.bnConnectClick
end;

procedure TfmServerProperties.pmCopyPopup(Sender: TObject);
begin
  // TODO: TfmServerProperties.pmCopyPopup
end;

procedure TfmServerProperties.piCopyClick(Sender: TObject);
begin
  // TODO: TfmServerProperties.piCopyClick
end;

end.
