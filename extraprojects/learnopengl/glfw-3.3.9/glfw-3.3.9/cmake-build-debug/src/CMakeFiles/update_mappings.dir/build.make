# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /app/extra/clion/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /app/extra/clion/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/cmake-build-debug

# Utility rule file for update_mappings.

# Include any custom commands dependencies for this target.
include src/CMakeFiles/update_mappings.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/update_mappings.dir/progress.make

src/CMakeFiles/update_mappings:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Updating gamepad mappings from upstream repository"
	cd /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/src && /app/extra/clion/bin/cmake/linux/x64/bin/cmake -P /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/CMake/GenerateMappings.cmake mappings.h.in mappings.h

update_mappings: src/CMakeFiles/update_mappings
update_mappings: src/CMakeFiles/update_mappings.dir/build.make
.PHONY : update_mappings

# Rule to build all files generated by this target.
src/CMakeFiles/update_mappings.dir/build: update_mappings
.PHONY : src/CMakeFiles/update_mappings.dir/build

src/CMakeFiles/update_mappings.dir/clean:
	cd /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/cmake-build-debug/src && $(CMAKE_COMMAND) -P CMakeFiles/update_mappings.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/update_mappings.dir/clean

src/CMakeFiles/update_mappings.dir/depend:
	cd /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9 /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/src /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/cmake-build-debug /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/cmake-build-debug/src /home/oskar/cphb-dat/extraprojects/learnopengl/glfw-3.3.9/glfw-3.3.9/cmake-build-debug/src/CMakeFiles/update_mappings.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/CMakeFiles/update_mappings.dir/depend

