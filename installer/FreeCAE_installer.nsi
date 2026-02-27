; FreeCAE NSIS Installer Script
; Pack path: build\Desktop_Qt_6_8_3_MSVC2022_64bit-Release\bin

!include "MUI2.nsh"
!include "FileFunc.nsh"

;--------------------------------
; Basic config
;--------------------------------
!define PRODUCT_NAME "FreeCAE"
!define PRODUCT_VERSION "0.0.1"
!define PRODUCT_PUBLISHER "FC"
!define PRODUCT_WEB_SITE "https://github.com"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; Source path relative to installer\ dir
!define SOURCE_BIN_DIR "..\build\Desktop_Qt_6_8_3_MSVC2022_64bit-Release\bin"
!define APP_ICON "..\src\APP\icon.ico"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\Output\FreeCAE_Setup_${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES64\${PRODUCT_NAME}"
InstallDirRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString"
RequestExecutionLevel admin

; Icon for the generated installer exe (shown in Explorer)
Icon "${APP_ICON}"

; Version info (2052 = LANG_CHINESE_SIMPLIFIED)
VIProductVersion "${PRODUCT_VERSION}.0"
VIAddVersionKey /LANG=2052 "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey /LANG=2052 "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey /LANG=2052 "CompanyName" "${PRODUCT_PUBLISHER}"
VIAddVersionKey /LANG=2052 "LegalCopyright" "Copyright (C) 2025 Kinvy"
VIAddVersionKey /LANG=2052 "FileDescription" "${PRODUCT_NAME} Installer"
VIAddVersionKey /LANG=2052 "FileVersion" "${PRODUCT_VERSION}"

;--------------------------------
; UI config
;--------------------------------
!define MUI_ABORTWARNING
!define MUI_ICON "${APP_ICON}"
!define MUI_UNICON "${APP_ICON}"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
; Install section
;--------------------------------
Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  File /r "${SOURCE_BIN_DIR}\*.*"
  File "${APP_ICON}"

  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\FreeCAE.exe" "" "$INSTDIR\FreeCAE.exe" 0

  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\FreeCAE.exe" "" "$INSTDIR\FreeCAE.exe" 0
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall ${PRODUCT_NAME}.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
SectionEnd

;--------------------------------
; Post install
;--------------------------------
Section -Post
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\FreeCAE.exe"
SectionEnd

;--------------------------------
; Uninstall section
;--------------------------------
Section "Uninstall"
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall ${PRODUCT_NAME}.lnk"
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

  RMDir /r "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
SectionEnd
