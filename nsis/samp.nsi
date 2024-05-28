;--------------------------------
;Include Modern UI

!include "MUI.nsh"

;--------------------------------
;General

!define VERSION "0.3.7-R5"

Name "San Andreas Multiplayer ${VERSION}"
OutFile "sa-mp-${VERSION}-install.exe"
AutoCloseWindow true
DirText "Please select your Grand Theft Auto: San Andreas directory:"
InstallDir "$PROGRAMFILES\Rockstar Games\GTA San Andreas\"
InstallDirRegKey HKLM "Software\Rockstar Games\GTA San Andreas\Installation" ExePath

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

;--------------------------------
;Pages


!define MUI_WELCOMEPAGE_TITLE "Welcome!"
!define MUI_FINISHPAGE_TITLE "Installation Complete."

!insertmacro MUI_PAGE_LICENSE "samp-license.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
 
!insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Functions

Function .onVerifyInstDir
	IfFileExists $INSTDIR\gta_sa.exe GoodGood
		Abort
	GoodGood:
FunctionEnd

;--------------------------------
;Installer Sections

Section ""
	SetOutPath $INSTDIR
	File samp.exe
	File samp.dll
	File bass.dll
	File samp.saa
	File rcon.exe
	File samp_debug.exe
	File sampgui.png
	File mouse.png
	File gtaweap3.ttf
	File sampaux3.ttf
	File "samp-license.txt"

	Delete $INSTDIR\models\samp.img
	Delete $INSTDIR\models\sampcol.img
	Delete $INSTDIR\models\samp.ide
	Delete $INSTDIR\models\samp.ipl

	SetOutPath $INSTDIR\SAMP
	File SAMP.img
	File SAMPCOL.img
	File SAMP.ide
	File SAMP.ipl
	File samaps.txd
	File blanktex.txd
	SetOverwrite off
	File custom.img
	File CUSTOM.ide

	SetOutPath $SYSDIR
	SetOverwrite on
	File "c:\windows\system32\d3dx9_25.dll"

	SetOutPath $INSTDIR
	WriteUninstaller SAMPUninstall.exe

	CreateDirectory "$SMPROGRAMS\San Andreas Multiplayer"
	CreateShortCut "$SMPROGRAMS\San Andreas Multiplayer\San Andreas Multiplayer.lnk" $INSTDIR\samp.exe
	CreateShortCut "$SMPROGRAMS\San Andreas Multiplayer\Uninstall.lnk" $INSTDIR\SAMPUninstall.exe

	WriteRegStr HKCR samp "" "San Andreas Multiplayer"
	WriteRegStr HKCR samp "Url Protocol" ""
	WriteRegStr HKCR samp\shell\open\command "" "$\"$INSTDIR\samp.exe$\" $\"%1$\""
	WriteRegStr HKCU Software\SAMP gta_sa_exe $INSTDIR\gta_sa.exe

	CreateDirectory "$DOCUMENTS\GTA San Andreas User Files"
	CreateDirectory "$DOCUMENTS\GTA San Andreas User Files\SAMP"

	CopyFiles $INSTDIR\userdata.dat "$DOCUMENTS\GTA San Andreas User Files\SAMP"
	Delete $INSTDIR\userdata.dat
SectionEnd

Section "Uninstall"
	Delete $INSTDIR\samp.exe
	Delete $INSTDIR\samp.dll
	Delete $INSTDIR\bass.dll
	Delete $INSTDIR\samp.saa
	Delete $INSTDIR\rcon.exe
	Delete $INSTDIR\samp_debug.exe
	Delete $INSTDIR\SAMPUninstall.exe
	Delete $INSTDIR\sampgui.png
	Delete $INSTDIR\mouse.png
	Delete $INSTDIR\gtaweap3.ttf
	Delete $INSTDIR\sampaux3.ttf
	Delete $INSTDIR\samp-license.txt
	Delete $INSTDIR\SAMP\samp.img
	Delete $INSTDIR\SAMP\sampcol.img
	Delete $INSTDIR\SAMP\custom.img
	Delete $INSTDIR\SAMP\samp.ide
	Delete $INSTDIR\SAMP\samp.ipl
	Delete $INSTDIR\SAMP\custom.ide
	Delete $INSTDIR\SAMP\samaps.txd
	Delete $INSTDIR\SAMP\blanktex.txd
	RMDir $INSTDIR\SAMP

	Delete "$SMPROGRAMS\San Andreas Multiplayer\San Andreas Multiplayer.lnk"
	Delete "$SMPROGRAMS\San Andreas Multiplayer\Uninstall.lnk"
	RMDir "$SMPROGRAMS\San Andreas Multiplayer"
SectionEnd
