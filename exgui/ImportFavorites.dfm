object fmImportFavorites: TfmImportFavorites
  Left = 194
  Top = 108
  BorderStyle = bsDialog
  Caption = 'Import Favorites'
  ClientHeight = 108
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
  object rbAddToCurrent: TRadioButton
    Left = 16
    Top = 16
    Width = 169
    Height = 17
    Caption = 'Add to current favorites'
    Checked = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    TabStop = True
  end
  object rbReplaceCurrent: TRadioButton
    Left = 16
    Top = 40
    Width = 169
    Height = 17
    Caption = 'Replace current favorites'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object bnOk: TButton
    Left = 64
    Top = 72
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
    TabOrder = 2
  end
end
