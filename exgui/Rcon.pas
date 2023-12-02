unit Rcon;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TfmRcon = class(TForm)
    edInput: TEdit;
    moOutput: TMemo;
    procedure edInputKeyPress(Sender: TObject; var Key: Char);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmRcon: TfmRcon;

implementation

{$R *.dfm}

procedure TfmRcon.edInputKeyPress(Sender: TObject; var Key: Char);
begin
  // TODO: TfmRcon.edInputKeyPress
end;

procedure TfmRcon.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  // TODO: TfmRcon.FormClose
end;

procedure TfmRcon.FormShow(Sender: TObject);
begin
  // TODO: TfmRcon.FormShow
end;

end.
