program samp;



uses
  Windows,
  Forms,
  Main in 'Main.pas' {fmMain},
  About in 'About.pas' {fmAbout},
  RconConfig in 'RconConfig.pas' {fmRconConfig},
  Settings in 'Settings.pas' {fmSettings},
  ServerProperties in 'ServerProperties.pas' {fmServerProperties},
  ImportFavorites in 'ImportFavorites.pas' {fmImportFavorites},
  ExportFavorites in 'ExportFavorites.pas' {fmExportFavorites},
  MasterUpdate in 'MasterUpdate.pas' {fmMasterUpdate},
  unit_webrunform in 'unit_webrunform.pas' {wnd_webrunform};

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'SA-MP 0.3';
  Application.CreateForm(TfmMain, fmMain);
  Application.CreateForm(TfmAbout, fmAbout);
  Application.CreateForm(TfmMasterUpdate, fmMasterUpdate);
  Application.CreateForm(Twnd_webrunform, wnd_webrunform);
  Application.Run;
end.
