unit ImportFavorites;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TfmImportFavorites = class(TForm)
    rbAddToCurrent: TRadioButton;
    rbReplaceCurrent: TRadioButton;
    bnOk: TButton;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmImportFavorites: TfmImportFavorites;

implementation

{$R *.dfm}

end.
