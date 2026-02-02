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

	File "..\build\gds2x\Release\*.dll"
	File "..\build\process_editor\Release\*.dll"

	File "..\AUTHORS"
	File "..\changes.txt"
	File "..\COPYING"
	File "..\installer\gds2x.ico"
	File "..\README.md"

	SetOutPath "$INSTDIR\examples"
	File "..\examples\config.txt"
	File "..\examples\example_2D.png"
	File "..\examples\example_3D.png"
	File "..\examples\example.cmd"
	File "..\examples\example_config.txt"
	File "..\examples\example.gds"
	File "..\examples\example_process.txt"
	File "..\examples\fullexample.gds"
	File "..\examples\process.txt"

	WriteUninstaller "$INSTDIR\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "DisplayName" "gds2x"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x" "DisplayIcon" "$INSTDIR\gds2x.ico"
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
	Delete "$INSTDIR\COPYING"
	Delete "$INSTDIR\README.md"
	Delete "$INSTDIR\examples\config.txt"
	Delete "$INSTDIR\examples\example_2D.png"
	Delete "$INSTDIR\examples\example_3D.png"
	Delete "$INSTDIR\examples\example.cmd"
	Delete "$INSTDIR\examples\example_config.txt"
	Delete "$INSTDIR\examples\example.gds"
	Delete "$INSTDIR\examples\example_process.txt"
	Delete "$INSTDIR\examples\fullexample.gds"
	Delete "$INSTDIR\examples\process.txt"

	Delete "$INSTDIR\Uninstall.exe"
	RMDir "$INSTDIR"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gds2x"
SectionEnd

LangString DESC_SecInstall ${LANG_ENGLISH} "The main installation."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SecInstall} $(DESC_SecInstall)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecService} $(DESC_SecService)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
