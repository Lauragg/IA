# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/laura/GitHub/IA/Practica2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/laura/GitHub/IA/Practica2

# Include any dependencies generated for this target.
include CMakeFiles/Belkan.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Belkan.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Belkan.dir/flags.make

CMakeFiles/Belkan.dir/bin_src/juego.cc.o: CMakeFiles/Belkan.dir/flags.make
CMakeFiles/Belkan.dir/bin_src/juego.cc.o: bin_src/juego.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/laura/GitHub/IA/Practica2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Belkan.dir/bin_src/juego.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Belkan.dir/bin_src/juego.cc.o -c /home/laura/GitHub/IA/Practica2/bin_src/juego.cc

CMakeFiles/Belkan.dir/bin_src/juego.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Belkan.dir/bin_src/juego.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/laura/GitHub/IA/Practica2/bin_src/juego.cc > CMakeFiles/Belkan.dir/bin_src/juego.cc.i

CMakeFiles/Belkan.dir/bin_src/juego.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Belkan.dir/bin_src/juego.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/laura/GitHub/IA/Practica2/bin_src/juego.cc -o CMakeFiles/Belkan.dir/bin_src/juego.cc.s

CMakeFiles/Belkan.dir/bin_src/juego.cc.o.requires:

.PHONY : CMakeFiles/Belkan.dir/bin_src/juego.cc.o.requires

CMakeFiles/Belkan.dir/bin_src/juego.cc.o.provides: CMakeFiles/Belkan.dir/bin_src/juego.cc.o.requires
	$(MAKE) -f CMakeFiles/Belkan.dir/build.make CMakeFiles/Belkan.dir/bin_src/juego.cc.o.provides.build
.PHONY : CMakeFiles/Belkan.dir/bin_src/juego.cc.o.provides

CMakeFiles/Belkan.dir/bin_src/juego.cc.o.provides.build: CMakeFiles/Belkan.dir/bin_src/juego.cc.o


# Object files for target Belkan
Belkan_OBJECTS = \
"CMakeFiles/Belkan.dir/bin_src/juego.cc.o"

# External object files for target Belkan
Belkan_EXTERNAL_OBJECTS =

Belkan: CMakeFiles/Belkan.dir/bin_src/juego.cc.o
Belkan: CMakeFiles/Belkan.dir/build.make
Belkan: lib/libobj3DLib.a
Belkan: lib/libmotorLib.a
Belkan: lib/libmodelosBelkanLib.a
Belkan: lib/libGLUILib.a
Belkan: lib/libcomportamientosLib.a
Belkan: lib/libobj3DLib.a
Belkan: /usr/lib/x86_64-linux-gnu/libjpeg.so
Belkan: /usr/lib/x86_64-linux-gnu/libGLU.so
Belkan: /usr/lib/x86_64-linux-gnu/libGL.so
Belkan: /usr/lib/x86_64-linux-gnu/libglut.so
Belkan: /usr/lib/x86_64-linux-gnu/libXmu.so
Belkan: /usr/lib/x86_64-linux-gnu/libXi.so
Belkan: CMakeFiles/Belkan.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/laura/GitHub/IA/Practica2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Belkan"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Belkan.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Belkan.dir/build: Belkan

.PHONY : CMakeFiles/Belkan.dir/build

CMakeFiles/Belkan.dir/requires: CMakeFiles/Belkan.dir/bin_src/juego.cc.o.requires

.PHONY : CMakeFiles/Belkan.dir/requires

CMakeFiles/Belkan.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Belkan.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Belkan.dir/clean

CMakeFiles/Belkan.dir/depend:
	cd /home/laura/GitHub/IA/Practica2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/laura/GitHub/IA/Practica2 /home/laura/GitHub/IA/Practica2 /home/laura/GitHub/IA/Practica2 /home/laura/GitHub/IA/Practica2 /home/laura/GitHub/IA/Practica2/CMakeFiles/Belkan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Belkan.dir/depend

