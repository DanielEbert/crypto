# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/daniel/Downloads/clion-2019.2.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/daniel/Downloads/clion-2019.2.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/daniel/K/Crypto/AK/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/daniel/K/Crypto/AK/src/cmake-build-debug

# Include any dependencies generated for this target.
include Praktikum-CMake/Lib/CMakeFiles/functions.dir/depend.make

# Include the progress variables for this target.
include Praktikum-CMake/Lib/CMakeFiles/functions.dir/progress.make

# Include the compile flags for this target's objects.
include Praktikum-CMake/Lib/CMakeFiles/functions.dir/flags.make

Praktikum-CMake/Lib/CMakeFiles/functions.dir/Sinus.cpp.o: Praktikum-CMake/Lib/CMakeFiles/functions.dir/flags.make
Praktikum-CMake/Lib/CMakeFiles/functions.dir/Sinus.cpp.o: ../Praktikum-CMake/Lib/Sinus.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Praktikum-CMake/Lib/CMakeFiles/functions.dir/Sinus.cpp.o"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/functions.dir/Sinus.cpp.o -c /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Lib/Sinus.cpp

Praktikum-CMake/Lib/CMakeFiles/functions.dir/Sinus.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/functions.dir/Sinus.cpp.i"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Lib/Sinus.cpp > CMakeFiles/functions.dir/Sinus.cpp.i

Praktikum-CMake/Lib/CMakeFiles/functions.dir/Sinus.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/functions.dir/Sinus.cpp.s"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Lib/Sinus.cpp -o CMakeFiles/functions.dir/Sinus.cpp.s

Praktikum-CMake/Lib/CMakeFiles/functions.dir/Square.cpp.o: Praktikum-CMake/Lib/CMakeFiles/functions.dir/flags.make
Praktikum-CMake/Lib/CMakeFiles/functions.dir/Square.cpp.o: ../Praktikum-CMake/Lib/Square.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Praktikum-CMake/Lib/CMakeFiles/functions.dir/Square.cpp.o"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/functions.dir/Square.cpp.o -c /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Lib/Square.cpp

Praktikum-CMake/Lib/CMakeFiles/functions.dir/Square.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/functions.dir/Square.cpp.i"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Lib/Square.cpp > CMakeFiles/functions.dir/Square.cpp.i

Praktikum-CMake/Lib/CMakeFiles/functions.dir/Square.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/functions.dir/Square.cpp.s"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Lib/Square.cpp -o CMakeFiles/functions.dir/Square.cpp.s

# Object files for target functions
functions_OBJECTS = \
"CMakeFiles/functions.dir/Sinus.cpp.o" \
"CMakeFiles/functions.dir/Square.cpp.o"

# External object files for target functions
functions_EXTERNAL_OBJECTS =

Praktikum-CMake/Lib/libfunctions.a: Praktikum-CMake/Lib/CMakeFiles/functions.dir/Sinus.cpp.o
Praktikum-CMake/Lib/libfunctions.a: Praktikum-CMake/Lib/CMakeFiles/functions.dir/Square.cpp.o
Praktikum-CMake/Lib/libfunctions.a: Praktikum-CMake/Lib/CMakeFiles/functions.dir/build.make
Praktikum-CMake/Lib/libfunctions.a: Praktikum-CMake/Lib/CMakeFiles/functions.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libfunctions.a"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && $(CMAKE_COMMAND) -P CMakeFiles/functions.dir/cmake_clean_target.cmake
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/functions.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Praktikum-CMake/Lib/CMakeFiles/functions.dir/build: Praktikum-CMake/Lib/libfunctions.a

.PHONY : Praktikum-CMake/Lib/CMakeFiles/functions.dir/build

Praktikum-CMake/Lib/CMakeFiles/functions.dir/clean:
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib && $(CMAKE_COMMAND) -P CMakeFiles/functions.dir/cmake_clean.cmake
.PHONY : Praktikum-CMake/Lib/CMakeFiles/functions.dir/clean

Praktikum-CMake/Lib/CMakeFiles/functions.dir/depend:
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniel/K/Crypto/AK/src /home/daniel/K/Crypto/AK/src/Praktikum-CMake/Lib /home/daniel/K/Crypto/AK/src/cmake-build-debug /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-CMake/Lib/CMakeFiles/functions.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Praktikum-CMake/Lib/CMakeFiles/functions.dir/depend
