unit RconConfig;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TfmRconConfig = class(TForm)
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

{$R *.dfm}

procedure TfmRconConfig.edHostKeyPress(Sender: TObject; var Key: Char);
begin
  // TODO: TfmRconConfig.edHostKeyPress
end;

procedure TfmRconConfig.bnCancelClick(Sender: TObject);
begin
  // TODO: TfmRconConfig.bnCancelClick
end;

procedure TfmRconConfig.bnConnectClick(Sender: TObject);
begin
  // TODO: TfmRconConfig.bnConnectClick
end;

procedure TfmRconConfig.edPasswordChange(Sender: TObject);
begin
  // TODO: TfmRconConfig.edPasswordChange
end;

end.
