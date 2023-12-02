unit ServerProperties;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, StdCtrls;

type
  TfmServerProperties = class(TForm)
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
