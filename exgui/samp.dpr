program samp;

uses
  Forms,
  Main in 'Main.pas' {fmMain},
  About in 'About.pas' {fmAbout},
  ExportFavorites in 'ExportFavorites.pas' {fmExportFavorites},
  ImportFavorites in 'ImportFavorites.pas' {fmImportFavorites},
  MasterUpdate in 'MasterUpdate.pas' {fmMasterUpdate},
  RconConfig in 'RconConfig.pas' {fmRconConfig},
  ServerProperties in 'ServerProperties.pas' {fmServerProperties},
  Settings in 'Settings.pas' {fmSettings},
  wnd_webrunform in 'wnd_webrunform.pas' {wnd_webrunform},
  Rcon in 'Rcon.pas' {fmRcon};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfmMain, fmMain);
  Application.CreateForm(TfmAbout, fmAbout);
  Application.CreateForm(TfmExportFavorites, fmExportFavorites);
  Application.CreateForm(TfmImportFavorites, fmImportFavorites);
  Application.CreateForm(TfmMasterUpdate, fmMasterUpdate);
  Application.CreateForm(TfmRconConfig, fmRconConfig);
  Application.CreateForm(TfmServerProperties, fmServerProperties);
  Application.CreateForm(TfmSettings, fmSettings);
  Application.CreateForm(TfmRcon, fmRcon);
  Application.CreateForm(Twnd_webrunform, wnd_webrunform);
  Application.Run;
end.
