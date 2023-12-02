unit About;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs;

type
  TfmAbout = class(TForm)
    procedure FormShow(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  fmAbout: TfmAbout;

implementation

{$R *.dfm}

procedure TfmAbout.FormShow(Sender: TObject);
begin
  // TODO: TfmAbout.FormShow
end;

procedure TfmAbout.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  // TODO: TfmAbout.FormClose
end;

procedure TfmAbout.FormCreate(Sender: TObject);
begin
  // TODO: TfmAbout.FormCreate
end;

procedure TfmAbout.FormDestroy(Sender: TObject);
begin
  // TODO: TfmAbout.FormDestroy
end;

procedure TfmAbout.FormClick(Sender: TObject);
begin
  // TODO: TfmAbout.FormClick
end;

end.
