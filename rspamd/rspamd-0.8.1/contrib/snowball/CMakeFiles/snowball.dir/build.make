# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/linux/rspamd/rspamd-0.8.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/linux/rspamd/rspamd-0.8.1

# Include any dependencies generated for this target.
include contrib/snowball/CMakeFiles/snowball.dir/depend.make

# Include the progress variables for this target.
include contrib/snowball/CMakeFiles/snowball.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/snowball/CMakeFiles/snowball.dir/flags.make

contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o: contrib/snowball/CMakeFiles/snowball.dir/flags.make
contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o: contrib/snowball/compiler/space.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd-0.8.1/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/snowball.dir/compiler/space.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/space.c

contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/snowball.dir/compiler/space.c.i"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/space.c > CMakeFiles/snowball.dir/compiler/space.c.i

contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/snowball.dir/compiler/space.c.s"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/space.c -o CMakeFiles/snowball.dir/compiler/space.c.s

contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o.requires:
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o.requires

contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o.provides: contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o.requires
	$(MAKE) -f contrib/snowball/CMakeFiles/snowball.dir/build.make contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o.provides.build
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o.provides

contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o.provides.build: contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o

contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o: contrib/snowball/CMakeFiles/snowball.dir/flags.make
contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o: contrib/snowball/compiler/tokeniser.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd-0.8.1/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/snowball.dir/compiler/tokeniser.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/tokeniser.c

contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/snowball.dir/compiler/tokeniser.c.i"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/tokeniser.c > CMakeFiles/snowball.dir/compiler/tokeniser.c.i

contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/snowball.dir/compiler/tokeniser.c.s"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/tokeniser.c -o CMakeFiles/snowball.dir/compiler/tokeniser.c.s

contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o.requires:
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o.requires

contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o.provides: contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o.requires
	$(MAKE) -f contrib/snowball/CMakeFiles/snowball.dir/build.make contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o.provides.build
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o.provides

contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o.provides.build: contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o

contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o: contrib/snowball/CMakeFiles/snowball.dir/flags.make
contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o: contrib/snowball/compiler/analyser.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd-0.8.1/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/snowball.dir/compiler/analyser.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/analyser.c

contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/snowball.dir/compiler/analyser.c.i"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/analyser.c > CMakeFiles/snowball.dir/compiler/analyser.c.i

contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/snowball.dir/compiler/analyser.c.s"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/analyser.c -o CMakeFiles/snowball.dir/compiler/analyser.c.s

contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o.requires:
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o.requires

contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o.provides: contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o.requires
	$(MAKE) -f contrib/snowball/CMakeFiles/snowball.dir/build.make contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o.provides.build
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o.provides

contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o.provides.build: contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o: contrib/snowball/CMakeFiles/snowball.dir/flags.make
contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o: contrib/snowball/compiler/generator.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd-0.8.1/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/snowball.dir/compiler/generator.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/generator.c

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/snowball.dir/compiler/generator.c.i"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/generator.c > CMakeFiles/snowball.dir/compiler/generator.c.i

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/snowball.dir/compiler/generator.c.s"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/generator.c -o CMakeFiles/snowball.dir/compiler/generator.c.s

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o.requires:
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o.requires

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o.provides: contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o.requires
	$(MAKE) -f contrib/snowball/CMakeFiles/snowball.dir/build.make contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o.provides.build
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o.provides

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o.provides.build: contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o

contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o: contrib/snowball/CMakeFiles/snowball.dir/flags.make
contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o: contrib/snowball/compiler/driver.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd-0.8.1/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/snowball.dir/compiler/driver.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/driver.c

contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/snowball.dir/compiler/driver.c.i"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/driver.c > CMakeFiles/snowball.dir/compiler/driver.c.i

contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/snowball.dir/compiler/driver.c.s"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/driver.c -o CMakeFiles/snowball.dir/compiler/driver.c.s

contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o.requires:
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o.requires

contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o.provides: contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o.requires
	$(MAKE) -f contrib/snowball/CMakeFiles/snowball.dir/build.make contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o.provides.build
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o.provides

contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o.provides.build: contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o: contrib/snowball/CMakeFiles/snowball.dir/flags.make
contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o: contrib/snowball/compiler/generator_java.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd-0.8.1/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/snowball.dir/compiler/generator_java.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/generator_java.c

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/snowball.dir/compiler/generator_java.c.i"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/generator_java.c > CMakeFiles/snowball.dir/compiler/generator_java.c.i

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/snowball.dir/compiler/generator_java.c.s"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/compiler/generator_java.c -o CMakeFiles/snowball.dir/compiler/generator_java.c.s

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o.requires:
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o.requires

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o.provides: contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o.requires
	$(MAKE) -f contrib/snowball/CMakeFiles/snowball.dir/build.make contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o.provides.build
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o.provides

contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o.provides.build: contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o

# Object files for target snowball
snowball_OBJECTS = \
"CMakeFiles/snowball.dir/compiler/space.c.o" \
"CMakeFiles/snowball.dir/compiler/tokeniser.c.o" \
"CMakeFiles/snowball.dir/compiler/analyser.c.o" \
"CMakeFiles/snowball.dir/compiler/generator.c.o" \
"CMakeFiles/snowball.dir/compiler/driver.c.o" \
"CMakeFiles/snowball.dir/compiler/generator_java.c.o"

# External object files for target snowball
snowball_EXTERNAL_OBJECTS =

contrib/snowball/snowball: contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o
contrib/snowball/snowball: contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o
contrib/snowball/snowball: contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o
contrib/snowball/snowball: contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o
contrib/snowball/snowball: contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o
contrib/snowball/snowball: contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o
contrib/snowball/snowball: contrib/snowball/CMakeFiles/snowball.dir/build.make
contrib/snowball/snowball: contrib/snowball/CMakeFiles/snowball.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable snowball"
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/snowball.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/snowball/CMakeFiles/snowball.dir/build: contrib/snowball/snowball
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/build

contrib/snowball/CMakeFiles/snowball.dir/requires: contrib/snowball/CMakeFiles/snowball.dir/compiler/space.c.o.requires
contrib/snowball/CMakeFiles/snowball.dir/requires: contrib/snowball/CMakeFiles/snowball.dir/compiler/tokeniser.c.o.requires
contrib/snowball/CMakeFiles/snowball.dir/requires: contrib/snowball/CMakeFiles/snowball.dir/compiler/analyser.c.o.requires
contrib/snowball/CMakeFiles/snowball.dir/requires: contrib/snowball/CMakeFiles/snowball.dir/compiler/generator.c.o.requires
contrib/snowball/CMakeFiles/snowball.dir/requires: contrib/snowball/CMakeFiles/snowball.dir/compiler/driver.c.o.requires
contrib/snowball/CMakeFiles/snowball.dir/requires: contrib/snowball/CMakeFiles/snowball.dir/compiler/generator_java.c.o.requires
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/requires

contrib/snowball/CMakeFiles/snowball.dir/clean:
	cd /home/linux/rspamd/rspamd-0.8.1/contrib/snowball && $(CMAKE_COMMAND) -P CMakeFiles/snowball.dir/cmake_clean.cmake
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/clean

contrib/snowball/CMakeFiles/snowball.dir/depend:
	cd /home/linux/rspamd/rspamd-0.8.1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linux/rspamd/rspamd-0.8.1 /home/linux/rspamd/rspamd-0.8.1/contrib/snowball /home/linux/rspamd/rspamd-0.8.1 /home/linux/rspamd/rspamd-0.8.1/contrib/snowball /home/linux/rspamd/rspamd-0.8.1/contrib/snowball/CMakeFiles/snowball.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : contrib/snowball/CMakeFiles/snowball.dir/depend

