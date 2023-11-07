object fmRcon: TfmRcon
  Left = 279
  Top = 107
  BorderStyle = bsDialog
  Caption = 'Remote Console'
  ClientHeight = 387
  ClientWidth = 844
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object edInput: TEdit
    Left = 0
    Top = 280
    Width = 649
    Height = 18
    BevelInner = bvNone
    BevelKind = bkFlat
    BorderStyle = bsNone
    Font.Charset = ANSI_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'edInput'
    OnKeyPress = edInputKeyPress
  end
  object moOutput: TMemo
    Left = 0
    Top = 0
    Width = 649
    Height = 281
    TabStop = False
    BevelInner = bvNone
    BevelKind = bkFlat
    BorderStyle = bsNone
    Color = clBlack
    Font.Charset = ANSI_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'SA:MP Remote Console Utility.'
      '(C) Copyright 2006 SA:MP Team.')
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
  end
end
