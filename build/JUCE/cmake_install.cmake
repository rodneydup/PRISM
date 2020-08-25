# Install script for directory: /home/rondo/Software/JUCE/PRISM-Dilate/JUCE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-6.0.1" TYPE FILE FILES
    "/home/rondo/Software/JUCE/PRISM-Dilate/build/JUCE/JUCEConfigVersion.cmake"
    "/home/rondo/Software/JUCE/PRISM-Dilate/build/JUCE/JUCEConfig.cmake"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/LaunchScreen.storyboard"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/PIPAudioProcessor.cpp.in"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/PIPComponent.cpp.in"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/PIPConsole.cpp.in"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/RecentFilesMenuTemplate.nib"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/UnityPluginGUIScript.cs.in"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/copyDir.cmake"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/JUCEHelperTargets.cmake"
    "/home/rondo/Software/JUCE/PRISM-Dilate/JUCE/extras/Build/CMake/JUCEUtils.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/rondo/Software/JUCE/PRISM-Dilate/build/JUCE/modules/cmake_install.cmake")
  include("/home/rondo/Software/JUCE/PRISM-Dilate/build/JUCE/extras/Build/cmake_install.cmake")

endif()

