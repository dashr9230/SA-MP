unit MasterUpdate;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TfmMasterUpdate = class(TForm)
    lblPleaseWait: TLabel;
    lblUpdating: TLabel;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmMasterUpdate: TfmMasterUpdate;

implementation

{$R *.dfm}

end.
