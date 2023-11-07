object fmMain: TfmMain
  Left = 327
  Top = 43
  Width = 839
  Height = 600
  Caption = 'San Andreas Multiplayer 0.3.7-R5'
  Color = clBtnFace
  Constraints.MinHeight = 420
  Constraints.MinWidth = 525
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clSilver
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = mmMain
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object spRight: TSplitter
    Left = 601
    Top = 28
    Width = 9
    Height = 380
    Align = alRight
    Beveled = True
    Color = clMenu
    ParentColor = False
    ResizeStyle = rsUpdate
  end
  object imPadlock: TImage
    Left = 477
    Top = 4
    Width = 13
    Height = 14
    AutoSize = True
    Picture.Data = {
      07544269746D617066020000424D660200000000000036000000280000000D00
      00000E000000010018000000000030020000120B0000120B0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000001DA8241DA72D20A737289E871899CC25A9D223
      9BCA1D978D1CA3381DA62C1DA824000000000000001DA82721A0844DB2D178CF
      EA71E1FF38A0B447DEFE3EC4EF2DA4D41C987F1DA7270000000000000022A18F
      51BFED9EE8FFB1EEFFA8E0F51F2F3481DDF56CE8FF4BE2FF44BBEA1C978A0000
      00000000004EAFB8A6EDFFA9E8FFA6E8FF97D5EC1F2B30A0D6ECABE9FF6BE2FF
      4AE0FF219AC00000000000000048AFC1B8FFFFA9F1FFABF1FF3C45490000003D
      4749AEF2FFABE7FF64E5FF1A9BC00000000000000048ABBA9EECFFBAFFFFB7FD
      FFABC3C75E6B71A7C0C7AEF3FFB8F1FF71E2FF1A98C00000000000000020A173
      49ADD76BC2D35AC0DD6EC9DA6BCFEC6EBDDF6AAADA65C0D145B1DC1C9C6E0000
      00000000001DA82821A54221A4441CA34722A63F1FA63825A15756AFDA51B1D2
      199C661DA824000000000000001DA8241FA24D319AC12393BD1CA2441DA8241D
      A3476DC3DA67BEE1189D501DA824000000000000001DA82425A54E6BC5DB5BC2
      EC149673159B5219987173CDE451B6E0189D501DA824000000000000001DA824
      1DA72E2CA49353C4EC6FCBF378C7E672CDF144BDF11C98921DA62D1DA8240000
      00000000001DA8241DA8241EA82731A58E57B4C851B4CE42ACD51C98901DA727
      1DA8241DA8240000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000}
    Visible = False
  end
  object imPadlocked: TImage
    Left = 490
    Top = 4
    Width = 13
    Height = 14
    AutoSize = True
    Picture.Data = {
      07544269746D617066020000424D660200000000000036000000280000000D00
      00000E000000010018000000000030020000120B0000120B0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000003E1DA83E1DA83D2EAA336EB31991C6229DD021
      94C52867B8382BAA3E1DA83E1DA8000000000000003E1DA8315AB845ABC86DC8
      E566E2FF3CB2C949E0FF3DBDEA259ACA2C53B23E1DA800000000000000315CB8
      42B5E596E6FFB1EDFFAEEAFF10131573E3FF5EE5FF4CE0FF3DB1E22C55B30000
      000000000048ABBF99EBFFACE8FFA3E5FFB1F9FF1C262ABDFAFFA1E5FF57E1FF
      4CDFFF2196C70000000000000046A9C6B9FFFFA7EDFFAEF3FF405B650508093F
      5B65B5F5FFA2E7FF5CE5FF1C94C5000000000000003AA8CFB4FDFFB1FAFFADF7
      FF85858318110E848584AAF3FFB3EAFF69E6FF1A97C6000000000000004494B5
      67CEF4A2EAF39CE6F4A0EAF692D9F09CDDF592D6FA9CE0F367CDF42083C00000
      00000000003E26A92C82BB42A7C23FA3BD2D7EB92E7BBB4780AD4491D240A7BE
      3083B73C25AA000000000000003E1DA83A39AB82CFE458B3DB3B23A93E1DA83D
      24A877C9D669BFE63435AC3E1DA8000000000000003E1DA83B44AB5BBDDD59C5
      F41F65B61D56AF1E65B861CDF74AB3DD2F3DAC3E1DA8000000000000003E1DA8
      3C26AA376EB651C1E77CD3F588D2EF80D4F441BAED2562B63B25A93E1DA80000
      00000000003E1DA83E1DA83E21A83A74B651ACBF46A9CE3BA3CE2667B73D20A9
      3E1DA83E1DA80000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000}
    Visible = False
  end
  object imDownArrow: TImage
    Left = 503
    Top = 5
    Width = 9
    Height = 12
    AutoSize = True
    Picture.Data = {
      07544269746D617086010000424D860100000000000036000000280000000900
      00000C000000010018000000000050010000120B0000120B0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00BF6C22BF6C22BF6C22BF6C22BF6C22BF6C22BF6C2200000000000000BF6C22
      BF6C22BF6C22FFFFFFBF6C22BF6C22BF6C2200000000000000BF6C22BF6C22FF
      FFFFFFFFFFFFFFFFBF6C22BF6C2200000000000000BF6C22FFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFBF6C2200000000000000BF6C22BF6C22BF6C22FFFFFFBF6C22
      BF6C22BF6C2200000000000000BF6C22BF6C22BF6C22FFFFFFBF6C22BF6C22BF
      6C2200000000000000BF6C22BF6C22BF6C22FFFFFFBF6C22BF6C22BF6C220000
      0000000000BF6C22BF6C22BF6C22FFFFFFBF6C22BF6C22BF6C22000000000000
      00BF6C22BF6C22BF6C22FFFFFFBF6C22BF6C22BF6C2200000000000000BF6C22
      BF6C22BF6C22BF6C22BF6C22BF6C22BF6C220000000000000000000000000000
      000000000000000000000000000000000000}
    Visible = False
  end
  object imUpArrow: TImage
    Left = 512
    Top = 5
    Width = 9
    Height = 12
    AutoSize = True
    Picture.Data = {
      07544269746D617086010000424D860100000000000036000000280000000900
      00000C000000010018000000000050010000120B0000120B0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00BF6C22BF6C22BF6C22BF6C22BF6C22BF6C22BF6C2200000000000000BF6C22
      BF6C22BF6C22FFFFFFBF6C22BF6C22BF6C2200000000000000BF6C22BF6C22BF
      6C22FFFFFFBF6C22BF6C22BF6C2200000000000000BF6C22BF6C22BF6C22FFFF
      FFBF6C22BF6C22BF6C2200000000000000BF6C22BF6C22BF6C22FFFFFFBF6C22
      BF6C22BF6C2200000000000000BF6C22BF6C22BF6C22FFFFFFBF6C22BF6C22BF
      6C2200000000000000BF6C22FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBF6C220000
      0000000000BF6C22BF6C22FFFFFFFFFFFFFFFFFFBF6C22BF6C22000000000000
      00BF6C22BF6C22BF6C22FFFFFFBF6C22BF6C22BF6C2200000000000000BF6C22
      BF6C22BF6C22BF6C22BF6C22BF6C22BF6C220000000000000000000000000000
      000000000000000000000000000000000000}
    Visible = False
  end
  object sbMain: TStatusBar
    Left = 0
    Top = 528
    Width = 831
    Height = 21
    Panels = <
      item
        Width = 500
      end>
    SimplePanel = True
    SimpleText = 'Client Loaded.'
    SizeGrip = False
    OnDrawPanel = sbMainDrawPanel
  end
  object tbMain: TToolBar
    Left = 0
    Top = 0
    Width = 831
    Height = 28
    ButtonHeight = 24
    Caption = 'tbMain'
    Color = clBtnFace
    Flat = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clSilver
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    Images = ilMain
    ParentColor = False
    ParentFont = False
    TabOrder = 1
    Transparent = False
    Wrapable = False
    OnResize = tbMainResize
    DesignSize = (
      831
      26)
    object tbConnect: TToolButton
      Left = 0
      Top = 0
      Hint = 'Connect'
      ImageIndex = 6
      ParentShowHint = False
      ShowHint = True
      OnClick = ConnectClick
    end
    object tbSpacer1: TToolButton
      Left = 23
      Top = 0
      Width = 8
      ImageIndex = 15
      Style = tbsSeparator
    end
    object tbAddServer: TToolButton
      Left = 31
      Top = 0
      Hint = 'Add Server'
      ImageIndex = 8
      ParentShowHint = False
      ShowHint = True
      OnClick = AddServerClick
    end
    object tbDeleteServer: TToolButton
      Left = 54
      Top = 0
      Hint = 'Delete Server'
      ImageIndex = 9
      ParentShowHint = False
      ShowHint = True
      OnClick = DeleteServerClick
    end
    object tbRefreshServer: TToolButton
      Left = 77
      Top = 0
      Hint = 'Refresh Server'
      ImageIndex = 7
      ParentShowHint = False
      ShowHint = True
      OnClick = RefreshServerClick
    end
    object tbSpacer2: TToolButton
      Left = 100
      Top = 0
      Width = 8
      ImageIndex = 15
      Style = tbsSeparator
    end
    object tbMasterServerUpdate: TToolButton
      Left = 108
      Top = 0
      Hint = 'Master Server Update'
      ImageIndex = 2
      ParentShowHint = False
      ShowHint = True
      OnClick = MasterServerUpdateClick
    end
    object tbSpacer3: TToolButton
      Left = 131
      Top = 0
      Width = 8
      ImageIndex = 15
      Style = tbsSeparator
    end
    object tbCopyServerInfo: TToolButton
      Left = 139
      Top = 0
      Hint = 'Copy Server Info'
      ImageIndex = 1
      ParentShowHint = False
      ShowHint = True
      OnClick = CopyServerInfoClick
    end
    object tbServerProperties: TToolButton
      Left = 162
      Top = 0
      Hint = 'Server Properties'
      ImageIndex = 10
      ParentShowHint = False
      ShowHint = True
      OnClick = ServerPropertiesClick
    end
    object tbSpacer4: TToolButton
      Left = 185
      Top = 0
      Width = 8
      ImageIndex = 15
      Style = tbsSeparator
    end
    object tbSettings: TToolButton
      Left = 193
      Top = 0
      Hint = 'Settings'
      ImageIndex = 0
      ParentShowHint = False
      ShowHint = True
      OnClick = SettingsClick
    end
    object tbSpacer5: TToolButton
      Left = 216
      Top = 0
      Width = 8
      ImageIndex = 15
      Style = tbsSeparator
    end
    object tbHelp: TToolButton
      Left = 224
      Top = 0
      Hint = 'Help'
      ImageIndex = 4
      ParentShowHint = False
      ShowHint = True
      OnClick = HelpTopicsClick
    end
    object tbAbout: TToolButton
      Left = 247
      Top = 0
      Hint = 'About'
      ImageIndex = 3
      ParentShowHint = False
      ShowHint = True
      OnClick = AboutClick
    end
    object lblPlayerName: TLabel
      Left = 270
      Top = 0
      Width = 50
      Height = 24
      Anchors = [akLeft]
      AutoSize = False
      Caption = '   Name: '
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clBtnText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
    end
    object edName: TEdit
      Left = 320
      Top = 0
      Width = 160
      Height = 24
      Anchors = [akLeft]
      AutoSize = False
      BevelInner = bvNone
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      MaxLength = 30
      ParentFont = False
      TabOrder = 0
    end
    object ToolButton1: TToolButton
      Left = 480
      Top = 0
      Width = 345
      ImageIndex = 4
      Style = tbsSeparator
    end
    object imLogo: TImage
      Left = 825
      Top = 0
      Width = 57
      Height = 26
      Cursor = crHandPoint
      Anchors = []
      AutoSize = True
      Center = True
      Picture.Data = {
        07544269746D61704E0A0000424D4E0A00000000000036040000280000003900
        00001A0000000100080000000000180600000000000000000000000100000001
        000000144000002C8F004458BB003E5DC900004EFF000072FF00445AC9004F68
        D4004F70D4004476EC004973E9004F76EC004F78EC00467BF100FF00FF005180
        E9004C80F7005180F7005183F7005480F4005188FC005188FF005488FC005488
        FF00BFD3FF000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000E050E0E0E050E0E0504050E0E0505050505050505050505050505050505
        0505050505050505050505050505050505050505050505050505050000000E0E
        050E0E05050E0E0504050E0E0504040404040404040404040404040404040404
        04040404040404040404040404040404040404040404050000000E0E0E050E0E
        05050E0E0504050E0E0504040404040401000000000001010001040100010404
        040100010100010100010100010404040404050000000E0E0E0E050E0E05050E
        0E0504050E0E0504040404040018181818180000180004001800010001001800
        0018000018000018000404040404050000000E0E0E0E0E050E0E05050E0E0504
        050E0E0504040404010000000018000018000000180000180000180000180000
        18000018000000000104050000000E0E0E0E0E0E050E0E05050E0E0504050E0E
        0504040400181818181800001818181818000100010018000018000018000018
        181818180004050000000E0E0E0E0E0E0E050E0E05050E0E0504050E0E050404
        0018000000000100180000001800001800001800001800001800001800000018
        0004050000000E0E0E0E0E0E0E0E050E0E05050E0E0504050E0E050400181818
        1818000018181818180001000100181818181818180000181818181800040500
        00000E0E0E0E0E0E0E0E0E050E0E05050E0E0504050E0E050100000000000101
        0000000000010404040100000000000000010100000000000104050000000E0E
        0E0E0E0E0E0E0E0E050E0E05050E0E0504050E0E050404040404040404040404
        04040404040404040404040404040404040404040404050000000E0E0E0E0E0E
        0E0E0E0E0E050E0E05050E0E0504050E0E050404040404040404040404040404
        04040404040404040B0A0C0A040404040404050000000E0E0E0E0E0E0E0E0E0E
        0E0E050E0E05050E0E0504050E0E050404040404040404040404040404040404
        0404040B100D09100A0404040404050000000E0E0E0E0E0E0E0E0E0E0E0E0E05
        0E0E05050E0E0504050E0E050404040404040404040404040404040404040B10
        15060309100A04040404050000000E0E0E0E0E0E0E0E0E0E0E0E0E0E050E0E05
        050E0E0504050E0E05040404040404040404040404040404040B101706060203
        09100A040404050000000E0E0E0E0E0E0E0E0E0E0E0E0E0E0E050E0E05050E0E
        0504050E0E0504040404040404040404040404040B101706031112130209100A
        0404050000000E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E050E0E05050E0E050405
        0E0E050404040404040404040404040B1017060306031103060209100A040500
        00000E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E050E0E05050E0E0504050E0E05
        04040404040404040404040C1706030606061206060302090A04050000000E0E
        0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E050E0E05050E0E0504050E0E05040404
        040404040404040C140F0703060311060306030D0A04050000000E0E0E0E0E0E
        0E0E0E0E0E0E0E0E0E0E0E0E0E050E0E05050E0E0504050E0E05040404040404
        0404040B10160F0706111203060610100A04050000000E0E0E0E0E0E0E0E0E0E
        0E0E0E0E0E0E0E0E0E0E050E0E05050E0E0504050E0E05040404040404040404
        0B10170F070308060615100A0404050000000E0E0E0E0E0E0E0E0E0E0E0E0E0E
        0E0E0E0E0E0E0E050E0E05050E0E0504050E0E050404040404040404040B1016
        0F07110317100A040404050000000E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E
        0E0E0E0E050E0E05050E0E0504050E0E050404040404040404040B10160F0615
        100A04040404050000000E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E
        0E050E0E05050E0E0504050E0E050404040404040404040B101417100A040404
        0404050000000E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E050E
        0E05050E0E0504050E0E050404040404040404040B0B0A0A0404040404040500
        00000E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E050E0E0505
        0E0E0504050E0E05040404040404040404040404040404040404050000000E0E
        0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E0E050E0E05050E0E05
        04050E0E05050505050505050505050505050505050505000000}
      Transparent = True
      OnClick = imLogoClick
    end
  end
  object tsServerLists: TTabSet
    Left = 0
    Top = 507
    Width = 831
    Height = 21
    Align = alBottom
    DitherBackground = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    StartMargin = 0
    SoftTop = True
    TabHeight = 25
    Tabs.Strings = (
      '    Favorites   '
      '    Internet    '
      '     Hosted    ')
    TabIndex = 0
    UnselectedColor = clBtnShadow
    OnChange = tsServerListsChange
  end
  object pnBreakable: TPanel
    Left = 0
    Top = 408
    Width = 831
    Height = 99
    Align = alBottom
    BevelOuter = bvNone
    Ctl3D = True
    Font.Charset = OEM_CHARSET
    Font.Color = clSilver
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    ParentShowHint = False
    ShowHint = False
    TabOrder = 3
    OnResize = pnBreakableResize
    object gbFilter: TGroupBox
      Left = 0
      Top = 0
      Width = 241
      Height = 99
      Align = alLeft
      Caption = ' Filter '
      Color = clBtnFace
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBtnText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 0
      OnDblClick = ToggleFilterServerInfo
      object edFilterMode: TLabeledEdit
        Left = 8
        Top = 32
        Width = 121
        Height = 15
        TabStop = False
        BevelInner = bvNone
        BevelKind = bkFlat
        BorderStyle = bsNone
        Ctl3D = True
        EditLabel.Width = 30
        EditLabel.Height = 13
        EditLabel.Caption = 'Mode:'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -11
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 0
        OnChange = FilterChange
        OnDblClick = ToggleFilterServerInfo
      end
      object edFilterMap: TLabeledEdit
        Left = 8
        Top = 72
        Width = 121
        Height = 15
        TabStop = False
        BevelInner = bvNone
        BevelKind = bkFlat
        BorderStyle = bsNone
        Ctl3D = True
        EditLabel.Width = 51
        EditLabel.Height = 13
        EditLabel.Caption = 'Language:'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -11
        EditLabel.Font.Name = 'MS Sans Serif'
        EditLabel.Font.Style = []
        EditLabel.ParentFont = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 1
        OnChange = FilterChange
        OnDblClick = ToggleFilterServerInfo
      end
      object cbFilterEmpty: TCheckBox
        Left = 136
        Top = 52
        Width = 97
        Height = 17
        TabStop = False
        Caption = 'Not Empty'
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 2
        OnClick = FilterChange
      end
      object cbFilterPassworded: TCheckBox
        Left = 136
        Top = 72
        Width = 97
        Height = 17
        TabStop = False
        Caption = 'No Password'
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 3
        OnClick = FilterChange
      end
      object cbFilterFull: TCheckBox
        Left = 136
        Top = 32
        Width = 97
        Height = 17
        TabStop = False
        Caption = 'Not Full'
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentCtl3D = False
        ParentFont = False
        TabOrder = 4
        OnClick = FilterChange
      end
    end
    object gbInfo: TGroupBox
      Left = 346
      Top = 0
      Width = 485
      Height = 99
      Align = alRight
      Caption = ' Server Info '
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBtnText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ParentCtl3D = False
      ParentFont = False
      PopupMenu = pmServers
      TabOrder = 1
      OnContextPopup = lbServersContextPopup
      OnDblClick = ToggleFilterServerInfo
      object lbSIAddressLab: TLabel
        Left = 8
        Top = 16
        Width = 51
        Height = 14
        Caption = 'Address:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object lbSIModeLab: TLabel
        Left = 8
        Top = 64
        Width = 34
        Height = 14
        Caption = 'Mode:'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object lbSIMapLab: TLabel
        Left = 8
        Top = 80
        Width = 57
        Height = 14
        Caption = 'Language:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object lbSIPlayersLab: TLabel
        Left = 8
        Top = 32
        Width = 44
        Height = 14
        Caption = 'Players:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object lbSIPingLab: TLabel
        Left = 8
        Top = 48
        Width = 27
        Height = 14
        Caption = 'Ping:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object lbSIPing: TLabel
        Left = 72
        Top = 47
        Width = 175
        Height = 13
        AutoSize = False
        Caption = '- - -'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object lbSIPlayers: TLabel
        Left = 72
        Top = 32
        Width = 175
        Height = 13
        AutoSize = False
        Caption = '- - -'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object lbSIMap: TLabel
        Left = 72
        Top = 80
        Width = 175
        Height = 13
        AutoSize = False
        Caption = '- - -'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object lbSIMode: TLabel
        Left = 72
        Top = 64
        Width = 175
        Height = 13
        AutoSize = False
        Caption = '- - -'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        OnDblClick = ToggleFilterServerInfo
      end
      object edSIAddress: TEdit
        Left = 72
        Top = 16
        Width = 175
        Height = 13
        TabStop = False
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBtnText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = True
        ParentFont = False
        ParentShowHint = False
        PopupMenu = pmCopy
        ReadOnly = True
        ShowHint = False
        TabOrder = 0
        Text = '- - -'
      end
      object pnSIDivider: TPanel
        Left = 256
        Top = 16
        Width = 1
        Height = 77
        BevelOuter = bvNone
        Caption = 'pnSIDivider'
        Color = clDefault
        TabOrder = 1
      end
      object chSIPingChart: TChart
        Left = 264
        Top = 17
        Width = 220
        Height = 73
        AllowPanning = pmNone
        BackWall.Brush.Color = clWhite
        BackWall.Color = clBlack
        BottomWall.Color = 16384
        Foot.Visible = False
        LeftWall.Brush.Color = clWhite
        LeftWall.Color = 8454016
        Title.AdjustFrame = False
        Title.Text.Strings = (
          'Time')
        Title.Visible = False
        BackColor = clBlack
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Maximum = 60
        BottomAxis.Visible = False
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Axis.Visible = False
        LeftAxis.Grid.Color = clBlue
        LeftAxis.Grid.Style = psSolid
        LeftAxis.Grid.SmallDots = True
        LeftAxis.Grid.Visible = False
        LeftAxis.LabelsFont.Charset = DEFAULT_CHARSET
        LeftAxis.LabelsFont.Color = clBtnText
        LeftAxis.LabelsFont.Height = -11
        LeftAxis.LabelsFont.Name = 'Tahoma'
        LeftAxis.LabelsFont.Style = []
        LeftAxis.Maximum = 50
        LeftAxis.MinorGrid.Color = 7303023
        LeftAxis.MinorGrid.SmallDots = True
        LeftAxis.MinorGrid.Visible = True
        LeftAxis.MinorTickCount = 1
        LeftAxis.MinorTickLength = 3
        LeftAxis.MinorTicks.SmallDots = True
        LeftAxis.EndPosition = 97
        LeftAxis.PositionPercent = -2
        LeftAxis.TickLength = 6
        LeftAxis.TicksInner.Visible = False
        LeftAxis.Title.Caption = 'Ping'
        LeftAxis.Title.Font.Charset = DEFAULT_CHARSET
        LeftAxis.Title.Font.Color = clBtnText
        LeftAxis.Title.Font.Height = -11
        LeftAxis.Title.Font.Name = 'Arial'
        LeftAxis.Title.Font.Style = []
        Legend.ShadowColor = clSilver
        Legend.Visible = False
        MaxPointsPerPage = 30
        RightAxis.Visible = False
        TopAxis.Labels = False
        TopAxis.LabelsFont.Charset = DEFAULT_CHARSET
        TopAxis.LabelsFont.Color = clWindowText
        TopAxis.LabelsFont.Height = -11
        TopAxis.LabelsFont.Name = 'Arial'
        TopAxis.LabelsFont.Style = []
        TopAxis.Ticks.Color = clWindowText
        TopAxis.Visible = False
        View3D = False
        View3DWalls = False
        BevelOuter = bvNone
        ParentColor = True
        TabOrder = 2
        OnDblClick = ToggleFilterServerInfo
        object chSIPingLineChart: TFastLineSeries
          Marks.ArrowLength = 8
          Marks.Style = smsValue
          Marks.Visible = False
          SeriesColor = 33023
          LinePen.Color = 33023
          LinePen.Width = 2
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
    end
  end
  object pnLine: TPanel
    Left = 0
    Top = 28
    Width = 831
    Height = 0
    Align = alTop
    BevelOuter = bvNone
    Color = clBlack
    TabOrder = 4
  end
  object pnRight: TPanel
    Left = 610
    Top = 28
    Width = 221
    Height = 380
    Align = alRight
    BevelOuter = bvNone
    Color = 4933703
    Ctl3D = True
    ParentCtl3D = False
    TabOrder = 5
    object Splitter1: TSplitter
      Left = 0
      Top = 220
      Width = 221
      Height = 11
      Cursor = crVSplit
      Align = alBottom
      Beveled = True
      Color = clMenu
      ParentColor = False
      ResizeStyle = rsUpdate
    end
    object pnPlayers: TPanel
      Left = 0
      Top = 0
      Width = 221
      Height = 220
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object lbPlayers: TListBox
        Left = 0
        Top = 17
        Width = 221
        Height = 203
        TabStop = False
        Style = lbOwnerDrawFixed
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 19
        ParentFont = False
        TabOrder = 0
        OnDrawItem = lbPlayersDrawItem
        OnExit = lbPlayersExit
      end
      object hcPlayers: THeaderControl
        Left = 0
        Top = 0
        Width = 221
        Height = 17
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        Images = ilMain
        Sections = <
          item
            AllowClick = False
            ImageIndex = -1
            MinWidth = 30
            Text = 'Player'
            Width = 140
          end
          item
            AllowClick = False
            ImageIndex = -1
            MinWidth = 30
            Text = 'Score'
            Width = 60
          end>
        Style = hsFlat
        OnSectionResize = hcPlayersSectionResize
        ParentFont = False
      end
    end
    object pnRules: TPanel
      Left = 0
      Top = 231
      Width = 221
      Height = 149
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      object label_url: TLabel
        Left = 0
        Top = 131
        Width = 221
        Height = 18
        Cursor = crHandPoint
        Align = alBottom
        Alignment = taCenter
        AutoSize = False
        Color = clWindow
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsItalic, fsUnderline]
        ParentColor = False
        ParentFont = False
        Layout = tlCenter
        OnClick = label_urlClick
      end
      object lbRules: TListBox
        Left = 0
        Top = 17
        Width = 221
        Height = 114
        TabStop = False
        Style = lbOwnerDrawFixed
        Align = alClient
        BorderStyle = bsNone
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 19
        ParentFont = False
        TabOrder = 0
        OnDrawItem = lbRulesDrawItem
        OnExit = lbRulesExit
      end
      object hcRules: THeaderControl
        Left = 0
        Top = 0
        Width = 221
        Height = 17
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        Images = ilMain
        Sections = <
          item
            AllowClick = False
            ImageIndex = -1
            MinWidth = 30
            Text = 'Rule'
            Width = 120
          end
          item
            AllowClick = False
            ImageIndex = -1
            MinWidth = 30
            Text = 'Value'
            Width = 100
          end>
        Style = hsFlat
        OnSectionResize = hcRulesSectionResize
        ParentFont = False
      end
    end
  end
  object pnMain: TPanel
    Left = 0
    Top = 28
    Width = 601
    Height = 380
    Align = alClient
    BevelOuter = bvNone
    Color = 4933703
    TabOrder = 6
    object hcServers: THeaderControl
      Left = 0
      Top = 0
      Width = 601
      Height = 17
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBtnFace
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      Images = ilMain
      Sections = <
        item
          AllowClick = False
          ImageIndex = 5
          MaxWidth = 28
          MinWidth = 28
          Width = 28
        end
        item
          ImageIndex = -1
          MinWidth = 30
          Text = 'HostName'
          Width = 250
        end
        item
          ImageIndex = -1
          MinWidth = 30
          Text = 'Players'
          Width = 60
        end
        item
          ImageIndex = -1
          MinWidth = 30
          Text = 'Ping'
          Width = 40
        end
        item
          ImageIndex = -1
          MinWidth = 30
          Text = 'Mode'
          Width = 120
        end
        item
          ImageIndex = -1
          MinWidth = 30
          Text = 'Language'
          Width = 90
        end>
      OnDrawSection = hcServersDrawSection
      OnSectionClick = hcServersSectionClick
      OnSectionResize = hcServersSectionResize
      ParentFont = False
    end
    object lbServers: TListBox
      Left = 0
      Top = 17
      Width = 601
      Height = 363
      Style = lbOwnerDrawFixed
      Align = alClient
      BorderStyle = bsNone
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 19
      ParentFont = False
      PopupMenu = pmServers
      TabOrder = 0
      OnClick = lbServersClick
      OnContextPopup = lbServersContextPopup
      OnDblClick = ServerPropertiesClick
      OnDrawItem = lbServersDrawItem
    end
  end
  object mmMain: TMainMenu
    BiDiMode = bdLeftToRight
    Images = ilMain
    ParentBiDiMode = False
    Left = 48
    Top = 61
    object miFile: TMenuItem
      Caption = 'File'
      object miImportFavoritesList: TMenuItem
        Caption = 'Import Favorites List'
        ImageIndex = 12
        OnClick = ImportFavoritesClick
      end
      object miExportFavoritesList: TMenuItem
        Caption = 'Export Favorites List'
        ImageIndex = 11
        OnClick = ExportFavoritesClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object miExit: TMenuItem
        Caption = 'Exit'
        ImageIndex = 14
        OnClick = ExitClick
      end
    end
    object miView: TMenuItem
      Caption = 'View'
      OnClick = miViewClick
      object miFilterServerInfo: TMenuItem
        Caption = 'Filter / Server Info'
        OnClick = ToggleFilterServerInfo
      end
      object N10: TMenuItem
        Caption = '-'
      end
      object miStatusBar: TMenuItem
        Caption = 'Status Bar'
        OnClick = ToggleStatusBar
      end
    end
    object miServers: TMenuItem
      Caption = 'Servers'
      object miConnect: TMenuItem
        Caption = 'Connect'
        ImageIndex = 6
        OnClick = ConnectClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object miAddServer: TMenuItem
        Caption = 'Add Server'
        ImageIndex = 8
        OnClick = AddServerClick
      end
      object miDeleteServer: TMenuItem
        Caption = 'Delete Server'
        ImageIndex = 9
        OnClick = DeleteServerClick
      end
      object miRefreshServer: TMenuItem
        Caption = 'Refresh Server'
        ImageIndex = 7
        OnClick = RefreshServerClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object miMasterServerUpdate: TMenuItem
        Caption = 'Master Server Update'
        ImageIndex = 2
        OnClick = MasterServerUpdateClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object miCopyServerInfo: TMenuItem
        Caption = 'Copy Server Info'
        ImageIndex = 1
        OnClick = CopyServerInfoClick
      end
      object miServerProperties: TMenuItem
        Caption = 'Server Properties'
        ImageIndex = 10
        OnClick = ServerPropertiesClick
      end
    end
    object miTools: TMenuItem
      Caption = 'Tools'
      object miSettings: TMenuItem
        Caption = 'Settings'
        ImageIndex = 0
        OnClick = SettingsClick
      end
    end
    object miHelp: TMenuItem
      Caption = 'Help'
      object miHelpTopics: TMenuItem
        Caption = 'Help Topics'
        ImageIndex = 4
        OnClick = HelpTopicsClick
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object miSamp: TMenuItem
        Caption = 'SA-MP.com'
        ImageIndex = 15
        OnClick = miSampClick
      end
      object N11: TMenuItem
        Caption = '-'
      end
      object miAbout: TMenuItem
        Caption = 'About'
        ImageIndex = 3
        OnClick = AboutClick
      end
    end
  end
  object pmServers: TPopupMenu
    Images = ilMain
    Left = 80
    Top = 61
    object piConnect: TMenuItem
      Caption = 'Connect'
      ImageIndex = 6
      OnClick = ConnectClick
    end
    object N7: TMenuItem
      Caption = '-'
    end
    object AddtoFavorites1: TMenuItem
      Caption = 'Add to Favorites'
      ImageIndex = 8
      OnClick = AddServerClick
    end
    object piDeleteServer: TMenuItem
      Caption = 'Delete Server'
      ImageIndex = 9
      OnClick = DeleteServerClick
    end
    object piRefreshServer: TMenuItem
      Caption = 'Refresh Server'
      ImageIndex = 7
      OnClick = RefreshServerClick
    end
    object N9: TMenuItem
      Caption = '-'
    end
    object piCopyServerInfo: TMenuItem
      Caption = 'Copy Server Info'
      ImageIndex = 1
      OnClick = CopyServerInfoClick
    end
    object piServerProperties: TMenuItem
      Caption = 'Server Properties'
      ImageIndex = 10
      OnClick = ServerPropertiesClick
    end
  end
  object ilMain: TImageList
    BlendColor = 4933703
    BkColor = 4933703
    DrawingStyle = dsTransparent
    Left = 16
    Top = 61
    Bitmap = {
      494C01011000130004001000100047484B00FF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000005000000001002000000000000050
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B484700845A5A00313139004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847006666660000000000000000000000000000000000336666006666
      66004B4847004B4847004B4847004B4847004B48470073CE7B00086B0800086B
      0800086B0800086B0800086B0800086B0800086B0800086B0800086B0800086B
      0800006300000063000073CE7B004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B484700845A
      5A00AD5A6300AD5A6300AD5A6300313139004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847006699
      660000000000CC333300CC333300CC333300CC333300CC333300CC3333000000
      0000666666004B4847004B4847004B4847004B48470000940000219418002194
      180021A5390021A539002194180021A539002194180010A5180010A5180010A5
      18000094000000940000006300004B484700CEB5A500A5948C00A5948C00A594
      8C00A5948C00A5948C00A5948C00A5948C00A5948C00A5948C00A5948C00A594
      8C00A5948C00A5948C00A5948C00CEC6C6004B484700845A5A00AD5A6300BD7B
      5A00CE735A00CE735A00AD5A630031313900AD5A6300AD5A6300AD5A6300845A
      5A00AD5A6300AD5A63004B4847004B4847004B4847004B484700999966000066
      3300339933000033000066003300CC666600CC666600CC666600CC333300CC33
      330099333300996666004B4847004B4847004B484700219418002194180021A5
      390021A5390021A5390021A53900ADBD9C00FFFFFF0010A5180010A5180008BD
      100008BD100000940000006300004B484700844A390052291000522910005229
      1000522910005229100052291000522910005229100052291000522910005229
      100052291000522910008C311000845A5A004B484700AD5A6300BD7B5A00CE73
      5A00CE735A00BD7B5A00CE735A0031313900B5847B00D67B8C00EFA59C00EFA5
      9C00EFA59C00845A5A004B4847004B4847004B48470066CC9900339933003399
      33000099330033CC66003366330099663300FF666600CC996600CC336600CC33
      33003333660000000000666666004B4847004B4847002194180021A5390052AD
      4A0052AD4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF0010A5180008BD
      100010A5180000940000086B08004B48470063524A0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000052291000847363004B484700AD5A6300CE735A00BD7B
      5A00E7736300E7736300BD7B5A00313139000094000000940000009400000094
      0000EFA59C00AD5A63004B4847004B4847004B4847003399330033CC330066CC
      660066CC33003399660000333300CC663300CC666600CC663300996666000066
      33003399000000663300000000004B4847004B4847002194180052AD4A0052AD
      4A0052AD4A0052AD4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF0008BD
      100010A5180010A51800009400004B48470063524A000000000044444400CDCD
      CD00000000000000000044444400CDCDCD00FFFFFF00FFFFFF00FFFFFF00CDCD
      CD00444444000000000052291000847363004B484700AD5A6300CE735A00E773
      6300E7736300D67B8C00CE848400313139000094000000940000009400000094
      0000EFA59C00AD5A63004B4847004B4847004B48470066CC6600CCFF990066CC
      330066CC66003399330000333300CC663300CC993300CC666600339900003399
      3300339933003399000000003300666666004B48470021A5390052AD4A0052AD
      4A0052AD4A0052AD4A0052AD4A0052AD4A0021A53900FFFFFF00FFFFFF00FFFF
      FF0010A5180010A51800009400004B48470063524A0000000000000000004444
      4400CDCDCD000000000000000000000000000000000000000000000000000000
      0000000000000000000052291000847363004B484700AD5A6300D67B8C00D67B
      8C00D67B8C00D67B8C00CE848400313139000094000000940000009400000094
      0000EFA59C00845A5A004B4847004B48470099CC6600CCFFCC0099CC990099FF
      6600006633003366000066663300CC663300CC99330099663300339966003399
      3300336633000099330033330000000000004B48470052AD4A0052AD4A00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0021941800219418004B48470063524A0000000000000000000000
      000044444400BBBBBB0000000000000000000000000000000000000000000000
      0000000000000000000052291000845A5A004B484700AD5A6300EF9C7B00D67B
      8C00EFA59C00F7E7E700CE848400313139000094000000940000006300000063
      0000EFA59C00AD5A63004B4847004B48470099CC9900FFFFFF00CCFFCC0099CC
      990033993300CC993300CC993300CC993300CC993300996633003399330066CC
      6600006633003399000000663300000000004B48470052AD4A0052AD4A00F7F7
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0021941800216B29004B48470063524A0000000000000000004444
      4400BBBBBB000000000000000000000000000000000000000000000000000000
      0000000000000000000052291000847363004B484700AD5A6300CE947B00EF9C
      7B00EFA59C00F7F7FF00D67B8C0031313900FFCEA500EFB57B00EFB57B00EFB5
      7B00EFA59C00AD5A63004B4847004B4847003399660099CC990066CC99003399
      3300CC993300CCCC6600CC993300CCCC6600CC9933009966330033CC66006699
      3300669966003399330033663300000000004B48470052AD4A0052AD4A0052AD
      4A0052AD4A0052AD4A0052AD4A0052AD4A0021A53900FFFFFF00FFFFFF00FFFF
      FF0021A5390021941800216B29004B48470063524A000000000044444400BBBB
      BB00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000052291000847363004B484700AD5A6300EF9C7B00EF9C
      7B00EF9C7B00EF9C7B00D67B8C0031313900FFCEA500F7E7BD00F7E7BD00F7E7
      BD00EFA59C00845A5A004B4847004B484700FFFFCC00CCCC9900CCCC3300CCCC
      3300CCCC6600CCCC6600FFCC6600CC993300CC993300CC9933006666330066CC
      660066CC66006699330000660000000000004B48470052AD4A0073CE7B0073CE
      7B0052AD4A0052AD4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF002194
      18002194180021941800216B29004B48470063524A0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000052291000847363004B484700AD5A6300CE947B00EF9C
      7B00EF9C7B00CE947B00D67B8C0031313900FFCEA500F7E7BD00EFCEC600F7E7
      BD00EFA59C00AD5A63004B4847004B4847004B484700CCCC9900FFCC9900CCFF
      99000033000066663300CCCC6600CCCC3300CC996600CC993300CC9933003366
      330066CC66003399330033333300666666004B48470052AD4A0073CE7B0073CE
      7B0052AD4A0052AD4A0052AD4A00F7F7FF00FFFFFF00FFFFFF0021A5390021A5
      390021A5390021A53900216B29004B48470063524A0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000052291000845A5A004B484700AD5A6300EF9C7B00EFA5
      9C00EFA59C00EF9C7B00D67B8C0031313900FFCEA500F7E7BD00F7E7BD00F7E7
      BD00EFA59C00AD5A63004B4847004B4847004B484700FFFFCC00CCCC99003366
      33003399330000333300CCCC6600CCCC6600FF993300CCCC3300CC993300CC66
      33009966660033663300000000004B4847004B48470073CE7B0073CE7B0073CE
      7B0073CE7B0073CE7B0052AD4A00ADD6C600F7EFF70052AD4A0052AD4A0052AD
      4A0021A5390021A53900216B29004B484700BD7B5A005A5229005A522900525A
      42005A5229005A522900525A42005A5229005A522900525A420063524A006352
      4A0063524A0063524A00844A3900845A5A004B484700AD5A6300CE947B00EF9C
      7B00EFA59C00CE9C9C00EF9C7B0031313900F7CEA500F7E7BD00F7E7BD00EFCE
      C600EFA59C00845A5A004B4847004B4847004B484700CCCCCC0066CC660099CC
      6600339933003366330000333300CCCC3300CCCC6600CC993300CC993300CC66
      3300CC663300CC333300666666004B4847004B48470073CE7B00ADBD9C0073CE
      7B0073CE7B0073CE7B0073CE7B0073CE7B0052AD4A0052AD4A0052AD4A0052AD
      4A0052AD4A0021A53900086B08004B484700DE730800F79C1800F79C1800F79C
      1800F79C1800F79C1800F79C1800F79C1800F79C1800F79C1800FFC64A00FFC6
      4A00FFC64A00A59C9C009C63310094847B004B484700AD5A6300AD5A6300EFA5
      9C00EF9C7B00EFA59C00EF9C7B0031313900F7CEA500F7E7BD00F7E7BD00F7E7
      BD00EFA59C00AD5A63004B4847004B4847004B4847004B48470099CC9900FFFF
      FF00CCFF990033CC330033663300003333000033330000333300FF993300CC99
      3300CC333300666666004B4847004B4847004B484700ADD6C60073CE7B0073CE
      7B0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0021A5
      390021A539002194180073CE7B004B484700BD7B5A00CE6B3100CE6B3100CE6B
      31009C633100CE6B3100CE6B3100CE6B31009C633100CE6B3100CE6B3100CE6B
      31009C633100CE6B3100BD7B5A00CEC6C6004B4847004B4847004B484700AD5A
      6300B5847B00CE848400CE84840031313900AD5A6300AD5A6300AD5A6300845A
      5A00AD5A6300AD5A63004B4847004B4847004B4847004B4847004B48470099CC
      990099CCCC00FFFFCC0066CC330066CC6600339933003399330000330000CC33
      3300FFCC99004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700AD5A6300AD5A6300313139004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B48470099CC990099CCCC0099CC990066CC330033CC330066CC99004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700B5847B00B5847B00B584
      7B00B5847B00B5847B00B5847B00B5847B00B5847B00B5847B00B5847B00B584
      7B00B5847B00B5847B004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847005AA5FF0008428400006B
      9400006B9400006B9400185ABD00185ABD00006B9400006B9400084284000842
      840008428400084284004AB5F7004B4847004B4847005AA5FF0008428400006B
      9400006B9400006B9400185ABD00185ABD00006B9400006B9400084284000842
      840008428400084284004AB5F7004B4847004B484700CE9C9C00FFF7DE00F7E7
      BD00F7E7BD00F7E7BD00F7CEA500FFDEA500E7BD9C00F7CEA500F7CEA500F7CE
      A500F7CEA500B5847B004B4847004B4847004B48470073CE7B00086B0800086B
      0800086B0800086B0800086B0800086B0800086B0800086B0800086B0800086B
      0800006300000063000073CE7B004B4847004B484700185ABD00185ABD00107B
      DE00107BDE00107BDE00107BDE00107BDE00107BDE00107BDE00107BDE00185A
      BD00185ABD00185ABD00084284004B4847004B484700185ABD00185ABD00107B
      DE00107BDE00107BDE00107BDE00107BDE00107BDE00107BDE00107BDE00185A
      BD00185ABD00185ABD00084284004B4847004B484700CE9C9C00FFF7DE00CEC6
      C600CEC6C600CEC6C600CEC6C600CEC6C600CEC6C600CEC6C600CEC6C600CEC6
      C600E7BD9C00B5847B004B4847004B4847004B48470000940000219418002194
      180021A5390021A539002194180021A539002194180010A5180010A5180010A5
      18000094000000940000006300004B4847004B484700185ABD00107BDE00107B
      DE00107BDE00107BDE00218CE7001894FF001884FF001894FF001894FF00107B
      DE00185ABD00185ABD00084284004B4847004B484700185ABD00107BDE00107B
      DE00107BDE00107BDE00218CE7001894FF001894FF001894FF001884FF00107B
      DE00185ABD00185ABD00084284004B4847004B484700CEB5A500FFE7DE003131
      390063524A00DEBDBD00F7E7BD00F7CEA500F7CEA500E7BD9C00F7CEA500F7CE
      A500F7CEA500B5847B004B4847004B4847004B484700219418002194180021A5
      390021A5390021A5390021A53900FFFFFF00ADBD9C0052AD4A0052AD4A0052AD
      4A0008BD100000940000006300004B4847004B484700185ABD00107BDE000884
      F7000884F7000884F7007BCEFF001894FF001894FF001894FF001884FF00218C
      E700107BDE00185ABD00084284004B4847004B484700185ABD00107BDE000884
      F700FFFFFF007BCEFF001884FF001894FF001894FF001884FF007BCEFF00FFFF
      F700107BDE00185ABD00084284004B4847004B484700CEB5A500F7F7E7006352
      4A003131390063635A00DEBDBD00F7E7BD00E7BD9C00F7CEA500F7CEA500E7BD
      9C00F7CEA500B5847B004B4847004B4847004B4847002194180021A5390052AD
      4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF0052AD4A0052AD4A0052AD
      4A0010A5180000940000086B08004B4847004B484700107BDE00218CE7001894
      FF001884FF007BCEFF00FFF7F7007BCEFF001884FF001894FF001894FF00218C
      E700107BDE00185ABD00006B94004B4847004B484700107BDE00218CE7001894
      FF001884FF00FFFFFF007BCEFF001884FF001894FF007BCEFF00F7F7FF00218C
      E700107BDE00185ABD00006B94004B4847004B484700CEB5A500F7FFEF00CEC6
      C600636B7B00427BAD0008428400845A5A00CEB5A500CEC6C600CEC6C600CEC6
      C600F7CEA500B5847B004B4847004B4847004B4847002194180052AD4A0052AD
      4A0052AD4A00FFFFFF00FFFFFF00FFFFFF0052AD4A0052AD4A0052AD4A0052AD
      4A0010A5180010A51800009400004B4847004B484700107BDE001884FF001894
      FF007BCEFF00FFFFFF007BCEFF00FFFFFF007BCEFF001884FF001894FF00218C
      E700107BDE00107BDE00185ABD004B4847004B484700107BDE001894FF001894
      FF001894FF001894FF00FFFFFF007BCEFF007BCEFF00FFFFFF001894FF00218C
      E700107BDE00107BDE00185ABD004B4847004B484700DEBDBD00FFFFFF00FFF7
      EF00636B7B00427BAD0094847B00CE6B3100845A5A00DEBDBD00F7CEA500F7CE
      A500E7BD9C00B5847B004B4847004B4847004B48470021A5390052AD4A0052AD
      4A00FFFFFF00FFFFFF00FFFFFF0052AD4A0052AD4A0052AD4A0052AD4A0052AD
      4A0010A5180010A51800009400004B4847004B484700218CE7001894FF007BCE
      FF00F7F7FF007BCEFF001894FF001884FF00FFFFFF007BCEFF000884F700107B
      DE00107BDE00107BDE00185ABD004B4847004B484700218CE7001894FF001884
      FF001894FF001884FF001894FF00FFFFFF007BCEFF001884FF000884F700107B
      DE00107BDE00107BDE00185ABD004B4847004B484700DEBDBD00F7FFFF00FFF7
      F700FFF7EF0084736300EFB57B00E79C5200CE6B310063635A00DEBDBD00E7BD
      9C00F7CEA500B5847B004B4847004B4847004B48470052AD4A0052AD4A00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0021941800219418004B4847004B4847000884F7001894FF001894
      FF007BCEFF001884FF001894FF001894FF001894FF00FFFFFF007BCEFF00107B
      DE00107BDE00107BDE00185ABD004B4847004B4847000884F7001894FF001894
      FF001894FF001894FF007BCEFF00FFF7F700FFFFFF007BCEFF00218CE700107B
      DE00107BDE00107BDE00185ABD004B4847004B484700DEBDBD00FFFFFF00CEC6
      C600CEC6C600B5847B00F7CEA500FFD68400E79C5200CE6B3100845A5A00CEB5
      A500DEBDBD00CE9C9C004B4847004B4847004B48470052AD4A0052AD4A00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0021941800216B29004B4847004B4847001894FF001884FF001894
      FF001894FF001894FF001884FF001884FF000884F7000884F700FFFFF7007BCE
      FF00107BDE00107BDE00185ABD004B4847004B4847001894FF001884FF001894
      FF001884FF007BCEFF00FFFFFF001884FF000884F700FFFFFF007BCEFF00107B
      DE00107BDE00107BDE00185ABD004B4847004B484700DEBDBD00F7FFFF00FFFF
      FF00FFFFFF00FFF7F700B5847B00FFCEA500FFD68400E79C5200CE6B3100845A
      5A00CEC6C600CEB5A5004B4847004B4847004B48470052AD4A0052AD4A0052AD
      4A00FFEFF700FFFFFF00FFFFFF0052AD4A0052AD4A0052AD4A0052AD4A002194
      18002194180021941800216B29004B4847004B4847001894FF0039ADFF0039AD
      FF001884FF001894FF001894FF001894FF000884F700218CE700107BDE00FFFF
      F7007BCEFF00107BDE00185ABD004B4847004B4847001894FF0039ADFF0039AD
      FF007BCEFF00FFF7F7001894FF001894FF000884F700218CE700FFFFF7007BCE
      FF00107BDE00107BDE00185ABD004B4847004B484700DEBDBD00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFF7F700B5847B00F7CEA500FFD68400E79C5200CE6B
      3100845A5A00CEB5A5004B4847004B4847004B48470052AD4A0073CE7B0073CE
      7B0052AD4A00F7F7FF00FFFFFF00FFFFFF0052AD4A0052AD4A0052AD4A002194
      18002194180021941800216B29004B4847004B4847001884FF005AA5FF004AB5
      F70029A5FF001894FF001894FF001884FF001894FF000884F700218CE700107B
      DE00FFFFF700107BDE00185ABD004B4847004B4847001894FF005AA5FF004AB5
      F700FFFFFF001894FF001884FF001894FF001894FF000884F700218CE700FFFF
      F700107BDE00107BDE00185ABD004B4847004B484700EFC6C600FFFFF700CEC6
      C600CEC6C600CEC6C600CEC6C600CEC6C600B5847B00F7CEA500A594940029A5
      FF001018B50008087B004B4847004B4847004B48470052AD4A0073CE7B0073CE
      7B0052AD4A0052AD4A00F7EFF700FFFFFF00FFFFFF0052AD4A0021A5390021A5
      390021A5390021A53900216B29004B4847004B48470039ADFF0063C6FF005AA5
      FF0039ADFF0029A5FF0029A5FF0029A5FF001894FF001884FF001884FF000884
      F700107BDE00107BDE00185ABD004B4847004B48470039ADFF0063C6FF005AA5
      FF0039ADFF0029A5FF0029A5FF0029A5FF001894FF001884FF001884FF000884
      F700107BDE00107BDE00185ABD004B4847004B484700DEBDBD00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFF700B5847B004AA5F7001010
      DE001010DE001018B50008087B004B4847004B48470073CE7B0073CE7B0073CE
      7B0073CE7B0073CE7B0052AD4A00F7EFF700ADD6C60052AD4A0052AD4A0052AD
      4A0021A5390021A53900216B29004B4847004B4847004AB5FF007BCEFF0063C6
      FF005AA5FF004AB5FF0039ADFF0029A5FF0029A5FF001894FF001894FF001884
      FF000884F700107BDE00006B94004B4847004B4847004AB5FF007BCEFF0063C6
      FF005AA5FF004AB5FF0039ADFF0029A5FF0029A5FF001894FF001894FF001884
      FF000884F700107BDE00006B94004B4847004B484700EFCEC600FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00EFCEC6001018B5005A84
      FF000073FF001010DE004B4847004B4847004B48470073CE7B00ADBD9C0073CE
      7B0073CE7B0073CE7B0073CE7B0073CE7B0052AD4A0052AD4A0052AD4A0052AD
      4A0052AD4A0021A53900086B08004B4847004B484700A5E7FF004AB5FF0029A5
      FF001894FF001894FF001884FF001894FF001894FF000884F700218CE700107B
      DE00107BDE00185ABD005ABDFF004B4847004B484700A5E7FF004AB5FF0029A5
      FF001894FF001894FF001884FF001894FF001894FF000884F700218CE700107B
      DE00107BDE00185ABD005ABDFF004B4847004B484700DEBDBD00FFF7F700F7F7
      E700FFF7EF00F7F7E700FFF7EF00FFF7EF00F7F7E700EFD6D600CE947B001018
      B5001018B5004B4847004B4847004B4847004B484700ADD6C60073CE7B0073CE
      7B0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0021A5
      390021A539002194180073CE7B004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700EFC6C600EFCEC600EFCE
      C600EFCEC600EFCEC600DEBDBD00EFC6C600EFCEC600DEBDBD00B5847B004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      4700949C9400636B7B004A524A004A524A004A524A004A524A004A524A00A59C
      9C004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B484700ADAD
      BD00EFCEC600E7DED600FFE7DE00FFE7DE00E7DED600E7DED600DEBDBD006363
      5A0084736300A59C9C004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700188CBD00188CBD00188CBD00188CBD00188CBD004B4847004B48
      47004B4847004B4847004B4847004B4847004B48470073CE7B00086B0800086B
      0800086B0800086B0800086B0800086B0800086B0800086B0800086B0800086B
      0800006300000063000073CE7B004B4847004B4847005AA5FF0008428400006B
      9400006B9400006B9400185ABD00185ABD00006B9400006B9400084284000842
      840008428400084284004AB5F7004B4847004B4847004B484700CEC6C600FFE7
      DE00F7F7F700E7E7E700CEB5A500CEB5A500CEC6C600EFF7F700FFF7EF00E7DE
      D60094847B00636B7B00A59C9C004B4847004B4847004B4847004B484700299C
      C6004AA5AD0052C6F7004ADEFF004ADEFF004ADEFF0052C6F700188CBD00188C
      BD004B4847004B4847004B4847004B4847004B48470000940000219418002194
      180021A5390021A539002194180021A539002194180010A518002194180010A5
      18000094000000940000006300004B4847004B484700185ABD00185ABD00107B
      DE00107BDE00107BDE00107BDE00107BDE00107BDE00107BDE00107BDE00185A
      BD00185ABD00185ABD00084284004B4847004B484700EFD6D600FFE7DE00EFFF
      FF00CE947B00AD4A1800CE6B3100CEB5A500C65A1000C65A1000CEB5A500EFFF
      FF00FFE7DE0084736300847363004B4847004B4847004B484700299CC60052C6
      F700A5E7FF00A5E7FF00A5E7FF00000000004ADEFF004ADEFF004ADEFF0052C6
      F700188CBD004B4847004B4847004B4847004B484700219418002194180021A5
      390021A5390021A53900FFFFFF00ADBD9C0052AD4A0052AD4A0052AD4A0021A5
      390010A5180000940000006300004B4847004B484700185ABD00107BDE00107B
      DE00107BDE00107BDE00107BDE001894FF001884FF001894FF001894FF00107B
      DE00185ABD00185ABD00084284004B4847004B484700E7DED600F7FFFF00CE73
      5A00C65A1000C65A1000CE9C9C00FFFFFF00EF9C7B00C65A1000AD4A1800CE94
      7B00F7FFFF00E7DED60063635A00A59C9C004B484700299CC60052C6F700A5E7
      FF00A5E7FF00A5E7FF00A5E7FF0000000000A5E7FF00A5E7FF004ADEFF004ADE
      FF0052C6F700188CBD004B4847004B4847004B4847002194180021A5390052AD
      4A0052AD4A0052AD4A00FFFFFF00FFFFFF00ADBD9C0052AD4A0052AD4A0021A5
      39002194180000940000086B08004B4847004B484700185ABD00107BDE000884
      F7000884F700FFFFFF001894FF00A5E7FF00A5E7FF00A5E7FF001894FF000884
      F700107BDE00185ABD00084284004B4847004B484700F7F7F700CEB5A500C65A
      1000CE6B3100CE6B3100CE6B3100EF9C7B00CE6B3100CE6B3100C65A1000AD4A
      1800DEBDBD00F7EFE700A594940063635A004B4847004AA5AD00ADF7FF00A5E7
      FF00A5E7FF00A5E7FF00A5E7FF0000000000A5E7FF00A5E7FF00A5E7FF004ADE
      FF004ADEFF00188CBD004B4847004B4847004B4847002194180052AD4A0052AD
      4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF00ADBD9C0052AD4A0021A5
      390010A5180010A51800009400004B4847004B484700107BDE00107BDE001894
      FF001894FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00A5E7FF000884
      F700107BDE00185ABD00006B94004B484700F7EFE700F7F7F700CE6B3100C65A
      1000CE6B3100C65A1000CE947B00FFE7DE00CE6B3100C65A1000CE6B3100C65A
      1000CE735A00F7FFFF00EFCEC6004A524A004B484700299CC600ADF7FF00ADF7
      FF00A5E7FF00A5E7FF00000000000000000000000000A5E7FF00A5E7FF00A5E7
      FF004ADEFF00188CBD004B4847004B4847004B48470021A5390052AD4A0052AD
      4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF00FFFFFF00ADBD9C0021A5
      390010A5180010A51800009400004B4847004B484700107BDE001894FF001884
      FF001894FF00FFFFFF00FFFFFF001894FF001884FF001894FF00FFFFFF00107B
      DE00107BDE00107BDE00185ABD004B484700F7EFE700E7DED600C65A1000CE6B
      3100CE6B3100C65A1000B5847B00FFFFFF00EFA59C00C65A1000CE6B3100CE6B
      3100C65A1000FFF7EF00E7DED6004A524A004B484700299CC600ADF7FF00ADF7
      FF00ADF7FF00A5E7FF0094847B000000000094847B00A5E7FF00A5E7FF00A5E7
      FF004ADEFF00188CBD004B4847004B4847004B48470052AD4A0052AD4A0052AD
      4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00ADBD
      9C002194180021941800219418004B4847004B484700107BDE001894FF001894
      FF001894FF00FFF7F700FFFFFF00FFFFFF001894FF001894FF000884F700107B
      DE00107BDE00107BDE00185ABD004B484700F7EFE700F7E7BD00C65A1000CE6B
      3100CE6B3100CE6B3100C65A1000CEB5A500F7FFFF00EF9C7B00C65A1000CE6B
      3100C65A1000FFF7EF00E7DED6004A524A004B4847004AA5AD0052C6F700ADEF
      FF00ADF7FF00ADEFFF00ADF7FF00A5E7FF00A5E7FF00A5E7FF00A5E7FF00A5E7
      FF0052C6F700188CBD004B4847004B4847004B48470052AD4A0052AD4A0052AD
      4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00ADBD
      9C002194180021941800216B29004B4847004B4847000884F7001894FF001884
      FF001894FF001894FF001894FF001894FF001884FF000884F700107BDE00107B
      DE00107BDE00107BDE00185ABD004B484700F7EFE700F7EFE700CE6B3100CE6B
      3100CE6B3100C65A1000C65A1000C65A1000EFCEC600F7FFFF00CE6B3100C65A
      1000CE6B3100FFF7F700E7DED600636B7B004B4847004B484700299CC6004AA5
      AD00299CC6004AA5AD00299CC600299CC6004AA5AD00427BD600299CC6004AA5
      AD00299CC6004B4847004B4847004B4847004B48470052AD4A0052AD4A0052AD
      4A0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF00FFFFFF00ADBD9C0021A5
      39002194180021941800216B29004B4847004B4847001884FF001894FF001894
      FF001894FF001894FF001884FF001884FF00FFFFFF00FFFFFF00FFFFFF00107B
      DE00107BDE00107BDE00185ABD004B484700FFFFFF00FFFFFF00EF9C7B00CE6B
      3100CE947B00E7E7E700CE735A00AD391800CE947B00FFFFFF00E7736300C65A
      1000EF9C7B00F7FFFF00DEBDBD004B4847004B4847004B4847004B484700299C
      C600A5E7FF00188CBD004B4847004B4847004B4847004AA5AD00A5E7FF00188C
      BD004B4847004B4847004B4847004B4847004B48470052AD4A0073CE7B0073CE
      7B0052AD4A0052AD4A00FFFFFF00FFFFFF00FFFFFF00ADBD9C0021A5390021A5
      39002194180021941800216B29004B4847004B4847001894FF0039ADFF0039AD
      FF001884FF00FFFFFF001894FF001894FF000884F700FFFFFF00FFFFFF00107B
      DE00107BDE00107BDE00185ABD004B4847004B484700F7EFE700FFF7DE00E79C
      5200E7736300E7FFFF00FFFFFF00E7BD9C00F7F7FF00E7FFFF00CE6B3100CE6B
      3100FFF7EF00FFE7DE00A59494004B4847004B4847004B4847004B4847004AA5
      AD00A5E7FF00188CBD004B4847004B4847004B484700299CC600A5E7FF00188C
      BD004B4847004B4847004B4847004B4847004B48470052AD4A0073CE7B0073CE
      7B0052AD4A0052AD4A00FFEFF700FFFFFF00ADBD9C0052AD4A0052AD4A0021A5
      390021A5390021A53900216B29004B4847004B4847001894FF005AA5FF004AB5
      F70029A5FF00A5E7FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00107B
      DE00107BDE00107BDE00185ABD004B4847004B484700E7DED600FFFFFF00FFEF
      CE00EFB57B00EFB57B00E7E7E700EFF7F700E7E7E700EF9C7B00E79C5200FFEF
      CE00FFFFFF00E7DED6004B4847004B4847004B4847004B4847004B484700299C
      C60052C6F70052C6F700188CBD00188CBD00188CBD0052C6F70052C6F700188C
      BD004B4847004B4847004B4847004B4847004B48470073CE7B0073CE7B0073CE
      7B0073CE7B0073CE7B00E7EFFF00ADBD9C0052AD4A0052AD4A0052AD4A0052AD
      4A0021A5390021A53900216B29004B4847004B48470039ADFF0063C6FF005AA5
      FF0039ADFF0029A5FF00A5E7FF00A5E7FF00A5E7FF001884FF00FFFFFF000884
      F700107BDE00107BDE00185ABD004B4847004B4847004B484700E7DED600F7FF
      FF00FFFFFF00FFEFCE00FFDEA500F7CEA500FFCEA500FFDEA500FFF7F700FFFF
      FF00E7DED6004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004AA5AD0052C6F700A5E7FF00A5E7FF00A5E7FF0052C6F700188CBD004B48
      47004B4847004B4847004B4847004B4847004B48470073CE7B00ADBD9C0073CE
      7B0073CE7B0073CE7B0073CE7B0073CE7B0052AD4A0052AD4A0052AD4A0052AD
      4A0052AD4A0021A53900086B08004B4847004B4847004AB5FF007BCEFF0063C6
      FF005AA5FF004AB5FF0039ADFF0029A5FF0029A5FF001894FF001894FF001884
      FF000884F700107BDE00006B94004B4847004B4847004B4847004B484700E7DE
      D600F7E7E700FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00E7E7E700E7DE
      D6004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700299CC6004AA5AD00299CC600299CC600188CBD004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700ADD6C60073CE7B0073CE
      7B0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0052AD4A0021A5
      390021A539002194180073CE7B004B4847004B484700A5E7FF004AB5FF0029A5
      FF001894FF001894FF001884FF001894FF001894FF000884F700107BDE00107B
      DE00107BDE00185ABD005ABDFF004B4847004B4847004B4847004B4847004B48
      4700F7EFE700FFF7EF00F7EFE700FFF7EF00F7EFE700FFF7EF00F7EFE7004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700A5948C00845A5A00845A5A00845A5A00845A5A00845A5A00845A
      5A00845A5A00845A5A00845A5A004B4847004B484700B5847B00B5847B00B584
      7B00B5847B00B5847B00B5847B00B5847B00B5847B00B5847B00B5847B00B584
      7B00B5847B00B5847B004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700738C9400084284004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700A5948C00FFFFE700F7E7E700F7EFE700F7E7E700F7EFE700F7EF
      E700F7EFE700FFEFCE00845A5A004B4847004B484700CE9C9C00FFF7DE00427B
      D600427BD600F7E7BD00FFDEA500FFDEA500E7BD9C00F7CEA500F7CEA500E7BD
      9C00F7CEA500B5847B004B4847004B4847004B4847004B4847004B4847004B48
      47009C633100844A39004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B484700087BB500087BB50031428400188CBD0018A5D6003142
      8400006B9400006B94004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700A5948C00F7EFE700F7E7BD00F7E7BD00F7E7BD00F7E7BD00F7E7
      BD00E7DED600E7DED600845A5A004B4847004B484700CE9C9C00FFF7DE0000F7
      FF00427BD600427BD600A5949400A59C9C00A59C9C00A59C9C00A59C9C00A59C
      9C00F7CEA500B5847B004B4847004B4847004B4847004B4847004B4847004B48
      4700FFF7DE00844A3900844A39004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004AB5F7004AB5F700006B9400188CBD0018A5D600087B
      B500188CBD00188CBD004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700A5948C00FFFFE700FFDEA500FFDEA500FFDEA500F7CEA500FFDE
      A500FFDEA500E7DED600845A5A004B4847004B484700CEB5A500FFE7DE0039AD
      FF0000F7FF00427BD600427BD600427BD600FFDEA500E7BD9C00F7CEA500E7BD
      9C00F7CEA500B5847B004B4847004B4847004B4847004B4847004B4847004B48
      4700FFEFE700CE9C9C00844A3900844A39004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B48470031428400188CBD004AB5F7004AB5F7004AB5F70018A5D60018A5
      D60018A5D600188CBD0008428400396B84004B484700A5948C00845A5A00845A
      5A00845A5A00A5948C00FFF7EF00F7E7BD00F7E7BD00F7E7BD00F7E7BD00F7E7
      BD00FFEFCE00E7DED6009C7363004B4847004B484700CEB5A500F7F7E700FFE7
      DE0039ADFF0094F7FF0000F7FF00427BD600427BD600FFDEA500F7CEA500F7CE
      A500F7CEA500B5847B004B4847004B4847004B4847004B4847004B4847004B48
      4700FFEFE700FFEFCE00CE9C9C00844A3900844A3900844A3900844A3900844A
      39004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700314284004AB5F70052C6F7004AB5F70063C6FF00427BAD00087B
      B500188CBD00188CBD00006B9400006B94004B484700A5948C00FFFFE700F7E7
      E700F7EFE700A5948C00FFF7EF00FFEFCE00F7E7BD00F7E7BD00F7E7BD00F7E7
      BD00FFE7DE00E7DED6009C7363004B4847004B484700CEB5A500427BD600427B
      D600427BD600427BD60094F7FF0000F7FF00427BD600427BD600A5949400A59C
      9C00E7BD9C00B5847B004B4847004B4847004B4847004B4847009C633100844A
      3900FFEFCE00FFF7DE00FFF7DE00FFE7BD00F7E7BD00FFEFCE00CE9C9C009C63
      3100844A3900844A39004B4847004B4847004B4847004B4847004B4847004B48
      47004B48470063C6FF0052C6F7004AB5F70063C6FF00E7E7E700636B7B003142
      8400188CBD00188CBD0018A5D600739CC6004B484700A5948C00F7EFE700F7E7
      BD00F7E7BD00A5948C00FFFFF700FFDEA500FFDEA500FFDEA500FFDEA500FFDE
      A500FFDEA500E7DED6009C7363004B4847004B484700DEBDBD0039ADFF0094F7
      FF0000F7FF0000F7FF0000F7FF0000F7FF0000F7FF00427BD600427BD600FFDE
      A500FFDEA500B5847B004B4847004B4847004B4847009C633100DECEA500DECE
      A500FFEFCE00FFEFCE009C6331008C311000AD4A1800CE6B3100FFEFCE00F7E7
      BD00CE9C9C00844A3900844A39004B4847004B4847004B4847004B4847004B48
      47004B48470021A53900299CC6004AB5F70063C6FF00E7E7E700636B7B00396B
      840018A5D600299CC600188CBD004B4847004B484700A5948C00FFFFE700FFDE
      A500FFDEA500A5948C00FFFFF700FFE7DE00FFE7DE00FFE7DE00FFEFCE00FFE7
      DE00F7F7E700E7E7E7009C7363004B4847004B484700DEBDBD00FFFFFF0039AD
      FF0094F7FF0000F7FF00427BD600427BD600F7E7BD00F7E7BD00F7E7BD00F7E7
      BD00FFDEA500B5847B004B4847004B4847009C633100DECEA500FFEFCE00FFE7
      DE00FFF7DE00FFF7DE00FFF7DE008C3110008C100000FFF7DE00FFF7DE00FFEF
      CE00FFE7BD00CE9C9C00844A3900844A39004B48470052AD4A00009400000063
      00002194180029BD4A00299CC6004AB5F7005AC6D600D6D6D60063635A00396B
      840018A5D60018A5D6004B4847004B4847004B484700A5948C00FFF7EF00F7E7
      BD00F7E7BD00A5948C00FFFFFF00FFFFFF00FFFFFF00FFFFF700FFFFF700E7E7
      E700CEC6C600CEB5A500B5847B004B4847004B484700DEBDBD00F7FFFF00E7E7
      E70039ADFF0094F7FF0000F7FF00427BD600427BD600A5949400A59C9C00A594
      9400F7E7BD00B5847B004B4847004B484700DECEA500FFEFCE00FFE7DE00FFF7
      DE00FFF7DE00FFF7DE00FFF7DE008C3110008C100000FFF7DE00FFF7DE00FFF7
      DE00FFEFCE00FFE7BD00CE9C9C00844A39004B48470021A5390029DE5A0021A5
      39002194180029BD4A0021A5390029AD4A0052AD4A00CEB5A5004A524A008473
      6300299CC600299CC6004B4847004B4847004B484700A5948C00FFF7EF00FFEF
      CE00F7E7BD00A5948C00FFFFFF00FFFFFF00FFFFFF00FFFFF700FFFFF700A594
      8C00A5948C00A5948C00A5948C004B4847004B484700DEBDBD00FFFFFF00FFFF
      FF0039ADFF0094F7FF0000F7FF0000F7FF00427BD600427BD600F7E7BD00F7E7
      BD00F7E7BD00B5847B004B4847004B484700FFF7EF00FFEFE700F7E7E700FFF7
      DE00FFF7DE00FFEFCE008C3110008C3110008C100000FFF7DE00FFF7DE00FFEF
      CE00FFF7DE00FFEFCE00FFEFCE00844A39004B4847002194180029DE5A0029DE
      5A0029DE5A0029BD4A0029BD4A0021A5390010A518002194180063635A004B48
      47004B4847004B4847004B4847004B4847004B484700A5948C00FFFFF700FFDE
      A500FFDEA500A5948C00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00A594
      8C00EFB57B00CE947B004B4847004B4847004B484700DEBDBD00F7FFFF00FFFF
      FF00FFFFFF0039ADFF0094F7FF0000F7FF0000F7FF00427BD600427BD600DEE7
      C600CEB5A500B5847B004B4847004B484700FFF7EF00FFF7EF00FFEFE700F7E7
      E700FFF7DE00FFF7DE009C6331008C3110008C100000FFF7DE00FFF7DE00FFF7
      DE00FFF7DE00FFEFCE00FFEFCE00844A390021A5390021A5390029DE5A0029DE
      5A0073CE7B0073CE7B002194180010A5180010A518000094000052AD4A004B48
      47004B4847004B4847004B4847004B4847004B484700A5948C00FFFFF700FFE7
      DE00FFE7DE00A5948C00A5948C00A5948C00A5948C00A5948C00A5948C00A594
      8C00B5847B004B4847004B4847004B4847004B484700EFC6C600FFFFF700E7E7
      E700A59C9C00A59C9C0039ADFF0094F7FF0000F7FF0000F7FF00427BD600427B
      D600B5847B00B5847B004B4847004B4847004B484700FFF7EF00FFF7EF00FFEF
      E700FFEFE700FFF7DE00FFF7DE00FFF7DE00FFF7DE00FFF7DE00FFEFCE00FFF7
      DE00FFEFCE00FFF7EF00CE9C9C009C63310073CE7B0073CE7B0073CE7B0029DE
      5A00CEC6C600ADA5A500216B29000094000010A5180010A5180073CE7B004B48
      47004B4847004B4847004B4847004B4847004B484700A5948C00FFFFFF00FFFF
      FF00FFFFFF00FFFFF700FFFFF700E7E7E700CEC6C600CEB5A500B5847B004B48
      47004B4847004B4847004B4847004B4847004B484700EFC6C600FFFFF700FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFF700EFD6D600B5847B00EFB5
      7B00E79C5200B5847B004B4847004B4847004B4847004B484700FFF7EF00FFF7
      EF00FFEFE700FFEFE700F7E7E700AD391800CE6B3100FFF7DE00FFF7DE00FFEF
      CE00F7F7E700CE9C9C009C6331004B4847004B48470029DE5A0029DE5A0029DE
      5A00D6D6D600ADA5A50029AD4A0021A5390010A5180052AD4A004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700A5948C00FFFFFF00FFFF
      FF00FFFFFF00FFFFF700FFFFF700A5948C00A5948C00A5948C00A5948C004B48
      47004B4847004B4847004B4847004B4847004B484700EFCEC600FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00EFCEC600CE947B00FFD6
      8400CE947B004B4847004B4847004B4847004B4847004B4847004B4847004B48
      4700FFF7EF00FFF7EF00FFF7EF008400000084000000FFF7DE00FFEFCE00FFF7
      DE00CE9C9C009C6331004B4847004B4847004B4847004B48470073CE7B0029DE
      5A00CEC6C6009C8C8C0052AD4A0052AD4A0073CE7B004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700A5948C00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00A5948C00EFB57B00CE947B004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700DEBDBD00FFF7EF00FFF7
      EF00F7F7E700FFF7EF00FFF7EF00FFF7EF00F7F7E700EFD6D600CE947B00CE94
      7B004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B484700FFF7EF00FFF7EF00FFF7DE00FFF7DE00FFE7BD00CE9C9C009C63
      31004B4847004B4847004B4847004B4847004B4847004B4847004B48470029DE
      5A00A5948C00847363008CAD4A0021A539004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700A5948C00A5948C00A594
      8C00A5948C00A5948C00A5948C00A5948C00B5847B004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B484700EFC6C600EFCEC600EFCE
      C600EFCEC600EFCEC600DEBDBD00EFC6C600EFCEC600DEBDBD00B5847B004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B4847004B4847004B4847004B4847004B48
      47004B4847004B4847004B4847004B484700424D3E000000000000003E000000
      2800000040000000500000000100010000000000800200000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFFFCFFF80F8001FFFFE0FFE007
      800100008003C003800100008003800180010000800380018001000080038000
      8001000080030000800100008003000080010000800300008001000080030000
      800100008003800080010000800380018001000080038001800100008003C003
      80010000E003E007FFFFFFFFF8FFF81FFFFFFFFF8003FFFF8001800180038001
      8001800180038001800180018003800180018001800380018001800180038001
      8001800180038001800180018003800180018001800380018001800180038001
      8001800180038001800180018003800180018001800180018001800180038001
      8001800180078001FFFFFFFF801FFFFFF00FFFFFFFFFFFFFE003F83F80018001
      C001E00F800180018001C0078001800180008003800180018000800380018001
      0000800380018001000080038001800100008003800180010000C00780018001
      0001E38F800180018001E38F800180018003E00F80018001C007F01F80018001
      E00FF83F80018001F01FFFFFFFFFFFFFFFFFF8018003FFFFFF9FF8018003F3FF
      FC03F8018003F1FFFC03F8018003F0FFF80080018003F00FF80080018003C003
      F800800180038001F80180018003000080038001800300008003800180030000
      801F800380030000001F800780038000001F801F8003C001803F801F8007F003
      C07F803F800FF80FE0FF807F801FFFFF00000000000000000000000000000000
      000000000000}
  end
  object XPManifest: TXPManifest
    Left = 144
    Top = 61
  end
  object tmSIPingUpdate: TTimer
    Interval = 1500
    OnTimer = tmSIPingUpdateTimer
    Left = 176
    Top = 61
  end
  object pmCopy: TPopupMenu
    OnPopup = pmCopyPopup
    Left = 112
    Top = 61
    object piCopy: TMenuItem
      Caption = 'Copy'
      OnClick = piCopyClick
    end
  end
  object tmrQueryQueueProcess: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmrQueryQueueProcessTimer
    Left = 208
    Top = 61
  end
  object tmrServerListUpdate: TTimer
    Enabled = False
    Interval = 200
    OnTimer = tmServerListUpdate
    Left = 240
    Top = 61
  end
end
