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
include Praktikum-DES/CMakeFiles/des.dir/depend.make

# Include the progress variables for this target.
include Praktikum-DES/CMakeFiles/des.dir/progress.make

# Include the compile flags for this target's objects.
include Praktikum-DES/CMakeFiles/des.dir/flags.make

../Praktikum-DES/des-getopt.h: ../Praktikum-DES/des-getopt.ggo
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../Praktikum-DES/des-getopt.h, ../../Praktikum-DES/des-getopt.c"
	cd /home/daniel/K/Crypto/AK/src/Praktikum-DES && /usr/bin/gengetopt --include-getopt --unamed-opts -i des-getopt.ggo -F des-getopt

../Praktikum-DES/des-getopt.c: ../Praktikum-DES/des-getopt.h
	@$(CMAKE_COMMAND) -E touch_nocreate ../Praktikum-DES/des-getopt.c

Praktikum-DES/CMakeFiles/des.dir/des-main.cpp.o: Praktikum-DES/CMakeFiles/des.dir/flags.make
Praktikum-DES/CMakeFiles/des.dir/des-main.cpp.o: ../Praktikum-DES/des-main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Praktikum-DES/CMakeFiles/des.dir/des-main.cpp.o"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/des.dir/des-main.cpp.o -c /home/daniel/K/Crypto/AK/src/Praktikum-DES/des-main.cpp

Praktikum-DES/CMakeFiles/des.dir/des-main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/des.dir/des-main.cpp.i"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/K/Crypto/AK/src/Praktikum-DES/des-main.cpp > CMakeFiles/des.dir/des-main.cpp.i

Praktikum-DES/CMakeFiles/des.dir/des-main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/des.dir/des-main.cpp.s"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/K/Crypto/AK/src/Praktikum-DES/des-main.cpp -o CMakeFiles/des.dir/des-main.cpp.s

Praktikum-DES/CMakeFiles/des.dir/DESCipher.cpp.o: Praktikum-DES/CMakeFiles/des.dir/flags.make
Praktikum-DES/CMakeFiles/des.dir/DESCipher.cpp.o: ../Praktikum-DES/DESCipher.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Praktikum-DES/CMakeFiles/des.dir/DESCipher.cpp.o"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/des.dir/DESCipher.cpp.o -c /home/daniel/K/Crypto/AK/src/Praktikum-DES/DESCipher.cpp

Praktikum-DES/CMakeFiles/des.dir/DESCipher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/des.dir/DESCipher.cpp.i"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/K/Crypto/AK/src/Praktikum-DES/DESCipher.cpp > CMakeFiles/des.dir/DESCipher.cpp.i

Praktikum-DES/CMakeFiles/des.dir/DESCipher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/des.dir/DESCipher.cpp.s"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/K/Crypto/AK/src/Praktikum-DES/DESCipher.cpp -o CMakeFiles/des.dir/DESCipher.cpp.s

Praktikum-DES/CMakeFiles/des.dir/BlockCipher.cpp.o: Praktikum-DES/CMakeFiles/des.dir/flags.make
Praktikum-DES/CMakeFiles/des.dir/BlockCipher.cpp.o: ../Praktikum-DES/BlockCipher.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object Praktikum-DES/CMakeFiles/des.dir/BlockCipher.cpp.o"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/des.dir/BlockCipher.cpp.o -c /home/daniel/K/Crypto/AK/src/Praktikum-DES/BlockCipher.cpp

Praktikum-DES/CMakeFiles/des.dir/BlockCipher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/des.dir/BlockCipher.cpp.i"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/daniel/K/Crypto/AK/src/Praktikum-DES/BlockCipher.cpp > CMakeFiles/des.dir/BlockCipher.cpp.i

Praktikum-DES/CMakeFiles/des.dir/BlockCipher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/des.dir/BlockCipher.cpp.s"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/daniel/K/Crypto/AK/src/Praktikum-DES/BlockCipher.cpp -o CMakeFiles/des.dir/BlockCipher.cpp.s

Praktikum-DES/CMakeFiles/des.dir/des-getopt.c.o: Praktikum-DES/CMakeFiles/des.dir/flags.make
Praktikum-DES/CMakeFiles/des.dir/des-getopt.c.o: ../Praktikum-DES/des-getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object Praktikum-DES/CMakeFiles/des.dir/des-getopt.c.o"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/des.dir/des-getopt.c.o   -c /home/daniel/K/Crypto/AK/src/Praktikum-DES/des-getopt.c

Praktikum-DES/CMakeFiles/des.dir/des-getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/des.dir/des-getopt.c.i"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/daniel/K/Crypto/AK/src/Praktikum-DES/des-getopt.c > CMakeFiles/des.dir/des-getopt.c.i

Praktikum-DES/CMakeFiles/des.dir/des-getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/des.dir/des-getopt.c.s"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/daniel/K/Crypto/AK/src/Praktikum-DES/des-getopt.c -o CMakeFiles/des.dir/des-getopt.c.s

# Object files for target des
des_OBJECTS = \
"CMakeFiles/des.dir/des-main.cpp.o" \
"CMakeFiles/des.dir/DESCipher.cpp.o" \
"CMakeFiles/des.dir/BlockCipher.cpp.o" \
"CMakeFiles/des.dir/des-getopt.c.o"

# External object files for target des
des_EXTERNAL_OBJECTS =

bin/des: Praktikum-DES/CMakeFiles/des.dir/des-main.cpp.o
bin/des: Praktikum-DES/CMakeFiles/des.dir/DESCipher.cpp.o
bin/des: Praktikum-DES/CMakeFiles/des.dir/BlockCipher.cpp.o
bin/des: Praktikum-DES/CMakeFiles/des.dir/des-getopt.c.o
bin/des: Praktikum-DES/CMakeFiles/des.dir/build.make
bin/des: Praktikum-DES/CMakeFiles/des.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/daniel/K/Crypto/AK/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable ../bin/des"
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/des.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Praktikum-DES/CMakeFiles/des.dir/build: bin/des

.PHONY : Praktikum-DES/CMakeFiles/des.dir/build

Praktikum-DES/CMakeFiles/des.dir/clean:
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES && $(CMAKE_COMMAND) -P CMakeFiles/des.dir/cmake_clean.cmake
.PHONY : Praktikum-DES/CMakeFiles/des.dir/clean

Praktikum-DES/CMakeFiles/des.dir/depend: ../Praktikum-DES/des-getopt.h
Praktikum-DES/CMakeFiles/des.dir/depend: ../Praktikum-DES/des-getopt.c
	cd /home/daniel/K/Crypto/AK/src/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/daniel/K/Crypto/AK/src /home/daniel/K/Crypto/AK/src/Praktikum-DES /home/daniel/K/Crypto/AK/src/cmake-build-debug /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES /home/daniel/K/Crypto/AK/src/cmake-build-debug/Praktikum-DES/CMakeFiles/des.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Praktikum-DES/CMakeFiles/des.dir/depend
