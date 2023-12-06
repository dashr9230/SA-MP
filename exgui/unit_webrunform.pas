unit unit_webrunform;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Buttons;

type
  Twnd_webrunform = class(TForm)
    BitBtn1: TBitBtn;
    BitBtn2: TBitBtn;
    BitBtn3: TBitBtn;
    Label1: TLabel;

    procedure BitBtn2Click(Sender: TObject);
    procedure BitBtn3Click(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  wnd_webrunform: Twnd_webrunform;

implementation

{$R *.dfm}

procedure Twnd_webrunform.BitBtn2Click(Sender: TObject);
begin
  ModalResult:= mrCancel;
end;

procedure Twnd_webrunform.BitBtn3Click(Sender: TObject);
begin
  ModalResult:= mrOk;
end;

procedure Twnd_webrunform.BitBtn1Click(Sender: TObject);
begin
  ModalResult:= mrYes;
end;

end.
 