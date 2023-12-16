unit Rcon;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TfmRcon = class(TForm)
    edInput: TEdit;
    moOutput: TMemo;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure edInputKeyPress(Sender: TObject; var Key: Char);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    Host: String;
    Password: String;
  protected
    procedure CreateParams(var Params: TCreateParams); override;
  end;

var
  fmRcon: TfmRcon;

implementation

{$R *.dfm}

procedure Output(szOutput: PChar); stdcall;
begin
  fmRcon.moOutput.Lines.Add(szOutput);
end;

procedure TfmRcon.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
  Params.ExStyle:= Params.ExStyle or WS_EX_APPWINDOW;
  Params.WndParent:= GetDesktopWindow; 
end;

procedure TfmRcon.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  Destroy;
end;

procedure TfmRcon.edInputKeyPress(Sender: TObject; var Key: Char);
begin
  if Key = #13 then begin
    if edInput.Text <> '' then begin
      moOutput.Lines.Add('> ' + edInput.Text);
      edInput.Text:= '';
    end;
    Key:= #0;
  end;
end;

procedure TfmRcon.FormShow(Sender: TObject);
begin
  Caption:= 'Remote Console - ' + Host;
end;

end.
