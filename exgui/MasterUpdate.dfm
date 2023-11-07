object fmMasterUpdate: TfmMasterUpdate
  Left = 489
  Top = 332
  BorderStyle = bsNone
  Caption = 'Master Server Update'
  ClientHeight = 103
  ClientWidth = 300
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object lblPleaseWait: TLabel
    Left = 0
    Top = 24
    Width = 300
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'Please Wait'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lblUpdating: TLabel
    Left = 0
    Top = 56
    Width = 300
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = 'Retrieving list from master server...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
end
