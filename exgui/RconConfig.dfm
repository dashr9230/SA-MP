object fmRconConfig: TfmRconConfig
  Left = 303
  Top = 533
  BorderStyle = bsDialog
  Caption = 'Remote Console Config'
  ClientHeight = 107
  ClientWidth = 242
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
  object lbHost: TLabel
    Left = 8
    Top = 16
    Width = 62
    Height = 13
    Caption = 'Host:Port -'
  end
  object lbPassword: TLabel
    Left = 8
    Top = 48
    Width = 63
    Height = 13
    Caption = 'Password -'
  end
  object edHost: TEdit
    Left = 80
    Top = 11
    Width = 153
    Height = 21
    TabOrder = 0
    OnChange = edPasswordChange
    OnKeyPress = edHostKeyPress
  end
  object edPassword: TEdit
    Left = 80
    Top = 43
    Width = 153
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
    OnChange = edPasswordChange
  end
  object bnConnect: TButton
    Left = 80
    Top = 72
    Width = 75
    Height = 25
    Caption = 'Connect'
    Enabled = False
    TabOrder = 2
    OnClick = bnConnectClick
  end
  object bnCancel: TButton
    Left = 160
    Top = 72
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = bnCancelClick
  end
end
