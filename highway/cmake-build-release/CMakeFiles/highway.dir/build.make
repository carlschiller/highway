# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/highway.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/highway.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/highway.dir/flags.make

CMakeFiles/highway.dir/main.cpp.o: CMakeFiles/highway.dir/flags.make
CMakeFiles/highway.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/highway.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/highway.dir/main.cpp.o -c /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/main.cpp

CMakeFiles/highway.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/highway.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/main.cpp > CMakeFiles/highway.dir/main.cpp.i

CMakeFiles/highway.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/highway.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/main.cpp -o CMakeFiles/highway.dir/main.cpp.s

CMakeFiles/highway.dir/traffic.cpp.o: CMakeFiles/highway.dir/flags.make
CMakeFiles/highway.dir/traffic.cpp.o: ../traffic.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/highway.dir/traffic.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/highway.dir/traffic.cpp.o -c /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/traffic.cpp

CMakeFiles/highway.dir/traffic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/highway.dir/traffic.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/traffic.cpp > CMakeFiles/highway.dir/traffic.cpp.i

CMakeFiles/highway.dir/traffic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/highway.dir/traffic.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/traffic.cpp -o CMakeFiles/highway.dir/traffic.cpp.s

CMakeFiles/highway.dir/window.cpp.o: CMakeFiles/highway.dir/flags.make
CMakeFiles/highway.dir/window.cpp.o: ../window.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/highway.dir/window.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/highway.dir/window.cpp.o -c /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/window.cpp

CMakeFiles/highway.dir/window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/highway.dir/window.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/window.cpp > CMakeFiles/highway.dir/window.cpp.i

CMakeFiles/highway.dir/window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/highway.dir/window.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/window.cpp -o CMakeFiles/highway.dir/window.cpp.s

# Object files for target highway
highway_OBJECTS = \
"CMakeFiles/highway.dir/main.cpp.o" \
"CMakeFiles/highway.dir/traffic.cpp.o" \
"CMakeFiles/highway.dir/window.cpp.o"

# External object files for target highway
highway_EXTERNAL_OBJECTS =

highway: CMakeFiles/highway.dir/main.cpp.o
highway: CMakeFiles/highway.dir/traffic.cpp.o
highway: CMakeFiles/highway.dir/window.cpp.o
highway: CMakeFiles/highway.dir/build.make
highway: /usr/local/lib/libsfml-system.dylib
highway: /usr/local/lib/libsfml-window.dylib
highway: /usr/local/lib/libsfml-graphics.dylib
highway: /usr/local/lib/libsfml-network.dylib
highway: /usr/local/lib/libsfml-audio.dylib
highway: CMakeFiles/highway.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable highway"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/highway.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/highway.dir/build: highway

.PHONY : CMakeFiles/highway.dir/build

CMakeFiles/highway.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/highway.dir/cmake_clean.cmake
.PHONY : CMakeFiles/highway.dir/clean

CMakeFiles/highway.dir/depend:
	cd /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release /Users/carlschiller/Documents/KTH/Anteckningar/year3/Simulering/Project/highway/cmake-build-release/CMakeFiles/highway.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/highway.dir/depend

