object fmExportFavorites: TfmExportFavorites
  Left = 410
  Top = 136
  BorderStyle = bsDialog
  Caption = 'Export Favorites'
  ClientHeight = 83
  ClientWidth = 204
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object cbIncludeSavedPasswords: TCheckBox
    Left = 16
    Top = 16
    Width = 169
    Height = 17
    Caption = 'Include Saved Passwords'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object bnOk: TButton
    Left = 64
    Top = 48
    Width = 75
    Height = 25
    Caption = 'OK'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ModalResult = 1
    ParentFont = False
    TabOrder = 1
  end
end
