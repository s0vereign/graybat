# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_SOURCE_DIR = /home/erik/projects/graybat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/erik/projects/graybat/build

# Include any dependencies generated for this target.
include CMakeFiles/check.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/check.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/check.dir/flags.make

CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o: CMakeFiles/check.dir/flags.make
CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o: ../test/CommunicationPolicyUT.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/erik/projects/graybat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o"
	ccache /usr/bin/clang++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o -c /home/erik/projects/graybat/test/CommunicationPolicyUT.cpp

CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.i"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/erik/projects/graybat/test/CommunicationPolicyUT.cpp > CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.i

CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.s"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/erik/projects/graybat/test/CommunicationPolicyUT.cpp -o CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.s

CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o.requires:

.PHONY : CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o.requires

CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o.provides: CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o.requires
	$(MAKE) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o.provides.build
.PHONY : CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o.provides

CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o.provides.build: CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o


CMakeFiles/check.dir/test/VertexUT.cpp.o: CMakeFiles/check.dir/flags.make
CMakeFiles/check.dir/test/VertexUT.cpp.o: ../test/VertexUT.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/erik/projects/graybat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/check.dir/test/VertexUT.cpp.o"
	ccache /usr/bin/clang++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/check.dir/test/VertexUT.cpp.o -c /home/erik/projects/graybat/test/VertexUT.cpp

CMakeFiles/check.dir/test/VertexUT.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/check.dir/test/VertexUT.cpp.i"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/erik/projects/graybat/test/VertexUT.cpp > CMakeFiles/check.dir/test/VertexUT.cpp.i

CMakeFiles/check.dir/test/VertexUT.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/check.dir/test/VertexUT.cpp.s"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/erik/projects/graybat/test/VertexUT.cpp -o CMakeFiles/check.dir/test/VertexUT.cpp.s

CMakeFiles/check.dir/test/VertexUT.cpp.o.requires:

.PHONY : CMakeFiles/check.dir/test/VertexUT.cpp.o.requires

CMakeFiles/check.dir/test/VertexUT.cpp.o.provides: CMakeFiles/check.dir/test/VertexUT.cpp.o.requires
	$(MAKE) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/test/VertexUT.cpp.o.provides.build
.PHONY : CMakeFiles/check.dir/test/VertexUT.cpp.o.provides

CMakeFiles/check.dir/test/VertexUT.cpp.o.provides.build: CMakeFiles/check.dir/test/VertexUT.cpp.o


CMakeFiles/check.dir/test/CageUT.cpp.o: CMakeFiles/check.dir/flags.make
CMakeFiles/check.dir/test/CageUT.cpp.o: ../test/CageUT.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/erik/projects/graybat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/check.dir/test/CageUT.cpp.o"
	ccache /usr/bin/clang++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/check.dir/test/CageUT.cpp.o -c /home/erik/projects/graybat/test/CageUT.cpp

CMakeFiles/check.dir/test/CageUT.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/check.dir/test/CageUT.cpp.i"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/erik/projects/graybat/test/CageUT.cpp > CMakeFiles/check.dir/test/CageUT.cpp.i

CMakeFiles/check.dir/test/CageUT.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/check.dir/test/CageUT.cpp.s"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/erik/projects/graybat/test/CageUT.cpp -o CMakeFiles/check.dir/test/CageUT.cpp.s

CMakeFiles/check.dir/test/CageUT.cpp.o.requires:

.PHONY : CMakeFiles/check.dir/test/CageUT.cpp.o.requires

CMakeFiles/check.dir/test/CageUT.cpp.o.provides: CMakeFiles/check.dir/test/CageUT.cpp.o.requires
	$(MAKE) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/test/CageUT.cpp.o.provides.build
.PHONY : CMakeFiles/check.dir/test/CageUT.cpp.o.provides

CMakeFiles/check.dir/test/CageUT.cpp.o.provides.build: CMakeFiles/check.dir/test/CageUT.cpp.o


CMakeFiles/check.dir/test/UT.cpp.o: CMakeFiles/check.dir/flags.make
CMakeFiles/check.dir/test/UT.cpp.o: ../test/UT.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/erik/projects/graybat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/check.dir/test/UT.cpp.o"
	ccache /usr/bin/clang++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/check.dir/test/UT.cpp.o -c /home/erik/projects/graybat/test/UT.cpp

CMakeFiles/check.dir/test/UT.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/check.dir/test/UT.cpp.i"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/erik/projects/graybat/test/UT.cpp > CMakeFiles/check.dir/test/UT.cpp.i

CMakeFiles/check.dir/test/UT.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/check.dir/test/UT.cpp.s"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/erik/projects/graybat/test/UT.cpp -o CMakeFiles/check.dir/test/UT.cpp.s

CMakeFiles/check.dir/test/UT.cpp.o.requires:

.PHONY : CMakeFiles/check.dir/test/UT.cpp.o.requires

CMakeFiles/check.dir/test/UT.cpp.o.provides: CMakeFiles/check.dir/test/UT.cpp.o.requires
	$(MAKE) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/test/UT.cpp.o.provides.build
.PHONY : CMakeFiles/check.dir/test/UT.cpp.o.provides

CMakeFiles/check.dir/test/UT.cpp.o.provides.build: CMakeFiles/check.dir/test/UT.cpp.o


CMakeFiles/check.dir/test/EdgeUT.cpp.o: CMakeFiles/check.dir/flags.make
CMakeFiles/check.dir/test/EdgeUT.cpp.o: ../test/EdgeUT.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/erik/projects/graybat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/check.dir/test/EdgeUT.cpp.o"
	ccache /usr/bin/clang++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/check.dir/test/EdgeUT.cpp.o -c /home/erik/projects/graybat/test/EdgeUT.cpp

CMakeFiles/check.dir/test/EdgeUT.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/check.dir/test/EdgeUT.cpp.i"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/erik/projects/graybat/test/EdgeUT.cpp > CMakeFiles/check.dir/test/EdgeUT.cpp.i

CMakeFiles/check.dir/test/EdgeUT.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/check.dir/test/EdgeUT.cpp.s"
	ccache /usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/erik/projects/graybat/test/EdgeUT.cpp -o CMakeFiles/check.dir/test/EdgeUT.cpp.s

CMakeFiles/check.dir/test/EdgeUT.cpp.o.requires:

.PHONY : CMakeFiles/check.dir/test/EdgeUT.cpp.o.requires

CMakeFiles/check.dir/test/EdgeUT.cpp.o.provides: CMakeFiles/check.dir/test/EdgeUT.cpp.o.requires
	$(MAKE) -f CMakeFiles/check.dir/build.make CMakeFiles/check.dir/test/EdgeUT.cpp.o.provides.build
.PHONY : CMakeFiles/check.dir/test/EdgeUT.cpp.o.provides

CMakeFiles/check.dir/test/EdgeUT.cpp.o.provides.build: CMakeFiles/check.dir/test/EdgeUT.cpp.o


# Object files for target check
check_OBJECTS = \
"CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o" \
"CMakeFiles/check.dir/test/VertexUT.cpp.o" \
"CMakeFiles/check.dir/test/CageUT.cpp.o" \
"CMakeFiles/check.dir/test/UT.cpp.o" \
"CMakeFiles/check.dir/test/EdgeUT.cpp.o"

# External object files for target check
check_EXTERNAL_OBJECTS =

check: CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o
check: CMakeFiles/check.dir/test/VertexUT.cpp.o
check: CMakeFiles/check.dir/test/CageUT.cpp.o
check: CMakeFiles/check.dir/test/UT.cpp.o
check: CMakeFiles/check.dir/test/EdgeUT.cpp.o
check: CMakeFiles/check.dir/build.make
check: /usr/lib64/libboost_unit_test_framework.so
check: /usr/lib64/libboost_program_options.so
check: /usr/lib64/libzmq.so
check: /usr/lib64/libboost_mpi.so
check: /usr/lib64/libboost_serialization.so
check: /usr/lib64/openmpi/libmpi.so
check: /usr/lib64/openmpi/libmpi_cxx.so
check: /usr/lib64/openmpi/libmpi.so
check: /usr/lib64/openmpi/libmpi_cxx.so
check: CMakeFiles/check.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/erik/projects/graybat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable check"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/check.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/check.dir/build: check

.PHONY : CMakeFiles/check.dir/build

CMakeFiles/check.dir/requires: CMakeFiles/check.dir/test/CommunicationPolicyUT.cpp.o.requires
CMakeFiles/check.dir/requires: CMakeFiles/check.dir/test/VertexUT.cpp.o.requires
CMakeFiles/check.dir/requires: CMakeFiles/check.dir/test/CageUT.cpp.o.requires
CMakeFiles/check.dir/requires: CMakeFiles/check.dir/test/UT.cpp.o.requires
CMakeFiles/check.dir/requires: CMakeFiles/check.dir/test/EdgeUT.cpp.o.requires

.PHONY : CMakeFiles/check.dir/requires

CMakeFiles/check.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/check.dir/cmake_clean.cmake
.PHONY : CMakeFiles/check.dir/clean

CMakeFiles/check.dir/depend:
	cd /home/erik/projects/graybat/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/erik/projects/graybat /home/erik/projects/graybat /home/erik/projects/graybat/build /home/erik/projects/graybat/build /home/erik/projects/graybat/build/CMakeFiles/check.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/check.dir/depend

