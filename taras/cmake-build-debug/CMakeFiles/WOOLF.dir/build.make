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
CMAKE_SOURCE_DIR = /Users/tvoronyu/Desktop/doom_begin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tvoronyu/Desktop/doom_begin/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/WOOLF.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/WOOLF.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/WOOLF.dir/flags.make

CMakeFiles/WOOLF.dir/src/main.c.o: CMakeFiles/WOOLF.dir/flags.make
CMakeFiles/WOOLF.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tvoronyu/Desktop/doom_begin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/WOOLF.dir/src/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WOOLF.dir/src/main.c.o   -c /Users/tvoronyu/Desktop/doom_begin/src/main.c

CMakeFiles/WOOLF.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WOOLF.dir/src/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/tvoronyu/Desktop/doom_begin/src/main.c > CMakeFiles/WOOLF.dir/src/main.c.i

CMakeFiles/WOOLF.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WOOLF.dir/src/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/tvoronyu/Desktop/doom_begin/src/main.c -o CMakeFiles/WOOLF.dir/src/main.c.s

# Object files for target WOOLF
WOOLF_OBJECTS = \
"CMakeFiles/WOOLF.dir/src/main.c.o"

# External object files for target WOOLF
WOOLF_EXTERNAL_OBJECTS =

WOOLF: CMakeFiles/WOOLF.dir/src/main.c.o
WOOLF: CMakeFiles/WOOLF.dir/build.make
WOOLF: CMakeFiles/WOOLF.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/tvoronyu/Desktop/doom_begin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable WOOLF"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/WOOLF.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/WOOLF.dir/build: WOOLF

.PHONY : CMakeFiles/WOOLF.dir/build

CMakeFiles/WOOLF.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/WOOLF.dir/cmake_clean.cmake
.PHONY : CMakeFiles/WOOLF.dir/clean

CMakeFiles/WOOLF.dir/depend:
	cd /Users/tvoronyu/Desktop/doom_begin/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tvoronyu/Desktop/doom_begin /Users/tvoronyu/Desktop/doom_begin /Users/tvoronyu/Desktop/doom_begin/cmake-build-debug /Users/tvoronyu/Desktop/doom_begin/cmake-build-debug /Users/tvoronyu/Desktop/doom_begin/cmake-build-debug/CMakeFiles/WOOLF.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/WOOLF.dir/depend

