unit ExportFavorites;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TfmExportFavorites = class(TForm)
    cbIncludeSavedPasswords: TCheckBox;
    bnOk: TButton;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmExportFavorites: TfmExportFavorites;

implementation

{$R *.dfm}

end.
