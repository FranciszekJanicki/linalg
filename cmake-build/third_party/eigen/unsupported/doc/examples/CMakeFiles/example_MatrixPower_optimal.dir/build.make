# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/franek/Documents/C++/Kalman

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/franek/Documents/C++/Kalman/cmake-build

# Include any dependencies generated for this target.
include third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/progress.make

# Include the compile flags for this target's objects.
include third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/flags.make

third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o: third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/flags.make
third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o: /home/franek/Documents/C++/Kalman/third_party/eigen/unsupported/doc/examples/MatrixPower_optimal.cpp
third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o: third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/franek/Documents/C++/Kalman/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o"
	cd /home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o -MF CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o.d -o CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o -c /home/franek/Documents/C++/Kalman/third_party/eigen/unsupported/doc/examples/MatrixPower_optimal.cpp

third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.i"
	cd /home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/franek/Documents/C++/Kalman/third_party/eigen/unsupported/doc/examples/MatrixPower_optimal.cpp > CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.i

third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.s"
	cd /home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/franek/Documents/C++/Kalman/third_party/eigen/unsupported/doc/examples/MatrixPower_optimal.cpp -o CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.s

# Object files for target example_MatrixPower_optimal
example_MatrixPower_optimal_OBJECTS = \
"CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o"

# External object files for target example_MatrixPower_optimal
example_MatrixPower_optimal_EXTERNAL_OBJECTS =

third_party/eigen/unsupported/doc/examples/example_MatrixPower_optimal: third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/MatrixPower_optimal.cpp.o
third_party/eigen/unsupported/doc/examples/example_MatrixPower_optimal: third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/build.make
third_party/eigen/unsupported/doc/examples/example_MatrixPower_optimal: third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/franek/Documents/C++/Kalman/cmake-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable example_MatrixPower_optimal"
	cd /home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example_MatrixPower_optimal.dir/link.txt --verbose=$(VERBOSE)
	cd /home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples && ./example_MatrixPower_optimal >/home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples/MatrixPower_optimal.out

# Rule to build all files generated by this target.
third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/build: third_party/eigen/unsupported/doc/examples/example_MatrixPower_optimal
.PHONY : third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/build

third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/clean:
	cd /home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples && $(CMAKE_COMMAND) -P CMakeFiles/example_MatrixPower_optimal.dir/cmake_clean.cmake
.PHONY : third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/clean

third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/depend:
	cd /home/franek/Documents/C++/Kalman/cmake-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/franek/Documents/C++/Kalman /home/franek/Documents/C++/Kalman/third_party/eigen/unsupported/doc/examples /home/franek/Documents/C++/Kalman/cmake-build /home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples /home/franek/Documents/C++/Kalman/cmake-build/third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : third_party/eigen/unsupported/doc/examples/CMakeFiles/example_MatrixPower_optimal.dir/depend

