# PRISM-Dilate CMakeLists.txt

cmake_minimum_required(VERSION 3.12)

set(APP_NAME PRISM)

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.

project(${APP_NAME}_APP VERSION 0.6)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

set_directory_properties(PROPERTIES 
JUCE_COMPANY_COPYRIGHT "GPLv3"
JUCE_COMPANY_NAME "Rodney DuPlessis"
JUCE_COMPANY_WEBSITE "www.rodneyduplessis.com"
JUCE_COMPANY_EMAIL "rodney@rodneyduplessis.com")


# If your app depends the VST2 SDK, perhaps to host VST2 plugins, CMake needs to be told where
# to find the SDK on your system. This setup should be done before calling `juce_add_gui_app`.

# juce_set_vst2_sdk_path(...)

# # GUI APP (PRISM) =========================================================================================

# # Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.
# juce_add_gui_app(${APP_NAME}
#     # VERSION ...                       # Set this if the app version is different to the project version
#     # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
#     # ICON_SMALL ...
#     PRODUCT_NAME ${APP_NAME})         # The name of the final executable, which can differ from the target name

# # `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# # into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# # this header will be automatically added to the target. The main function of the JuceHeader is to
# # include all your JUCE module headers; if you're happy to include module headers directly, you
# # probably don't need to call this.

# # juce_generate_juce_header(${APP_NAME})

# # `target_sources` adds source files to a target. We pass the target that needs the sources as the
# # first argument, then a visibility parameter for the sources (PRIVATE is normally best practice,
# # although it doesn't really affect executable targets). Finally, we supply a list of source files
# # that will be built into the target. This is a standard CMake command.

# # GUI APP (standalone) version
# target_sources(${APP_NAME} PRIVATE
#     src/Main.cpp
#     src/MainComponent.cpp
#     src/customProcessors/AudioSettingsComponent.cpp
#     src/customProcessors/DilateProcessor.cpp
#     src/customProcessors/SpectrogramComponent.cpp
#     )

#     # `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# # project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# # of compile definitions to switch certain features on/off, so if there's a particular feature you
# # need that's not on by default, check the module header for the correct flag to set here. These
# # definitions will be visible both to your code, and also the JUCE module code, so for new
# # definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

# target_compile_definitions(${APP_NAME} PUBLIC
# # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
# JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
# JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
# JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${APP_NAME},JUCE_PROJECT_NAME>"
# JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${APP_NAME},JUCE_VERSION>"
# JUCE_DISPLAY_SPLASH_SCREEN=0
# JUCE_JACK=1
# JUCE_DSP_USE_SHARED_FFTW=1    
# JUCE_VST3_CAN_REPLACE_VST2=0                # If true requires vst2 SDK which I don't have so it has to be false    
# )

# # If your target needs extra binary assets, you can add them here. The first argument is the name of
# # a new static library target that will include all the binary resources. There is an optional
# # `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# # the SOURCES argument should be followed by a list of source files that should be built into the
# # static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# # Conversion to binary-data will happen when your target is built.

# # juce_add_binary_data(GuiAppData SOURCES ...)

# # `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# # we're linking our executable target to the `juce::juce_gui_extra` module. Inter-module
# # dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# # linked automatically. If we'd generated a binary data target above, we would need to link to it
# # here too. This is a standard CMake command.

# target_link_libraries(${APP_NAME} PRIVATE
# # GuiAppData            # If we'd created a binary data target, we'd link to it here
# juce::juce_gui_basics
# juce::juce_audio_devices
# juce::juce_audio_utils
# juce::juce_gui_extra
# juce::juce_dsp)

# # binaries are put into the ./bin directory by default
# set_target_properties(${APP_NAME} PROPERTIES
# BUILD_WITH_INSTALL_RPATH 1
# INSTALL_NAME_DIR "@rpath"
# CXX_STANDARD 14
# CXX_STANDARD_REQUIRED ON
# RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
# RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/bin/Debug
# RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/bin/Release
# )

# Plugin (Dilate) ==========================================================================================

set(PLUGIN_NAME ${APP_NAME}-Dilate)

juce_add_plugin(${PLUGIN_NAME}
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    IS_SYNTH FALSE                              # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE                      # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    COPY_PLUGIN_AFTER_BUILD TRUE               # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Dupl               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Dil8                            # A unique four-character plugin id with at least one upper-case character
    FORMATS VST3 Standalone                     # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    VST3_CATEGORIES "Fx, Pitch Shift"
    DESCRIPTION "Plugin for performing Spectral Dilation (DuPlessis, forthcoming)"
    PRODUCT_NAME ${PLUGIN_NAME})           # The name of the final executable, which can differ from the target name

target_sources(${PLUGIN_NAME} PRIVATE
    src/customProcessors/DilateProcessor.cpp
    )

target_compile_definitions(${PLUGIN_NAME} PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PLUGIN_NAME},JUCE_PROJECT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PLUGIN_NAME},JUCE_VERSION>"
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_JACK=1
    JUCE_DSP_USE_SHARED_FFTW=1    
    JUCE_VST3_CAN_REPLACE_VST2=0                # If true requires vst2 SDK which I don't have so it has to be false
    )

target_link_libraries(${PLUGIN_NAME} PRIVATE
    # GuiAppData            # If we'd created a binary data target, we'd link to it here
    juce::juce_gui_basics
    juce::juce_audio_devices
    juce::juce_audio_utils
    juce::juce_gui_extra
    juce::juce_dsp)

# binaries are put into the ./bin directory by default
set_target_properties(${PLUGIN_NAME} PROPERTIES
BUILD_WITH_INSTALL_RPATH 1
INSTALL_NAME_DIR "@rpath"
CXX_STANDARD 14
CXX_STANDARD_REQUIRED ON
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/bin/Debug
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/bin/Release
)

# Plugin (mask) ==========================================================================================

set(PLUGIN_NAME ${APP_NAME}-Mask)

juce_add_plugin(${PLUGIN_NAME}
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    IS_SYNTH FALSE                              # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE                      # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    COPY_PLUGIN_AFTER_BUILD TRUE               # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Dupl               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Mask                            # A unique four-character plugin id with at least one upper-case character
    FORMATS VST3 Standalone                     # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    VST3_CATEGORIES "Fx, Filter"
    DESCRIPTION "Plugin for performing Spectral Masking (DuPlessis, forthcoming)"
    PRODUCT_NAME ${PLUGIN_NAME})           # The name of the final executable, which can differ from the target name

target_sources(${PLUGIN_NAME} PRIVATE
    src/customProcessors/MaskProcessor.cpp
    )

target_compile_definitions(${PLUGIN_NAME} PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PLUGIN_NAME},JUCE_PROJECT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PLUGIN_NAME},JUCE_VERSION>"
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_JACK=1
    JUCE_DSP_USE_SHARED_FFTW=1    
    JUCE_VST3_CAN_REPLACE_VST2=0                # If true requires vst2 SDK which I don't have so it has to be false
    )

target_link_libraries(${PLUGIN_NAME} PRIVATE
    # GuiAppData            # If we'd created a binary data target, we'd link to it here
    juce::juce_gui_basics
    juce::juce_audio_devices
    juce::juce_audio_utils
    juce::juce_gui_extra
    juce::juce_dsp
    juce::juce_osc)

# binaries are put into the ./bin directory by default
set_target_properties(${PLUGIN_NAME} PROPERTIES
BUILD_WITH_INSTALL_RPATH 1
INSTALL_NAME_DIR "@rpath"
CXX_STANDARD 14
CXX_STANDARD_REQUIRED ON
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/bin/Debug
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/bin/Release
)