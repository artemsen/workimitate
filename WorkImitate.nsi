!define PRODUCT_NAME            "WorkImitate"
!define PRODUCT_VERSION         "1.1"
!define PRODUCT_WEB_SITE        "http://workimitate.sourceforge.net"
!define PRODUCT_SET_KEY         "Software\${PRODUCT_NAME}"
!define PRODUCT_SET_ROOT_KEY    "HKCU"
!define PRODUCT_UNINST_KEY      "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI2.nsh"

; MUI Settings
!define MUI_ICON                "Resources\WorkImitate.ico"
!define MUI_UNICON              "Resources\WorkImitate.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE       "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}-install.exe"
InstallDir "$SYSDIR"
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "Release\WorkImitate.scr"
SectionEnd

Section -Post
  ;Set WorkImitate as default screen saver
  WriteRegStr "HKCU" "Control Panel\Desktop" "SCRNSAVE.EXE" "WORKIM~1.SCR"

  ;Start display properties applet
  Exec 'Rundll32.exe shell32.dll,Control_RunDLL Desk.cpl,@0,1'
  
  ;Unintall information
  WriteUninstaller "$INSTDIR\WorkImitateUninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\WorkImitateUninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) has been successful uninstalled."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) screen saver?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\WorkImitateUninst.exe"
  Delete "$INSTDIR\WorkImitate.scr"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey ${PRODUCT_SET_ROOT_KEY} "${PRODUCT_SET_KEY}"

  SetAutoClose true
SectionEnd
