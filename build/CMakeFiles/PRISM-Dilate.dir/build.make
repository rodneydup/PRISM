# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rondo/Software/JUCE/PRISM-Dilate

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rondo/Software/JUCE/PRISM-Dilate/build

# Include any dependencies generated for this target.
include CMakeFiles/PRISM-Dilate.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/PRISM-Dilate.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PRISM-Dilate.dir/flags.make

CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.o: ../src/Main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/src/Main.cpp

CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/src/Main.cpp > CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.i

CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/src/Main.cpp -o CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.s

CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.o: ../src/MainComponent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/src/MainComponent.cpp

CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/src/MainComponent.cpp > CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.i

CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/src/MainComponent.cpp -o CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.s

CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.o: ../src/audioSettingsComponent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/src/audioSettingsComponent.cpp

CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/src/audioSettingsComponent.cpp > CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.i

CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/src/audioSettingsComponent.cpp -o CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o: ../JUCE/modules/juce_gui_basics/juce_gui_basics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.o: ../JUCE/modules/juce_graphics/juce_graphics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_graphics/juce_graphics.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_graphics/juce_graphics.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_graphics/juce_graphics.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.o: ../JUCE/modules/juce_events/juce_events.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_events/juce_events.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_events/juce_events.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_events/juce_events.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.o: ../JUCE/modules/juce_core/juce_core.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_core/juce_core.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_core/juce_core.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_core/juce_core.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o: ../JUCE/modules/juce_data_structures/juce_data_structures.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_data_structures/juce_data_structures.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_data_structures/juce_data_structures.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_data_structures/juce_data_structures.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o: ../JUCE/modules/juce_audio_devices/juce_audio_devices.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o: ../JUCE/modules/juce_audio_basics/juce_audio_basics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o: ../JUCE/modules/juce_audio_utils/juce_audio_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o: ../JUCE/modules/juce_audio_processors/juce_audio_processors.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o: ../JUCE/modules/juce_gui_extra/juce_gui_extra.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.s

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o: CMakeFiles/PRISM-Dilate.dir/flags.make
CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o: ../JUCE/modules/juce_audio_formats/juce_audio_formats.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o -c /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp > CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.i

CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rondo/Software/JUCE/PRISM-Dilate/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp -o CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.s

# Object files for target PRISM-Dilate
PRISM__Dilate_OBJECTS = \
"CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o" \
"CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o"

# External object files for target PRISM-Dilate
PRISM__Dilate_EXTERNAL_OBJECTS =

../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/src/Main.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/src/MainComponent.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/src/audioSettingsComponent.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_graphics/juce_graphics.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_events/juce_events.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_core/juce_core.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_data_structures/juce_data_structures.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_utils/juce_audio_utils.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp.o
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/build.make
../bin/PRISM-Dilate: pedal/lib/libpedal.a
../bin/PRISM-Dilate: /usr/lib/x86_64-linux-gnu/libasound.so
../bin/PRISM-Dilate: /usr/lib/x86_64-linux-gnu/libfreetype.so
../bin/PRISM-Dilate: CMakeFiles/PRISM-Dilate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX executable ../bin/PRISM-Dilate"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PRISM-Dilate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PRISM-Dilate.dir/build: ../bin/PRISM-Dilate

.PHONY : CMakeFiles/PRISM-Dilate.dir/build

CMakeFiles/PRISM-Dilate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PRISM-Dilate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PRISM-Dilate.dir/clean

CMakeFiles/PRISM-Dilate.dir/depend:
	cd /home/rondo/Software/JUCE/PRISM-Dilate/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rondo/Software/JUCE/PRISM-Dilate /home/rondo/Software/JUCE/PRISM-Dilate /home/rondo/Software/JUCE/PRISM-Dilate/build /home/rondo/Software/JUCE/PRISM-Dilate/build /home/rondo/Software/JUCE/PRISM-Dilate/build/CMakeFiles/PRISM-Dilate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PRISM-Dilate.dir/depend

