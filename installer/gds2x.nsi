; NSIS installer script
Unicode True
SetCompressor /SOLID lzma

!include "MUI2.nsh"
!include "nsDialogs.nsh"
!include "LogicLib.nsh"

Name "gds2x"
!define VERSION 20080229
OutFile "gds2x-${VERSION}-install-windows-x64.exe"

!include "x64.nsh"
InstallDir "$PROGRAMFILES64\gds2x"

;--------------------------------
; Installer pages
!insertmacro MUI_PAGE_WELCOME

!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH


;--------------------------------
; Uninstaller pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
; Languages
!insertmacro MUI_LANGUAGE "English"

;--------------------------------
; Installer sections

Section "Files" SecInstall
	SectionIn RO

	SetOutPath "$INSTDIR"
	File "..\build\gds2x\Release\gds2x.exe"
	File "..\build\libgds2x\Release\gds2x.dll"
	File "..\build\process_editor\Release\gds2x_process_editor.exe"
	
	File "..\build\gds2x\Release\bz2.dll"
	File "..\build\gds2x\Release\lib3mf.dll"
	File "..\build\gds2x\Release\zip.dll"
	File "..\build\gds2x\Release\zlib1.dll"

	File "..\build\process_editor\Release\jpeg62.dll"
	File "..\build\process_editor\Release\liblzma.dll"
	File "..\build\process_editor\Release\libpng16.dll"
	File "..\build\process_editor\Release\pcre2-16.dll"
	File "..\build\process_editor\Release\tiff.dll"
	File "..\build\process_editor\Release\wxbase32u_vc_x64_custom.dll"
	File "..\build\process_editor\Release\wxmsw32u_core_vc_x64_custom.dll"

	File "..\AUTHORS"
	File "..\changes.txt"
	File "..\config.txt"
	File "..\COPYING"
	File "..\example_2D.png"
	File "..\example_3D.png"
	File "..\example.cmd"
	File "..\example_config.txt"
	File "..\example.gds"
	File "..\example_process.txt"
	File "..\fullexample.gds"
	File "..\process.txt"
	File "..\readme.txt"

	WriteUninstaller "$INSTDIR\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "DisplayName" "gds2x"
	#WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "DisplayIcon" "$INSTDIR\gds2x.ico"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "QuietUninstallString" "$\"$INSTDIR\Uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "HelpLink" "https://atchoo.org/gds2pov/"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "URLInfoAbout" "https://atchoo.org/gds2pov/"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "DisplayVersion" "${VERSION}"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "NoModify" "1"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "NoRepair" "1"
SectionEnd

Section "Visual Studio Runtime"
  SetOutPath "$INSTDIR"
  File "VC_redist.x64.exe"
  ExecWait '"$INSTDIR\VC_redist.x64.exe" /quiet /norestart'
  Delete "$INSTDIR\VC_redist.x64.exe"
SectionEnd

Section "Uninstall"
	Delete "$INSTDIR\gds2x.exe"
	Delete "$INSTDIR\gds2x_process_editor.exe"
	Delete "$INSTDIR\libgds2x.dll"

	Delete "$INSTDIR\bz2.dll"
	Delete "$INSTDIR\lib3mf.dll"
	Delete "$INSTDIR\zip.dll"
	Delete "$INSTDIR\zlib1.dll"

	Delete "$INSTDIR\jpeg62.dll"
	Delete "$INSTDIR\liblzma.dll"
	Delete "$INSTDIR\libpng16.dll"
	Delete "$INSTDIR\pcre2-16.dll"
	Delete "$INSTDIR\tiff.dll"
	Delete "$INSTDIR\wxbase32u_vc_x64_custom.dll"
	Delete "$INSTDIR\wxmsw32u_core_vc_x64_custom.dll"

	Delete "$INSTDIR\AUTHORS"
	Delete "$INSTDIR\changes.txt"
	Delete "$INSTDIR\config.txt"
	Delete "$INSTDIR\COPYING"
	Delete "$INSTDIR\example_2D.png"
	Delete "$INSTDIR\example_3D.png"
	Delete "$INSTDIR\example.cmd"
	Delete "$INSTDIR\example_config.txt"
	Delete "$INSTDIR\example.gds"
	Delete "$INSTDIR\example_process.txt"
	Delete "$INSTDIR\fullexample.gds"
	Delete "$INSTDIR\process.txt"
	Delete "$INSTDIR\readme.txt"

	Delete "$INSTDIR\Uninstall.exe"
	RMDir "$INSTDIR"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x"
SectionEnd

LangString DESC_SecInstall ${LANG_ENGLISH} "The main installation."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SecInstall} $(DESC_SecInstall)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecService} $(DESC_SecService)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
