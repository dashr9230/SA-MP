unit Settings;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Registry, Buttons;

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

uses Main;

{$R *.dfm}

procedure TfmSettings.bnSaveClick(Sender: TObject);
var
  Reg: TRegistry;
begin
  Reg:= TRegistry.Create;
  Reg.RootKey:= HKEY_CURRENT_USER;
  Reg.OpenKey('SOFTWARE\SAMP', true);
  Reg.WriteBool('SaveServPasses', cbSaveServerPasswords.Checked);
  Reg.WriteBool('SaveRconPasses', cbSaveRconPasswords.Checked);
  Reg.CloseKey;
  Reg.Free;
  Close;
end;

procedure TfmSettings.bnCancelClick(Sender: TObject);
begin
  Close;
end;

procedure TfmSettings.FormCreate(Sender: TObject);
var
  Reg: TRegistry;
begin
  Reg:= TRegistry.Create;
  Reg.RootKey:= HKEY_CURRENT_USER;
  Reg.OpenKey('SOFTWARE\SAMP', true);
  if Reg.ValueExists('SaveServPasses') then
    cbSaveServerPasswords.Checked:= Reg.ReadBool('SaveServPasses');
  if Reg.ValueExists('SaveRconPasses') then
    cbSaveRconPasswords.Checked:= Reg.ReadBool('SaveRconPasses');
  Reg.CloseKey;
  Reg.Free;

  edInstallLoc.Text:= ExtractFilePath(gta_sa_exe);
end;

procedure TfmSettings.sbBrowseClick(Sender: TObject);
begin
  fmMain.GetGTAExe(Handle);
  edInstallLoc.Text:= ExtractFilePath(gta_sa_exe);
end;

end.
