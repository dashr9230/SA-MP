unit Settings;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Buttons;

type
  TfmSettings = class(TForm)
    bnSave: TButton;
    bnCancel: TButton;
    gbPasswords: TGroupBox;
    cbSaveServerPasswords: TCheckBox;
    cbSaveRconPasswords: TCheckBox;
    edInstallLoc: TEdit;
    Label1: TLabel;
    sbBrowse: TSpeedButton;
    procedure bnSaveClick(Sender: TObject);
    procedure bnCancelClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure sbBrowseClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmSettings: TfmSettings;

implementation

{$R *.dfm}

procedure TfmSettings.bnSaveClick(Sender: TObject);
begin
  // TODO: TfmSettings.bnSaveClick
end;

procedure TfmSettings.bnCancelClick(Sender: TObject);
begin
  // TODO: TfmSettings.bnCancelClick
end;

procedure TfmSettings.FormCreate(Sender: TObject);
begin
  // TODO: TfmSettings.FormCreate
end;

procedure TfmSettings.sbBrowseClick(Sender: TObject);
begin
  // TODO: TfmSettings.sbBrowseClick
end;

end.
