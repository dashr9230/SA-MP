program samp;

uses
  Forms,
  Main in 'Main.pas' {Form1},
  About in 'About.pas' {Form2},
  ExportFavorites in 'ExportFavorites.pas' {Form3},
  ImportFavorites in 'ImportFavorites.pas' {Form4},
  MasterUpdate in 'MasterUpdate.pas' {Form5},
  RconConfig in 'RconConfig.pas' {Form6},
  ServerProperties in 'ServerProperties.pas' {Form7},
  Settings in 'Settings.pas' {Form8},
  wnd_webrunform in 'wnd_webrunform.pas' {Form9},
  Rcon in 'Rcon.pas' {Form10};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TForm2, Form2);
  Application.CreateForm(TForm3, Form3);
  Application.CreateForm(TForm4, Form4);
  Application.CreateForm(TForm5, Form5);
  Application.CreateForm(TForm6, Form6);
  Application.CreateForm(TForm7, Form7);
  Application.CreateForm(TForm8, Form8);
  Application.CreateForm(TForm9, Form9);
  Application.Run;
end.
