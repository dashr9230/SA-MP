object fmServerProperties: TfmServerProperties
  Left = 638
  Top = 304
  BorderStyle = bsDialog
  Caption = 'Server Properties - ( Just click "Connect"! )'
  ClientHeight = 227
  ClientWidth = 278
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
  object lbAddressLab: TLabel
    Left = 16
    Top = 40
    Width = 45
    Height = 14
    Caption = 'Address:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object lbPlayersLab: TLabel
    Left = 16
    Top = 56
    Width = 39
    Height = 14
    Caption = 'Players:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object lbPingLab: TLabel
    Left = 16
    Top = 72
    Width = 23
    Height = 14
    Caption = 'Ping:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object lbModeLab: TLabel
    Left = 16
    Top = 88
    Width = 29
    Height = 14
    Caption = 'Mode:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object lbMapLab: TLabel
    Left = 16
    Top = 104
    Width = 51
    Height = 14
    Caption = 'Language:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object lbMap: TLabel
    Left = 80
    Top = 104
    Width = 185
    Height = 13
    AutoSize = False
    Caption = '- - -'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lbMode: TLabel
    Left = 80
    Top = 88
    Width = 185
    Height = 13
    AutoSize = False
    Caption = '- - -'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lbPing: TLabel
    Left = 80
    Top = 71
    Width = 185
    Height = 13
    AutoSize = False
    Caption = '- - -'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lbPlayers: TLabel
    Left = 80
    Top = 56
    Width = 185
    Height = 13
    AutoSize = False
    Caption = '- - -'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lbHostName: TLabel
    Left = 16
    Top = 8
    Width = 251
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = '- - -'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lbServerPassword: TLabel
    Left = 16
    Top = 136
    Width = 89
    Height = 14
    Caption = 'Server Password:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object lbRconPassword: TLabel
    Left = 16
    Top = 160
    Width = 85
    Height = 14
    Caption = 'RCON Password:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object edAddress: TEdit
    Left = 80
    Top = 40
    Width = 185
    Height = 13
    TabStop = False
    BorderStyle = bsNone
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ParentShowHint = False
    PopupMenu = pmCopy
    ReadOnly = True
    ShowHint = False
    TabOrder = 5
    Text = '- - -'
  end
  object edServerPassword: TEdit
    Left = 128
    Top = 135
    Width = 139
    Height = 16
    BevelInner = bvNone
    BevelKind = bkFlat
    BorderStyle = bsNone
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 3
  end
  object edRconPassword: TEdit
    Left = 128
    Top = 159
    Width = 139
    Height = 16
    BevelInner = bvNone
    BevelKind = bkFlat
    BorderStyle = bsNone
    Ctl3D = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 4
  end
  object bnSave: TButton
    Left = 112
    Top = 192
    Width = 75
    Height = 25
    Caption = 'Save'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = bnSaveClick
  end
  object bnCancel: TButton
    Left = 192
    Top = 192
    Width = 75
    Height = 25
    Caption = 'Cancel'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = bnCancelClick
  end
  object bnConnect: TButton
    Left = 16
    Top = 192
    Width = 75
    Height = 25
    Caption = 'Connect'
    Default = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = bnConnectClick
  end
  object pmCopy: TPopupMenu
    OnPopup = pmCopyPopup
    Left = 8
    Top = 8
    object piCopy: TMenuItem
      Caption = 'Copy'
      OnClick = piCopyClick
    end
  end
end
