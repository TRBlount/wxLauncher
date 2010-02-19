# Setup the packer

IF(WIN32) # and WIN64
  # find the runtime files
  IF(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(VC9_RUNTIME_LOCATIONS "C:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/redist/Debug_NonRedist/x86/Microsoft.VC90.DebugCRT" "C:/Program Files/Microsoft Visual Studio 9.0/VC/redist/Debug_NonRedist/x86/Microsoft.VC90.DebugCRT")
    FIND_FILE(VC9_RUNTIME_msvcm90 msvcm90d.dll 
      PATHS ${VC9_RUNTIME_LOCATIONS}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${VC9_RUNTIME_msvcm90} DESTINATION bin)
    FIND_FILE(VC9_RUNTIME_msvcp90 msvcp90d.dll 
      PATHS ${VC9_RUNTIME_LOCATIONS}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${VC9_RUNTIME_msvcp90} DESTINATION bin)
    FIND_FILE(VC9_RUNTIME_msvcr90 msvcr90d.dll 
      PATHS ${VC9_RUNTIME_LOCATIONS}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${VC9_RUNTIME_msvcr90} DESTINATION bin)
    FIND_FILE(VC9_RUNTIME_manifest Microsoft.VC90.DebugCRT.manifest 
      PATHS ${VC9_RUNTIME_LOCATIONS}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${VC9_RUNTIME_manifest} DESTINATION bin)
    FIND_FILE(wxLauncher_pdb wxlauncher.pdb PATHS ${CMAKE_CURRENT_BINARY_DIR})
    INSTALL(FILES ${wxLauncher_pdb} DESTINATION bin)
    FIND_FILE(registry_helper_pdb registry_helper.pdb PATHS ${CMAKE_CURRENT_BINARY_DIR}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${registry_helper_pdb} DESTINATION bin)
    
  ELSE()
    SET(VC9_RUNTIME_LOCATIONS "C:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/redist/x86/Microsoft.VC90.CRT" "C:/Program Files/Microsoft Visual Studio 9.0/VC/redist/x86/Microsoft.VC90.CRT")
    FIND_FILE(VC9_RUNTIME_msvcm90 msvcm90.dll 
      PATHS ${VC9_RUNTIME_LOCATIONS}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${VC9_RUNTIME_msvcm90} DESTINATION bin)
    FIND_FILE(VC9_RUNTIME_msvcp90 msvcp90.dll 
      PATHS ${VC9_RUNTIME_LOCATIONS}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${VC9_RUNTIME_msvcp90} DESTINATION bin)
    FIND_FILE(VC9_RUNTIME_msvcr90 msvcr90.dll 
      PATHS ${VC9_RUNTIME_LOCATIONS}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${VC9_RUNTIME_msvcr90} DESTINATION bin)
    FIND_FILE(VC9_RUNTIME_manifest Microsoft.VC90.CRT.manifest 
      PATHS ${VC9_RUNTIME_LOCATIONS}
      NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH)
    INSTALL(FILES ${VC9_RUNTIME_manifest} DESTINATION bin)
  ENDIF()
ENDIF()
  
SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
SET(CPACK_PACKAGE_NAME "wxLauncher")
SET(CPACK_PACKAGE_VENDOR wxLauncher Team)
SET(CPACK_PACKAGE_INSTALL_DIRECTORY wxLauncher)
SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/License.txt)
SET(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/ReadMe.txt)
SET(CPACK_RESOURCE_FILE_WELCOME ${PROJECT_SOURCE_DIR}/ReadMe.txt)
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY wxLauncher)
# the \\\\ is because NSIS does not handle unix paths correctly - http://www.batchmake.org/Wiki/CMake:Packaging_With_CPack
SET(CPACK_NSIS_MUI_ICON ${PROJECT_SOURCE_DIR}/platform/win32\\\\wxlauncher.ico)
SET(CPACK_NSIS_MUI_UNIICON ${PROJECT_SOURCE_DIR}/platform/win32\\\\wxlauncher.ico)
# the header image *must* be a bitmap
SET(CPACK_PACKAGE_ICON ${PROJECT_SOURCE_DIR}/platform/win32\\\\installer_header.bmp)
SET(CPACK_NSIS_HELP_LINK http::\\\\\\\\wxlauncher.googlecode.com)
SET(CPACK_NSIS_URL_INFO_ABOUT http::\\\\\\\\www.hard-light.net)
IF(DEVELOPMENT_MODE)
  SET(CPACK_PACKAGE_FILE_NAME wxlaucher-development-mode-no-distribution)
ELSE()
  IF(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(CPACK_PACKAGE_FILE_NAME wxlauncher-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-debug-withpdbs)
  ELSEIF(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    SET(CPACK_PACKAGE_FILE_NAME wxlauncher-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-withpdbs)
  ELSE()
    SET(CPACK_PACKAGE_FILE_NAME wxlauncher-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
  ENDIF()
ENDIF()

if( WIN32 AND NOT UNIX )
    SET(CPACK_PACKAGE_EXECUTABLES "wxLauncher" "wxLauncher for the SCP")
endif()

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Iss Mneur <iss.mneur@telus.net>")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Launcher and installer for the FreeSpace SCP")
SET(CPACK_PACKAGE_DESCRIPTION "Launcher and installer for the FreeSpace 2 Open Source Code Project")
SET(CPACK_PACKAGE_DEPENDS "libwxgtk2.8 libopenal1")
SET(CPACK_DEBIAN_PACKAGE_SETION "Games")
if(WIN32)
  SET(CPACK_BINARY_NSIS "ON")
  OPTION(BUILD_BINARY_ZIP "Generate a binary zip. Requires 7z, WinZip, or Cygwin Zip installed" OFF)
  IF(BUILD_BINARY_ZIP)
    SET(CPACK_BINARY_ZIP "ON")
  ELSE()
    SET(CPACK_BINARY_ZIP "OFF")
  ENDIF()
else()
  SET(CPACK_BINARY_DEB "ON")
  find_program(rpmbuild_exists rpmbuild)
  if(rpmbuild_exists)
    SET(CPACK_BINARY_RPM "ON")
  else()
    SET(CPACK_BINARY_RPM "OFF")
    MESSAGE(STATUS "rpmbuild does not exist, will not build RPM")    
  endif()
  SET(CPACK_BINARY_STGZ "OFF")
  SET(CPACK_BINARY_TZ "OFF")
  SET(CPACK_BINARY_TGZ "OFF")
endif()

include(CPack)
if(WIN32)
  install(TARGETS registry_helper RUNTIME DESTINATION bin)
  install(TARGETS wxlauncher RUNTIME DESTINATION bin)
else()  
  install(TARGETS wxlauncher 
	RUNTIME DESTINATION local/bin
	BUNDLE DESTINATION bin)
endif()
if(WIN32)
install(DIRECTORY resources/ DESTINATION resources)
install(FILES ${helphtblocation} DESTINATION .)
else()
install(DIRECTORY resources/ DESTINATION local/share/wxlauncher)
install(FILES ${helphtblocation} DESTINATION local/share/wxlauncher)
endif()
