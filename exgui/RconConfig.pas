unit RconConfig;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ShellAPI;

type
  TfmRconConfig = class(TForm)
    edHost: TEdit;
    lbHost: TLabel;
    lbPassword: TLabel;
    edPassword: TEdit;
    bnConnect: TButton;
    bnCancel: TButton;
    procedure edHostKeyPress(Sender: TObject; var Key: Char);
    procedure bnCancelClick(Sender: TObject);
    procedure bnConnectClick(Sender: TObject);
    procedure edPasswordChange(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmRconConfig: TfmRconConfig;

implementation

uses
  Rcon;

{$R *.dfm}

procedure TfmRconConfig.edHostKeyPress(Sender: TObject; var Key: Char);
begin
  if not (Key in ['0'..'9','a'..'z','A'..'Z','.',':'])
    and (Key <> #8) and (Key <> #46) then
    Key:= #0;
end;

procedure TfmRconConfig.bnCancelClick(Sender: TObject);
begin
  Close;
end;

procedure TfmRconConfig.bnConnectClick(Sender: TObject);
 var
  //fmRcon: TfmRcon;
  Server, Addr, Port: String;
begin
  //fmRcon:= TfmRcon.Create(Application);
  //fmRcon.Host:= edHost.Text;
  //fmRcon.Password:= edPassword.Text;
  //fmRcon.Show;
  Server:= edHost.Text;
  if Pos(':', Server) <> 0 then begin
    Addr:= Copy(Server, 1, Pos(':', Server)-1);
    Port:= Copy(Server, Pos(':', Server)+1, 5);
  end else begin
    Addr:= Server;
    Port:= '5193';
  end;
  ShellExecute(0, 'open', 'rcon.exe', PChar(Addr + ' ' + Port + ' ' + edPassword.text), PChar(ExtractFilePath(ParamStr(0))), SW_SHOWNORMAL);
  Close;
end;

procedure TfmRconConfig.edPasswordChange(Sender: TObject);
begin
  bnConnect.Enabled:= (edHost.Text <> '') and (edPassword.Text <> '');
end;

end.
